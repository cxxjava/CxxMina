/*
 * EAbstractHttpLogicHandler.cpp
 *
 *  Created on: 2016-7-7
 *      Author: cxxjava@163.com
 */

#include "EAbstractHttpLogicHandler.hh"
#include "EIoBufferDecoder.hh"
#include "EStringUtilities.hh"
#include "EHttpProxyConstants.hh"
#include "EAbstractAuthLogicHandler.hh"

namespace efc {
namespace eio {

sp<ELogger> EAbstractHttpLogicHandler::LOGGER = ELoggerManager::getLogger("EAbstractHttpLogicHandler");
sp<EString> EAbstractHttpLogicHandler::DECODER = new EString("EAbstractHttpLogicHandler.Decoder");

EAbstractHttpLogicHandler::~EAbstractHttpLogicHandler() {
	//
}

EAbstractHttpLogicHandler::EAbstractHttpLogicHandler(
		sp<EProxyIoSession>& proxyIoSession) : EAbstractProxyLogicHandler(proxyIoSession),
		contentLength(-1),
		hasChunkedData(false),
		waitingChunkedData(false),
		waitingFooters(false),
		entityBodyStartPosition(0),
		entityBodyLimitPosition(0) {
}

void EAbstractHttpLogicHandler::messageReceived(
		EIoFilter::NextFilter* nextFilter, sp<EIoBuffer>& buf) {
	LOGGER->debug(__FILE__, __LINE__, " messageReceived()");

	sp<EIoBufferDecoder> decoder = dynamic_pointer_cast<EIoBufferDecoder>(getSession()->getAttribute(DECODER.get()));
	if (decoder == null) {
		decoder = new EIoBufferDecoder(HTTP_DELIMITER);
		getSession()->setAttribute(DECODER, decoder);
	}

	try {
		if (parsedResponse == null) {

			responseData = decoder->decodeFully(buf);
			if (responseData == null) {
				return;
			}

			// Handle the response
			EString responseHeader = responseData->getString(/*getProxyIoSession().getCharset().newDecoder()*/);
			entityBodyStartPosition = responseData->position();

			LOGGER->debug__(__FILE__, __LINE__, "  response header received:\n%s",
					responseHeader.replace("\r", "\\r").replace("\n", "\\n\n").c_str());

			// Parse the response
			parsedResponse = decodeResponse(responseHeader.c_str());

			// Is handshake complete ?
			if (parsedResponse->getStatusCode() == 200
					|| (parsedResponse->getStatusCode() >= 300 && parsedResponse->getStatusCode() <= 307)) {
				buf->position(0);
				setHandshakeComplete();
				return;
			}

			//@see: String contentLengthHeader = StringUtilities.getSingleValuedHeader(parsedResponse.getHeaders(), "Content-Length");
			EString k("Content-Length");
			EString* contentLengthHeader = parsedResponse->getHeaders()->get(&k)->getAt(0);
			if (contentLengthHeader == null) {
				contentLength = 0;
			} else {
				contentLength = EInteger::parseInt(contentLengthHeader->c_str()); //trim?
				decoder->setContentLength(contentLength, true);
			}
		}

		if (!hasChunkedData) {
			if (contentLength > 0) {
				sp<EIoBuffer> tmp = decoder->decodeFully(buf);
				if (tmp == null) {
					return;
				}
				responseData->setAutoExpand(true);
				responseData->put(tmp.get());
				contentLength = 0;
			}

			if (EAbstractAuthLogicHandler::getSingleValuedHeader(parsedResponse->getHeaders(),
					"Transfer-Encoding").equalsIgnoreCase("chunked")) {
				// Handle Transfer-Encoding: Chunked
				LOGGER->debug(__FILE__, __LINE__, "Retrieving additional http response chunks");
				hasChunkedData = true;
				waitingChunkedData = true;
			}
		}

		if (hasChunkedData) {
			// Read chunks
			while (waitingChunkedData) {
				if (contentLength == 0) {
					decoder->setDelimiter(CRLF_DELIMITER, false);
					sp<EIoBuffer> tmp = decoder->decodeFully(buf);
					if (tmp == null) {
						return;
					}

					EString chunkSize = tmp->getString(/*getProxyIoSession().getCharset().newDecoder()*/);
					int pos = chunkSize.indexOf(';');
					if (pos >= 0) {
						chunkSize = chunkSize.substring(0, pos);
					} else {
						chunkSize = chunkSize.substring(0, chunkSize.length() - 2);
					}
					contentLength = EInteger::valueOf(chunkSize.c_str(), 16).value; //@see: Integer.decode("0x" + chunkSize);
					if (contentLength > 0) {
						contentLength += 2; // also read chunk's trailing CRLF
						decoder->setContentLength(contentLength, true);
					}
				}

				if (contentLength == 0) {
					waitingChunkedData = false;
					waitingFooters = true;
					entityBodyLimitPosition = responseData->position();
					break;
				}

				sp<EIoBuffer> tmp = decoder->decodeFully(buf);
				if (tmp == null) {
					return;
				}
				contentLength = 0;
				responseData->put(tmp.get());
				buf->position(buf->position());
			}

			// Read footers
			while (waitingFooters) {
				decoder->setDelimiter(CRLF_DELIMITER, false);
				sp<EIoBuffer> tmp = decoder->decodeFully(buf);
				if (tmp == null) {
					return;
				}

				if (tmp->remaining() == 2) {
					waitingFooters = false;
					break;
				}

				// add footer to headers
				EString footer = tmp->getString(/*getProxyIoSession().getCharset().newDecoder()*/);
				EArray<EString*> f = EPattern::split(":\\s?", footer.c_str(), 2);
                EAbstractAuthLogicHandler::addValueToHeader(parsedResponse->getHeaders(), f[0]->c_str(), f[1]->c_str(), false);
				responseData->put(tmp.get());
				responseData->putString(CRLF_DELIMITER);
			}
		}

		responseData->flip();

		LOGGER->debug__(__FILE__, __LINE__, "  end of response received:\n%s",
				responseData->getString(/*getProxyIoSession().getCharset().newDecoder()*/).c_str());

		// Retrieve entity body content
		responseData->position(entityBodyStartPosition);
		responseData->limit(entityBodyLimitPosition);
		parsedResponse->setBody(responseData->getString(/*getProxyIoSession().getCharset().newDecoder()*/).c_str());

		// Free the response buffer
		responseData->free();
		responseData = null;

		handleResponse(parsedResponse);

		parsedResponse = null;
		hasChunkedData = false;
		contentLength = -1;
		decoder->setDelimiter(HTTP_DELIMITER, true);

		if (!isHandshakeComplete()) {
			doHandshake(nextFilter);
		}
	} catch (EException& ex) {
		if (instanceof<EProxyAuthException>(ex)) {
			throw ex; //@see: throw ((ProxyAuthException) ex);
		}

		throw EProxyAuthException(__FILE__, __LINE__, "Handshake failed", &ex);
	}
}

void EAbstractHttpLogicHandler::writeRequest(EIoFilter::NextFilter* nextFilter,
		sp<EHttpProxyRequest>& request) {
	sp<EProxyIoSession> proxyIoSession = getProxyIoSession();

	if (proxyIoSession->isReconnectionNeeded()) {
		reconnect(nextFilter, request);
	} else {
		writeRequest0(nextFilter, request);
	}
}

sp<EHttpProxyResponse> EAbstractHttpLogicHandler::decodeResponse(
		const char* response) {
	LOGGER->debug(__FILE__, __LINE__, "  parseResponse()");

	// Break response into lines
	EArray<EString*> responseLines = EPattern::split(EHttpProxyConstants_CRLF, response);

	// Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
	// BUG FIX : Trimed to prevent failures with some proxies that add
	// extra space chars like "Microsoft-IIS/5.0" ...
	EArray<EString*> statusLine = EPattern::split(" ", responseLines[0]->trim().c_str(), 2);

	if (statusLine.length() < 2) {
		EString msg("Invalid response status line (" + statusLine.toString() + "). Response: " + response);
		throw EException(__FILE__, __LINE__, msg.c_str());
	}

	// Status code is 3 digits
	if (!EPattern::matches("^\\d\\d\\d", statusLine[1]->c_str())) { //@see: if (!statusLine[1].matches("^\\d\\d\\d")) {
		throw EException(__FILE__, __LINE__,
			EString::formatOf("Invalid response code (%s). Response: %s",
					statusLine[1]->c_str(), response).c_str());
	}

	EHashMap<EString*, EList<EString*>*> headers;

	for (int i = 1; i < responseLines.length(); i++) {
		EArray<EString*> args = EPattern::split(":\\s?", responseLines[i]->c_str(), 2);
		EAbstractAuthLogicHandler::addValueToHeader(&headers, args[0]->c_str(), args[1]->c_str(), false);
	}

	return new EHttpProxyResponse(statusLine[0]->c_str(), statusLine[1]->c_str(), &headers);
}

void EAbstractHttpLogicHandler::writeRequest0(EIoFilter::NextFilter* nextFilter,
		sp<EHttpProxyRequest>& request) {
	try {
		EString data = request->toHttpString();
		/* @see:
		IoBuffer buf = IoBuffer.wrap(data.getBytes(getProxyIoSession().getCharsetName()));
		 */
		sp<EIoBuffer> buf = EIoBuffer::allocate(data.length());
		buf->putString(data.c_str());
		LOGGER->debug__(__FILE__, __LINE__,"   write:\n%s", data.replace("\r", "\\r").replace("\n", "\\n\n").c_str());

		writeData(nextFilter, buf);

	} catch (EUnsupportedEncodingException& ex) {
		closeSession("Unable to send HTTP request: ", ex);
	}
}

void EAbstractHttpLogicHandler::reconnect(EIoFilter::NextFilter* nextFilter,
		sp<EHttpProxyRequest>& request) {
	LOGGER->debug(__FILE__, __LINE__,"Reconnecting to proxy ...");

	sp<EProxyIoSession> proxyIoSession = getProxyIoSession();

	// Fires reconnection
	class IoSessionInitializer : public EIoSessionInitializer {
	private:
		EAbstractHttpLogicHandler* handler;
		EIoFilter::NextFilter* nextFilter;
		sp<EHttpProxyRequest> request;
	public:
		IoSessionInitializer(EAbstractHttpLogicHandler* h,
				EIoFilter::NextFilter* n,
				sp<EHttpProxyRequest>& r) : handler(h), nextFilter(n), request(r) {
		}
		virtual void initializeSession(sp<EIoSession> session, sp<EIoFuture> future) {
			EAbstractHttpLogicHandler::LOGGER->debug__(__FILE__, __LINE__,"Initializing new session: %s", session->toString().c_str());
			sp<EProxyIoSession> proxyIoSession = handler->getProxyIoSession();
			session->setAttribute(EProxyIoSession::PROXY_SESSION, proxyIoSession);
			proxyIoSession->setSession(session);
			EAbstractHttpLogicHandler::LOGGER->debug__(__FILE__, __LINE__,"  setting up proxyIoSession: %s", proxyIoSession->toString().c_str());
			class IoFutureListener : public EIoFutureListener {
			private:
				EAbstractHttpLogicHandler* handler;
				EIoFilter::NextFilter* nextFilter;
				sp<EHttpProxyRequest> request;
			public:
				IoFutureListener(EAbstractHttpLogicHandler* h,
						EIoFilter::NextFilter* n,
						sp<EHttpProxyRequest>& r) : handler(h), nextFilter(n), request(r) {
				}
				virtual void operationComplete(EIoFuture* future) {
					// Reconnection is done so we send the
					// request to the proxy
					sp<EProxyIoSession> proxyIoSession = handler->getProxyIoSession();
					proxyIoSession->setReconnectionNeeded(false);
					handler->writeRequest0(nextFilter, request);
				}
			};
			sp<IoFutureListener> ifl(new IoFutureListener(handler, nextFilter, request));
			future->addListener(ifl);
		}
	};
	IoSessionInitializer isi(this, nextFilter, request);
	proxyIoSession->getConnector()->connect(&isi);
}

} /* namespace eio */
} /* namespace efc */
