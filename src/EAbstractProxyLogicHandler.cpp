/*
 * EAbstractProxyLogicHandler.cpp
 *
 *  Created on: 2016-7-6
 *      Author: cxxjava@lotuseed.com
 */

#include "EAbstractProxyLogicHandler.hh"
#include "EProxyHandshakeIoBuffer.hh"
#include "EDefaultWriteFuture.hh"
#include "EDefaultWriteRequest.hh"

namespace efc {
namespace eio {

sp<ELogger> EAbstractProxyLogicHandler::LOGGER = ELoggerManager::getLogger("EAbstractProxyLogicHandler");

EAbstractProxyLogicHandler::~EAbstractProxyLogicHandler() {
	delete writeRequestQueue;
}

EAbstractProxyLogicHandler::EAbstractProxyLogicHandler(
		sp<EProxyIoSession>& proxyIoSession) :
		writeRequestQueue(null), handshakeComplete(false) {
	this->proxyIoSession = proxyIoSession;
}

sp<EProxyIoSession> EAbstractProxyLogicHandler::getProxyIoSession() {
	return proxyIoSession;
}

boolean EAbstractProxyLogicHandler::isHandshakeComplete() {
	SYNCHRONIZED (this) {
		return handshakeComplete;
    }}
}

void EAbstractProxyLogicHandler::enqueueWriteRequest(
		EIoFilter::NextFilter* nextFilter, sp<EWriteRequest>& writeRequest) {
	if (writeRequestQueue == null) {
		writeRequestQueue = new ELinkedList<Event*>();
	}

	sp<EObject> data = dynamic_pointer_cast<EObject>(writeRequest);
	writeRequestQueue->offer(new Event(nextFilter, data));
}

EProxyFilter* EAbstractProxyLogicHandler::getProxyFilter() {
	return proxyIoSession->getProxyFilter();
}

sp<EIoSession> EAbstractProxyLogicHandler::getSession() {
	return proxyIoSession->getSession();
}

sp<EWriteFuture> EAbstractProxyLogicHandler::writeData(
		EIoFilter::NextFilter* nextFilter, sp<EIoBuffer>& data) {
	// write net data
	sp<EProxyHandshakeIoBuffer> writeBuffer = new EProxyHandshakeIoBuffer(data.dismiss()); //!!!

	LOGGER->debug__(__FILE__, __LINE__, "   session write: %s", writeBuffer->toString().c_str());

	sp<EWriteFuture> writeFuture = new EDefaultWriteFuture(getSession());
	sp<EWriteRequest> writeRequest = new EDefaultWriteRequest(writeBuffer, writeFuture);
	sp<EIoSession> session = getSession();
	getProxyFilter()->writeData(nextFilter, session, writeRequest, true);

	return writeFuture;
}

void EAbstractProxyLogicHandler::setHandshakeComplete() {
	SYNCHRONIZED(this) {
		handshakeComplete = true;
    }}

	sp<EProxyIoSession> proxyIoSession = getProxyIoSession();
    sp<EIoSession> session = proxyIoSession->getSession();
	proxyIoSession->getConnector()->fireConnected(session)->awaitUninterruptibly();

	LOGGER->debug(__FILE__, __LINE__, "  handshake completed");

	// Connected OK
	try {
		proxyIoSession->getEventQueue()->flushPendingSessionEvents();
		flushPendingWriteRequests();
	} catch (EException& ex) {
		LOGGER->error(__FILE__, __LINE__, "Unable to flush pending write requests", ex);
	}
}

void EAbstractProxyLogicHandler::flushPendingWriteRequests() {
	LOGGER->debug(__FILE__, __LINE__, " flushPendingWriteRequests()");

	if (writeRequestQueue == null) {
		return;
	}

	Event* scheduledWrite;
	while ((scheduledWrite = writeRequestQueue->poll()) != null) {
		LOGGER->debug__(__FILE__, __LINE__, " Flushing buffered write request: %s", scheduledWrite->data->toString().c_str());

		sp<EIoSession> session = getSession();
		sp<EWriteRequest> writeRequest = dynamic_pointer_cast<EWriteRequest>(scheduledWrite->data);
		getProxyFilter()->filterWrite(scheduledWrite->nextFilter, session, writeRequest);
	}

	// Free queue
	writeRequestQueue = null;
}

void EAbstractProxyLogicHandler::closeSession(const char* message,
		EThrowable& t) {
	LOGGER->error(__FILE__, __LINE__, message, t);
	proxyIoSession->setAuthenticationFailed(true);
	getSession()->closeNow();
}

void EAbstractProxyLogicHandler::closeSession(const char* message) {
	LOGGER->error(__FILE__, __LINE__, message);
	getSession()->closeNow();
}

} /* namespace eio */
} /* namespace efc */
