/*
 * EIoEvent.hh
 *
 *  Created on: 2016-9-22
 *      Author: cxxjava@163.com
 */

#ifndef EIOEVENT_HH_
#define EIOEVENT_HH_

#include "./EIoEventType.hh"
#include "./EIoSession.hh"

namespace efc {
namespace eio {

/**
 * An I/O event or an I/O request that MINA provides.
 * Most users won't need to use this class.  It is usually used by internal
 * components to store I/O events.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EIoEvent: virtual public ERunnable {
public:
	virtual ~EIoEvent();

	EIoEvent(EIoEventType type, sp<EIoSession>& session, sp<EObject>& parameter);
	EIoEventType getType();
	sp<EIoSession> getSession();
	sp<EObject> getParameter();

	virtual void run();
	virtual void fire();
	virtual EString toString();

private:
	EIoEventType type;
	sp<EIoSession> session;
	sp<EObject> parameter;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EIOEVENT_HH_ */
