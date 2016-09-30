/*
 * EIoSessionEventQueue.cpp
 *
 *  Created on: 2016-6-16
 *      Author: cxxjava@lotuseed.com
 */

#include "EIoSessionEventQueue.hh"
#include "EIoSessionEventType.hh"
#include "ESocksProxyRequest.hh"
#include "EProxyIoSession.hh"

namespace efc {
namespace eio {

sp<ELogger> EIoSessionEventQueue::logger = ELoggerManager::getLogger("EIoSessionEventQueue");

EIoSessionEventQueue::EIoSessionEventQueue(EProxyIoSession* proxyIoSession) {
	this->proxyIoSession = proxyIoSession;
}

void EIoSessionEventQueue::enqueueEventIfNecessary(EIoSessionEvent* evt) {
	logger->debug__(__FILE__, __LINE__, "??? >> Enqueue {%s}", evt->toString().c_str());

	if (dynamic_pointer_cast<ESocksProxyRequest>(proxyIoSession->getRequest()) != null) {
		// No reconnection used
		evt->deliverEvent();
		return;
	}

	if (proxyIoSession->getHandler()->isHandshakeComplete()) {
		evt->deliverEvent();
	} else {
		if (evt->getType() == EIoSessionEventType::CLOSED) {
			if (proxyIoSession->isAuthenticationFailed()) {
				proxyIoSession->getConnector()->cancelConnectFuture();
				discardSessionQueueEvents();
				evt->deliverEvent();
			} else {
				discardSessionQueueEvents();
			}
		} else if (evt->getType() == EIoSessionEventType::OPENED) {
			// Enqueue event cause it will not reach IoHandler but deliver it to enable
			// session creation.
			enqueueSessionEvent(evt);
			evt->deliverEvent();
		} else {
			enqueueSessionEvent(evt);
		}
	}
}

void EIoSessionEventQueue::flushPendingSessionEvents() {
	SYNCBLOCK(&queueLock) {
		EIoSessionEvent* evt;

		while ((evt = sessionEventsQueue.poll()) != null) {
			logger->debug__(__FILE__, __LINE__, " Flushing buffered event: {%s}", evt->toString().c_str());
			evt->deliverEvent();
			delete evt; //!
		}
    }}
}

void EIoSessionEventQueue::discardSessionQueueEvents() {
	SYNCBLOCK(&queueLock) {
		// Free queue
		sessionEventsQueue.clear();
		logger->debug(__FILE__, __LINE__, "Event queue CLEARED");
    }}
}

void EIoSessionEventQueue::enqueueSessionEvent(EIoSessionEvent* evt) {
	SYNCBLOCK(&queueLock) {
		logger->debug__(__FILE__, __LINE__, "Enqueuing event: {%s}", evt->toString().c_str());
		sessionEventsQueue.offer(evt);
    }}
}

} /* namespace eio */
} /* namespace efc */
