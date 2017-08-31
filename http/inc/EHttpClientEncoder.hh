/*
 * EHttpClientEncoder.hh
 *
 *  Created on: 2017-1-4
 *      Author: cxxjava@163.com
 */

#ifndef EHTTPCLIENTENCODER_HH_
#define EHTTPCLIENTENCODER_HH_

#include "../../inc/EProtocolEncoder.hh"
#include "ELog.hh"

namespace efc {
namespace eio {

/**
 *
 */

class EHttpClientEncoder: virtual public EProtocolEncoder {
public:
	virtual void encode(sp<EIoSession>& session, sp<EObject>& message, sp<EProtocolEncoderOutput>& out) THROWS(EException);
	virtual void dispose(sp<EIoSession>& session) THROWS(EException);

private:
	static sp<ELogger> LOG;// = LoggerFactory.getLogger(HttpServerCodec.class);
};

} /* namespace eio */
} /* namespace efc */
#endif /* EHTTPCLIENTENCODER_HH_ */
