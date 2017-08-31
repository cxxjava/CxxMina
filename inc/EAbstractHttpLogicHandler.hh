/*
 * EAbstractHttpLogicHandler.hh
 *
 *  Created on: 2016-7-7
 *      Author: cxxjava@163.com
 */

#ifndef EABSTRACTHTTPLOGICHANDLER_HH_
#define EABSTRACTHTTPLOGICHANDLER_HH_

#include "./EAbstractProxyLogicHandler.hh"
#include "./EHttpProxyRequest.hh"
#include "./EHttpProxyResponse.hh"
#include "ELog.hh"

namespace efc {
namespace eio {

/**
 * AbstractHttpLogicHandler.java - Base class for HTTP proxy {@link AbstractProxyLogicHandler} implementations.
 * Provides HTTP request encoding/response decoding functionality.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since MINA 2.0.0-M3
 */

class EAbstractHttpLogicHandler: public EAbstractProxyLogicHandler {
public:
	virtual ~EAbstractHttpLogicHandler();

	/**
	 * Creates a new {@link AbstractHttpLogicHandler}.
	 *
	 * @param proxyIoSession the {@link ProxyIoSession} in use.
	 */
	EAbstractHttpLogicHandler(sp<EProxyIoSession>& proxyIoSession);

	/**
	 * Handles incoming data during the handshake process. Should consume only the
	 * handshake data from the buffer, leaving any extra data in place.
	 *
	 * @param nextFilter the next filter
	 * @param buf the buffer holding received data
	 */
	virtual synchronized void messageReceived(EIoFilter::NextFilter* nextFilter, sp<EIoBuffer>& buf) THROWS(EProxyAuthException);

	/**
	 * Calls writeRequest0(NextFilter, HttpProxyRequest) to write the request.
	 * If needed a reconnection to the proxy is done previously.
	 *
	 * @param nextFilter the next filter
	 * @param request the http request
	 */
	virtual void writeRequest(EIoFilter::NextFilter* nextFilter, sp<EHttpProxyRequest>& request);

public:
	/**
	 * Handles a HTTP response from the proxy server.
	 *
	 * @param response The response.
	 * @throws ProxyAuthException If we get an error during the proxy authentication
	 */
	virtual void handleResponse(sp<EHttpProxyResponse>& response) THROWS(EProxyAuthException) = 0;

protected:
	/**
	 * Parse a HTTP response from the proxy server.
	 *
	 * @param response The response string.
	 * @return The decoded HttpResponse
	 * @throws Exception If we get an error while decoding the response
	 */
	sp<EHttpProxyResponse> decodeResponse(const char* response) THROWS(EException);

private:
	static sp<ELogger> LOGGER;// = LoggerFactory.getLogger(AbstractHttpLogicHandler.class);
	static sp<EString> DECODER;// = AbstractHttpLogicHandler.class.getName() + ".Decoder";

	#define HTTP_DELIMITER "\r\n\r\n"
	#define CRLF_DELIMITER "\r\n"

	// Parsing vars

	/**
	 * Temporary buffer to accumulate the HTTP response from the proxy.
	 */
	sp<EIoBuffer> responseData;// = null;

	/**
	 * The parsed http proxy response
	 */
	sp<EHttpProxyResponse> parsedResponse;// = null;

	/**
	 * The content length of the proxy response.
	 */
	int contentLength;// = -1;

	// HTTP/1.1 vars

	/**
	 * A flag that indicates that this is a HTTP/1.1 response with chunked data.and that some chunks are missing.
	 */
	boolean hasChunkedData;

	/**
	 * A flag that indicates that some chunks of data are missing to complete the HTTP/1.1 response.
	 */
	boolean waitingChunkedData;

	/**
	 * A flag that indicates that chunked data has been read and that we're now reading the footers.
	 */
	boolean waitingFooters;

	/**
	 * Contains the position of the entity body start in the <code>responseData</code> {@link IoBuffer}.
	 */
	int entityBodyStartPosition;

	/**
	 * Contains the limit of the entity body start in the <code>responseData</code> {@link IoBuffer}.
	 */
	int entityBodyLimitPosition;

	/**
	 * Encodes a HTTP request and sends it to the proxy server.
	 *
	 * @param nextFilter the next filter
	 * @param request the http request
	 */
	void writeRequest0(EIoFilter::NextFilter* nextFilter, sp<EHttpProxyRequest>& request);

	/**
	 * Method to reconnect to the proxy when it decides not to maintain the connection
	 * during handshake.
	 *
	 * @param nextFilter the next filter
	 * @param request the http request
	 */
	void reconnect(EIoFilter::NextFilter* nextFilter, sp<EHttpProxyRequest>& request);
};

} /* namespace eio */
} /* namespace efc */
#endif /* EABSTRACTHTTPLOGICHANDLER_HH_ */
