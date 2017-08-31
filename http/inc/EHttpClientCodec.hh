/*
 * EHttpClientCodec.hh
 *
 *  Created on: 2017-1-4
 *      Author: cxxjava@163.com
 */

#ifndef EHTTPCLIENTCODEC_HH_
#define EHTTPCLIENTCODEC_HH_

#include "../../inc/EProtocolCodecFilter.hh"

namespace efc {
namespace eio {

/**
 *
 */

class EHttpClientCodec: public EProtocolCodecFilter {
public:
	virtual ~EHttpClientCodec(){}

	EHttpClientCodec(): EProtocolCodecFilter(encoder, decoder) {
	}

	/**
	 * {@inheritDoc}
	 */
	virtual void sessionClosed(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session) THROWS(EException) {
		EProtocolCodecFilter::sessionClosed(nextFilter, session);
		session->removeAttribute(DECODER_STATE_ATT.get());
		session->removeAttribute(PARTIAL_HEAD_ATT.get());
	}

private:
	/** Key for decoder current state */
	static sp<EString> DECODER_STATE_ATT;// = "http.ds";

	/** Key for the partial HTTP requests head */
	static sp<EString> PARTIAL_HEAD_ATT;// = "http.ph";

	static EProtocolEncoder* encoder;// = new HttpClientEncoder();
	static EProtocolDecoder* decoder;// = new HttpClientDecoder();
};

} /* namespace eio */
} /* namespace efc */
#endif /* EHTTPCLIENTCODEC_HH_ */
