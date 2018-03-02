/*
 * EIoUtil.cpp
 *
 *  Created on: 2016-3-5
 *      Author: cxxjava@163.com
 */

#include "./EIoUtil.hh"
#include "../inc/EIoBuffer.hh"

namespace efc {
namespace eio {

EArrayList<sp<EWriteFuture> > EIoUtil::broadcast(sp<EObject>& message, sp<ECollection<sp<EIoSession> > > sessions) {
	EArrayList<sp<EWriteFuture> > answer;

	sp<EIterator<sp<EIoSession> > > iter = sessions->iterator();
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
