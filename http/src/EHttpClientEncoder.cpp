/*
 * EHttpClientEncoder.cpp
 *
 *  Created on: 2017-1-4
 *      Author: cxxjava@163.com
 */

#include "../inc/EHttpClientEncoder.hh"
#include "../inc/EHttpRequest.hh"
#include "../inc/EHttpEndOfContent.hh"
#include "../../inc/EIoBuffer.hh"

namespace efc {
namespace eio {

sp<ELogger> EHttpClientEncoder::LOG = ELoggerManager::getLogger("EHttpClientEncoder");

void EHttpClientEncoder::encode(sp<EIoSession>& session, sp<EObject>& message, sp<EProtocolEncoderOutput>& out) {
	LOG->debug_("encode %s", typeid(message.get()).name());
	EHttpRequest* msg = dynamic_cast<EHttpRequest*>(message.get());
	if (msg) {
		LOG->debug("HttpRequest");
		EString sb(msg->getMethod());
		sb.append(" ");
		sb.append(msg->getRequestPath().get());
		sp<EString> qs = msg->getQueryString();
		if (qs != null && !qs->isEmpty()) {
			sb.append("?");
			sb.append(qs.get());
		}
		sb.append(" ");
		sb.append(msg->getProtocolVersion());
		sb.append("\r\n");

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

void EHttpClientEncoder::dispose(sp<EIoSession>& session) THROWS(EException) {
	//
}

} /* namespace eio */
} /* namespace efc */
