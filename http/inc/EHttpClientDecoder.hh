/*
 * EHttpClientDecoder.hh
 *
 *  Created on: 2017-1-4
 *      Author: cxxjava@163.com
 */

#ifndef EHTTPCLIENTDECODER_HH_
#define EHTTPCLIENTDECODER_HH_

#include "../../inc/EProtocolDecoder.hh"
#include "./EHttpResponseImpl.hh"
#include "ELog.hh"

namespace efc {
namespace eio {

/**
 *
 */

class EHttpClientDecoder: virtual public EProtocolDecoder {
public:
    /** Regex to parse HttpRequest Request Line */
	#define REQUEST_LINE_PATTERN " "

	/** Regex to parse HttpRequest Request Line */
	#define RESPONSE_LINE_PATTERN " "

    /** Regex to parse out QueryString from HttpRequest */
	#define QUERY_STRING_PATTERN "\\?"

    /** Regex to parse out parameters from query string */
	#define PARAM_STRING_PATTERN "\\&|;"

    /** Regex to parse out key/value pairs */
	#define KEY_VALUE_PATTERN "="

    /** Regex to parse raw headers and body */
	#define RAW_VALUE_PATTERN "\r\n\r\n"

    /** Regex to parse raw headers from body */
	#define HEADERS_BODY_PATTERN "\r\n"

    /** Regex to parse header name and value */
	#define HEADER_VALUE_PATTERN ":"

    /** Regex to split cookie header following RFC6265 Section 5.4 */
	#define COOKIE_SEPARATOR_PATTERN ";"

public:
	virtual void decode(sp<EIoSession>& session, EIoBuffer* in, sp<EProtocolDecoderOutput>& out) THROWS(EException);

	/**
	 * Override this method to deal with the closed connection.
	 * The default implementation does nothing.
	 */
	virtual void finishDecode(sp<EIoSession>& session, sp<EProtocolDecoderOutput>& out) THROWS(EException);

	/**
	 * Override this method to dispose all resources related with this decoder.
	 * The default implementation does nothing.
	 */
	virtual void dispose(sp<EIoSession>& session) THROWS(EException);

private:
	static sp<ELogger> LOG;// = LoggerFactory.getLogger(HttpServerCodec.class);

	/** Key for decoder current state */
	static sp<EString> DECODER_STATE_ATT;// = "http.ds";

	/** Key for the partial HTTP requests head */
	static sp<EString> PARTIAL_HEAD_ATT;// = "http.ph";

	/** Key for the number of bytes remaining to read for completing the body */
	static sp<EString> BODY_REMAINING_BYTES;// = "http.brb";

	/** Key for indicating chunked data */
	static sp<EString> BODY_CHUNKED;// = "http.ckd";

	sp<EHttpResponseImpl> parseHttpReponseHead(EIOByteBuffer* buffer);
};

} /* namespace eio */
} /* namespace efc */
#endif /* EHTTPCLIENTDECODER_HH_ */
