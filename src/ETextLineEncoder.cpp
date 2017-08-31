/*
 * ETextLineEncoder.cpp
 *
 *  Created on: 2016-5-24
 *      Author: cxxjava@163.com
 */

#include "../inc/ETextLineEncoder.hh"
#include "../inc/EIoBuffer.hh"

namespace efc {
namespace eio {

ETextLineEncoder::~ETextLineEncoder() {
	eso_mfree(crlf);
}

ETextLineEncoder::ETextLineEncoder(const char* s): crlf(null) {
	crlf = eso_mstrdup(s ? s : "\r\n");
}

void ETextLineEncoder::encode(sp<EIoSession>& session, sp<EObject>& message, sp<EProtocolEncoderOutput>& out) {
	const char* value = (message == null ? "" : message->toString().c_str());
	sp<EIoBuffer> buf = EIoBuffer::allocate(eso_strlen(value) + 2)->setAutoExpand(true);
	buf->putString(value);

	buf->putString(crlf);
	buf->flip();
	out->write(buf);
}

} /* namespace eio */
} /* namespace efc */
