/*
 * EIoEvent.cpp
 *
 *  Created on: 2016-9-22
 *      Author: cxxjava@163.com
 */

#include "../inc/EIoEvent.hh"
#include "../inc/EIoFilterChain.hh"

namespace efc {
namespace eio {

EIoEvent::~EIoEvent() {

}

EIoEvent::EIoEvent(EIoEventType type, sp<EIoSession>& session,
		sp<EObject>& parameter) {
	if (session == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "session");
	}
	this->type = type;
	this->session = session;
	this->parameter = parameter;
}

EIoEventType EIoEvent::getType() {
	return type;
}

sp<EIoSession> EIoEvent::getSession() {
	return session;
}

sp<EObject> EIoEvent::getParameter() {
	return parameter;
}

void EIoEvent::run() {
	fire();
}

void EIoEvent::fire() {
	sp<EObject> obj = getParameter();

	switch (getType()) {
	case MESSAGE_RECEIVED:
		getSession()->getFilterChain()->fireMessageReceived(obj);
		break;
	case MESSAGE_SENT:
	{
		sp<EWriteRequest> request = dynamic_pointer_cast<EWriteRequest>(obj);
		getSession()->getFilterChain()->fireMessageSent(request);
		break;
	}
	case WRITE:
	{
		sp<EWriteRequest> request = dynamic_pointer_cast<EWriteRequest>(obj);
		getSession()->getFilterChain()->fireFilterWrite(request);
		break;
	}
	case CLOSE:
		getSession()->getFilterChain()->fireFilterClose();
		break;
	case EXCEPTION_CAUGHT:
		getSession()->getFilterChain()->fireExceptionCaught(dynamic_pointer_cast<EThrowableType>(obj));
		break;
	case SESSION_IDLE:
	{
		EIdleStatus stat = (EIdleStatus)dynamic_cast<EInteger*>(obj.get())->intValue();
		getSession()->getFilterChain()->fireSessionIdle(stat);
		break;
	}
	case SESSION_OPENED:
		getSession()->getFilterChain()->fireSessionOpened();
		break;
	case SESSION_CREATED:
		getSession()->getFilterChain()->fireSessionCreated();
		break;
	case SESSION_CLOSED:
		getSession()->getFilterChain()->fireSessionClosed();
		break;
	default:
	{
		EString msg("Unknown event type: "); msg << getType();
		throw EIllegalArgumentException(__FILE__, __LINE__, msg.c_str());
	}
	}
}

EStringBase EIoEvent::toString() {
	if (parameter == null) {
		return EStringBase() + "[" + getSession()->toString() + "] " + GetIoEventTypeName(getType());
	}

	return EStringBase() + "[" + getSession()->toString() + "] " + GetIoEventTypeName(getType()) + ": " + getParameter()->toString();
}

} /* namespace eio */
} /* namespace efc */
