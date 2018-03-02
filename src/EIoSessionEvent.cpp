/*
 * EIoSessionEvent.cpp
 *
 *  Created on: 2016-6-16
 *      Author: cxxjava@lotuseed.com
 */

#include "../inc/EIoSessionEvent.hh"

namespace efc {
namespace eio {

sp<ELogger> EIoSessionEvent::logger = ELoggerManager::getLogger("EIoSessionEvent");

EIoSessionEvent::EIoSessionEvent(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, EIoSessionEventType::TYPE type) {
	this->nextFilter = nextFilter;
	this->session = session;
	this->type = type;
}

EIoSessionEvent::EIoSessionEvent(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, EIdleStatus status) {
	this->nextFilter = nextFilter;
	this->session = session;
	this->type = EIoSessionEventType::IDLE;
	this->status = status;
}

void EIoSessionEvent::deliverEvent() {
	logger->debug__(__FILE__, __LINE__, "Delivering event {%s}", toString().c_str());
	deliverEvent(this->nextFilter, this->session, this->type, this->status);
}

EString EIoSessionEvent::toString() {
	EString sb("EIoSessionEvent");
	sb.append('@');
	sb.append(EInteger::toHexString(hashCode()));
	sb.append(" - [ ").append(session->toString());
	sb.append(", ").append((int)type);
	sb.append(']');
	return sb;
}

EIdleStatus EIoSessionEvent::getStatus() {
	return status;
}

EIoFilter::NextFilter* EIoSessionEvent::getNextFilter() {
	return nextFilter;
}

sp<EIoSession> EIoSessionEvent::getSession() {
	return session;
}

EIoSessionEventType::TYPE EIoSessionEvent::getType() {
	return type;
}

void EIoSessionEvent::deliverEvent(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session,
			EIoSessionEventType::TYPE type, EIdleStatus status) {
	switch (type) {
	case EIoSessionEventType::CREATED:
		nextFilter->sessionCreated(session);
		break;
	case EIoSessionEventType::OPENED:
		nextFilter->sessionOpened(session);
		break;
	case EIoSessionEventType::IDLE:
		nextFilter->sessionIdle(session, status);
		break;
	case EIoSessionEventType::CLOSED:
		nextFilter->sessionClosed(session);
		break;
	}
}

} /* namespace eio */
} /* namespace efc */
