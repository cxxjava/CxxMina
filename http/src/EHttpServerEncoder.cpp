/*
 * EHttpServerEncoder.cpp
 *
 *  Created on: 2017-1-4
 *      Author: cxxjava@163.com
 */

#include "EHttpServerEncoder.hh"
#include "EHttpResponse.hh"
#include "EHttpEndOfContent.hh"
#include "EIoBuffer.hh"

namespace efc {
namespace eio {

sp<ELogger> EHttpServerEncoder::LOG = ELoggerManager::getLogger("EHttpServerEncoder");

void EHttpServerEncoder::encode(sp<EIoSession>& session, sp<EObject>& message, sp<EProtocolEncoderOutput>& out) {
	LOG->debug_("encode %s", typeid(message.get()).name());
	EHttpResponse* msg = dynamic_cast<EHttpResponse*>(message.get());
	if (msg) {
		LOG->debug("HttpResponse");
		EString sb(EHttpStatus::valueOf(msg->getStatus())->line());

		sp<EIterator<EMapEntry<EString*,EString*>*> > iter = msg->getHeaders()->entrySet()->iterator();
		while (iter->hasNext()) {
			EMapEntry<EString*,EString*>* header = iter->next();

			sb.append(header->getKey());
			sb.append(": ");
			sb.append(header->getValue());
			sb.append("\r\n");
		}
		sb.append("\r\n");
		sp<EIoBuffer> buf = EIoBuffer::allocate(sb.length())->setAutoExpand(true);
		buf->putString(sb.toString().c_str()/*, ENCODER*/);
		buf->flip();
		out->write(buf);
	} else if (instanceof<EIoBuffer>(message.get())) {
		LOG->debug_("Body %s", message->toString().c_str());
		out->write(message.get());
	} else if (instanceof<EHttpEndOfContent>(message.get())) {
		LOG->debug("End of Content");
		// end of HTTP content
		// keep alive ?
	}
}

void EHttpServerEncoder::dispose(sp<EIoSession>& session) THROWS(EException) {
	//
}

} /* namespace eio */
} /* namespace efc */
