/*
 * EIoUtil.cpp
 *
 *  Created on: 2016-3-5
 *      Author: cxxjava@163.com
 */

#include "EIoUtil.hh"
#include "EIoBuffer.hh"

namespace efc {
namespace eio {

eal<EWriteFuture> EIoUtil::broadcast(sp<EObject>& message, sp<EConcurrentCollection<EIoSession> > sessions) {
	eal<EWriteFuture> answer;

	sp<EConcurrentIterator<EIoSession> > iter = sessions->iterator();
	EIoBuffer* buf = dynamic_cast<EIoBuffer*>(message.get());
	if (buf) {
		while (iter->hasNext()) {
			sp<EIoSession> s = iter->next();
			answer.add(s->write(buf->duplicate()));
		}
	} else {
		while (iter->hasNext()) {
			sp<EIoSession> s = iter->next();
			answer.add(s->write(message));
		}
	}

	return answer;
}

} /* namespace eio */
} /* namespace efc */
