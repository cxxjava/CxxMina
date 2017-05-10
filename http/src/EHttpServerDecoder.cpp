/*
 * EHttpServerDecoder.cpp
 *
 *  Created on: 2017-1-4
 *      Author: cxxjava@163.com
 */

#include "EHttpServerDecoder.hh"
#include "EHttpEndOfContent.hh"
#include "EDecoderState.hh"
#include "EHttpStatus.hh"
#include "EHttpException.hh"

namespace efc {
namespace eio {

sp<ELogger> EHttpServerDecoder::LOG = ELoggerManager::getLogger("EHttpServerDecoder");

sp<EString> EHttpServerDecoder::DECODER_STATE_ATT(new EString("http.ds"));
sp<EString> EHttpServerDecoder::PARTIAL_HEAD_ATT(new EString("http.ph"));
sp<EString> EHttpServerDecoder::BODY_REMAINING_BYTES(new EString("http.brb"));

void EHttpServerDecoder::decode(sp<EIoSession>& session, EIoBuffer* msg, sp<EProtocolDecoderOutput>& out) {
	sp<EInteger> state = dynamic_pointer_cast<EInteger>(session->getAttribute(DECODER_STATE_ATT.get()));
	if (state == null) {
		state = new EInteger(EDecoderState::NEW);
		session->setAttribute(DECODER_STATE_ATT, state);
	}

	sp<EIoBuffer> newmsg;

	switch (state->intValue()) {
	case EDecoderState::HEAD:
	{
		LOG->debug("decoding HEAD");
		// grab the stored a partial HEAD request
		sp<EIoBuffer> oldBuffer = dynamic_pointer_cast<EIoBuffer>(session->getAttribute(PARTIAL_HEAD_ATT.get()));
		// concat the old buffer and the new incoming one
		newmsg = EIoBuffer::allocate(oldBuffer->remaining() + msg->remaining())->put(oldBuffer.get())->put(msg)->flip();
		// now let's decode like it was a new message
		msg = newmsg.get();
	}
	case EDecoderState::NEW:
	{
		LOG->debug("decoding NEW");
		sp<EHttpRequestImpl> rq = parseHttpRequestHead(msg->buf());

		if (rq == null) {
			// we copy the incoming BB because it's going to be recycled by the inner IoProcessor for next reads
			sp<EIoBuffer> partial = EIoBuffer::allocate(msg->remaining());
			partial->put(msg->buf());
			partial->flip();
			// no request decoded, we accumulate
			session->setAttribute(PARTIAL_HEAD_ATT, partial);
			session->setAttribute(DECODER_STATE_ATT, new EInteger(EDecoderState::HEAD));
			break;
		} else {
			out->write(rq);
			// is it a request with some body content ?
			sp<EString> contentLen = rq->getHeader("content-length");

			if (contentLen != null) {
				LOG->debug_("found content len : %s", contentLen->c_str());
				session->setAttribute(BODY_REMAINING_BYTES, new EInteger(atol(contentLen->c_str())));
				session->setAttribute(DECODER_STATE_ATT, new EInteger(EDecoderState::BODY));
				// fallthrough, process body immediately
			} else {
				LOG->debug("request without content");
				session->setAttribute(DECODER_STATE_ATT, new EInteger(EDecoderState::NEW));
				out->write(new EHttpEndOfContent());
				break;
			}
		}
	}
	case EDecoderState::BODY:
	{
		LOG->debug_("decoding BODY: %d bytes", msg->remaining());
		int chunkSize = msg->remaining();
		// send the chunk of body
		if (chunkSize != 0) {
			sp<EIoBuffer> wb = EIoBuffer::allocate(msg->remaining());
			wb->put(msg);
			wb->flip();
			out->write(wb);
		}
		msg->position(msg->limit());
		// do we have reach end of body ?
		int remaining = dynamic_pointer_cast<EInteger>(session->getAttribute(BODY_REMAINING_BYTES.get()))->intValue();
		remaining -= chunkSize;

		if (remaining <= 0) {
			LOG->debug("end of HTTP body");
			session->setAttribute(DECODER_STATE_ATT, new EInteger(EDecoderState::NEW));
			session->removeAttribute(BODY_REMAINING_BYTES.get());
			out->write(new EHttpEndOfContent());
		} else {
			session->setAttribute(BODY_REMAINING_BYTES, new EInteger(remaining));
		}

		break;
	}
	default:
	{
		EString s("Unknonwn decoder state : "); s << state->intValue();
		throw EHttpException(__FILE__, __LINE__, EHttpStatus::CLIENT_ERROR_BAD_REQUEST.code(),  s.c_str());
	}
	}
}

sp<EHttpRequestImpl> EHttpServerDecoder::parseHttpRequestHead(EIOByteBuffer* buffer) {
	char* rnrn = eso_memstr((char*)buffer->address(), buffer->limit(), RAW_VALUE_PATTERN);
	int index_rnrn = rnrn ? rnrn - (char*)buffer->address() : -1;
	if (index_rnrn < 0) {
		// we didn't receive the full HTTP head
		return null;
	}

	EString headers((char*)buffer->address(), 0, index_rnrn);
	EArray<EString*> headerFields = EPattern::split(HEADERS_BODY_PATTERN, headers.c_str());

	EString* requestLine = headerFields[0];
	sp<EMap<EString*, EString*> > generalHeaders = new EHashMap<EString*, EString*>();

	for (int i = 1; i < headerFields.length(); i++) {
		int index_colon = headerFields[i]->indexOf(HEADER_VALUE_PATTERN);
		if (index_colon >= 0) {
			EString* key = new EString(headerFields[i]->c_str(), 0, index_colon);
			key->toLowerCase();
			EString* val = new EString(headerFields[i]->c_str() + index_colon+1);
			val->trim();
			generalHeaders->put(key, val);
		} else {
			EString* key = new EString(headerFields[i]);
			key->toLowerCase();
			generalHeaders->put(key, new EString());
		}
	}

	EArray<EString*> elements = EPattern::split(REQUEST_LINE_PATTERN, requestLine->c_str());
	EString* method = elements[0];
	EString* version = elements[2];
	EArray<EString*> pathFrags = EPattern::split(QUERY_STRING_PATTERN, elements[1]->c_str());
	EString* requestedPath = pathFrags[0];
	EString* queryString = pathFrags.length() == 2 ? pathFrags[1] : null;

	// we put the buffer position where we found the beginning of the HTTP body
	buffer->position(index_rnrn + 4);

	return new EHttpRequestImpl(version, method, requestedPath, queryString, generalHeaders);
}

void EHttpServerDecoder::finishDecode(sp<EIoSession>& session, sp<EProtocolDecoderOutput>& out) {
	// Do nothing
}

void EHttpServerDecoder::dispose(sp<EIoSession>& session) THROWS(EException) {
	// Do nothing
}

} /* namespace eio */
} /* namespace efc */
