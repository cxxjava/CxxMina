/*
 * EHttpServerCodec.hh
 *
 *  Created on: 2017-1-4
 *      Author: cxxjava@163.com
 */

#ifndef EHTTPSERVERCODEC_HH_
#define EHTTPSERVERCODEC_HH_

#include "../../inc/EProtocolCodecFilter.hh"

namespace efc {
namespace eio {

/**
 *
 */

class EHttpServerCodec: public EProtocolCodecFilter {
public:
	virtual ~EHttpServerCodec(){}

	EHttpServerCodec(): EProtocolCodecFilter(encoder, decoder) {
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

	static EProtocolEncoder* encoder;// = new HttpServerEncoder();
	static EProtocolDecoder* decoder;// = new HttpServerDecoder();
};

} /* namespace eio */
} /* namespace efc */
#endif /* EHTTPSERVERCODEC_HH_ */
