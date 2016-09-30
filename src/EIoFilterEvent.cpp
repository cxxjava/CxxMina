/*
 * EIoFilterEvent.cpp
 *
 *  Created on: 2016-9-23
 *      Author: cxxjava@163.com
 */

#include "EIoFilterEvent.hh"

namespace efc {
namespace eio {

sp<ELogger> EIoFilterEvent::LOGGER = ELoggerManager::getLogger("EIoFilterEvent");

EIoFilterEvent::~EIoFilterEvent() {
	//
}

EIoFilterEvent::EIoFilterEvent(EIoFilter::NextFilter* nextFilter,
		EIoEventType type, sp<EIoSession>& session, sp<EObject> parameter):
		EIoEvent(type, session, parameter) {
	if (nextFilter == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "nextFilter must not be null");
	}

	this->nextFilter = nextFilter;
}

EIoFilter::NextFilter* EIoFilterEvent::getNextFilter() {
	return nextFilter;
}

void EIoFilterEvent::fire() {
	sp<EIoSession> session = getSession();
	EIoEventType type = getType();

	LOGGER->debug__(__FILE__, __LINE__, "Firing a %s event for session %d", GetIoEventTypeName(type), session->getId());

	switch (type) {
	case MESSAGE_RECEIVED:
	{
		sp<EObject> parameter = getParameter();
		nextFilter->messageReceived(session, parameter);
		break;
	}
	case MESSAGE_SENT:
	{
		sp<EWriteRequest> writeRequest = dynamic_pointer_cast<EWriteRequest>(getParameter());
		nextFilter->messageSent(session, writeRequest);
		break;
	}
	case WRITE:
	{
		sp<EWriteRequest> writeRequest = dynamic_pointer_cast<EWriteRequest>(getParameter());
		nextFilter->filterWrite(session, writeRequest);
		break;
	}
	case CLOSE:
		nextFilter->filterClose(session);
		break;

	case EXCEPTION_CAUGHT:
	{
		sp<EThrowableType> throwable = dynamic_pointer_cast<EThrowableType>(getParameter());
		nextFilter->exceptionCaught(session, throwable);
		break;
	}
	case SESSION_IDLE:
	{
		sp<EInteger> parameter = dynamic_pointer_cast<EInteger>(getParameter());
		nextFilter->sessionIdle(session, (EIdleStatus)parameter->intValue());
		break;
	}
	case SESSION_OPENED:
		nextFilter->sessionOpened(session);
		break;

	case SESSION_CREATED:
		nextFilter->sessionCreated(session);
		break;

	case SESSION_CLOSED:
		nextFilter->sessionClosed(session);
		break;

	default:
	{
		EString msg("Unknown event type: "); msg << type;
		throw EIllegalArgumentException(__FILE__, __LINE__, msg.c_str());
	}
	}

	LOGGER->debug__(__FILE__, __LINE__, "Event %s has been fired for session  %d", GetIoEventTypeName(type), session->getId());
}

} /* namespace eio */
} /* namespace efc */
