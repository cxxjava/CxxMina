/*
 * ETextLineEncoder.hh
 *
 *  Created on: 2016-5-24
 *      Author: cxxjava@163.com
 */

#ifndef ETEXTLINEENCODER_HH_
#define ETEXTLINEENCODER_HH_

#include "./EProtocolEncoderAdapter.hh"

namespace efc {
namespace eio {

class ETextLineEncoder: public EProtocolEncoderAdapter {
public:
	virtual ~ETextLineEncoder();
	ETextLineEncoder(const char* crlf);

	/**
	 * {@inheritDoc}
	 */
	virtual void encode(sp<EIoSession>& session, sp<EObject>& message, sp<EProtocolEncoderOutput>& out) THROWS(EException);

private:
	es_string_t* crlf;
};

} /* namespace eio */
} /* namespace efc */
#endif /* ETEXTLINEENCODER_HH_ */
