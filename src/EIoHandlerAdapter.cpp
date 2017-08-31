/*
 * EIoHandlerAdapter.cpp
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#include "../inc/EIoHandlerAdapter.hh"

namespace efc {
namespace eio {

sp<ELogger> EIoHandlerAdapter::LOGGER = ELoggerManager::getLogger("EIoHandlerAdapter");

void EIoHandlerAdapter::sessionCreated(sp<EIoSession>& session) {
	// Empty handler
}

void EIoHandlerAdapter::sessionOpened(sp<EIoSession>& session) {
	// Empty handler
}

void EIoHandlerAdapter::sessionClosed(sp<EIoSession>& session) {
	// Empty handler
}

void EIoHandlerAdapter::sessionIdle(sp<EIoSession>& session, EIdleStatus status) {
	// Empty handler
}

void EIoHandlerAdapter::exceptionCaught(sp<EIoSession>& session, sp<EThrowableType>& cause) {
	EThrowable* t = cause->getThrowable();
	if (t) {
		LOGGER->warn_("EXCEPTION, please implement EIoHandlerAdapter.exceptionCaught() for proper handling:%s",
				t->getMessage());
	}
}

void EIoHandlerAdapter::messageReceived(sp<EIoSession>& session, sp<EObject>& message) {
	// Empty handler
}

void EIoHandlerAdapter::messageSent(sp<EIoSession>& session, sp<EObject>& message) {
	// Empty handler
}

void EIoHandlerAdapter::inputClosed(sp<EIoSession>& session) {
	session->closeNow();
}

} /* namespace eio */
} /* namespace efc */
