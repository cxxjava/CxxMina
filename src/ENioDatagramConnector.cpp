/*
 * ENioDatagramConnector.cpp
 *
 *  Created on: 2016-8-30
 *      Author: cxxjava@163.com
 */

#include "ENioDatagramConnector.hh"
#include "EDefaultDatagramSessionConfig.hh"
#include "ENioDatagramSession.hh"

namespace efc {
namespace eio {

ENioDatagramConnector::~ENioDatagramConnector() {
	delete ddsc;
}

ENioDatagramConnector::ENioDatagramConnector():
		EAbstractPollingIoConnector<EDatagramChannel*>(ddsc = new EDefaultDatagramSessionConfig()) {
	this->init();
}

ENioDatagramConnector::ENioDatagramConnector(int processorCount) :
		EAbstractPollingIoConnector<EDatagramChannel*>(ddsc = new EDefaultDatagramSessionConfig(), processorCount) {
	this->init();
}

ENioDatagramConnector::ENioDatagramConnector(EIoProcessor* processor):
		EAbstractPollingIoConnector<EDatagramChannel*>(ddsc = new EDefaultDatagramSessionConfig(), processor) {
	this->init();
}

void ENioDatagramConnector::init() {
	// Do nothing
}

void ENioDatagramConnector::destroy() {
	// Do nothing
}

EDatagramSessionConfig* ENioDatagramConnector::getSessionConfig() {
	return dynamic_cast<EDatagramSessionConfig*>(sessionConfig);
}

ETransportMetadata* ENioDatagramConnector::getTransportMetadata() {
	return ENioDatagramSession::METADATA();
}

EInetSocketAddress* ENioDatagramConnector::getDefaultRemoteAddress() {
	return EAbstractIoConnector::getDefaultRemoteAddress();
}

void ENioDatagramConnector::setDefaultRemoteAddress(
		EInetSocketAddress* defaultRemoteAddress) {
	EAbstractIoConnector::setDefaultRemoteAddress(defaultRemoteAddress);
}

EDatagramChannel* ENioDatagramConnector::newHandle(
		EInetSocketAddress* localAddress) {
	EDatagramChannel* ch = EDatagramChannel::open();

	try {
		if (localAddress != null) {
			try {
				ch->socket()->bind(localAddress);
				setDefaultLocalAddress(localAddress);
			} catch (EIOException& ioe) {
				// Add some info regarding the address we try to bind to the
				// message
				EString newMessage("Error while binding on ");
				newMessage << localAddress->toString() << "\n" << "original message : "
						<< ioe.getMessage();
				EIOException e(__FILE__, __LINE__, newMessage.c_str());
				e.initCause(ioe.getCause());

				// and close the channel
				ch->close();

				throw e;
			}
		}

		return ch;
	} catch (EException& e) {
		// If we got an exception while binding the datagram,
		// we have to close it otherwise we will loose an handle
		ch->close();
		throw e;
	}
}

bool ENioDatagramConnector::connect(EDatagramChannel* handle,
		EInetSocketAddress* remoteAddress) {
	handle->connect(remoteAddress);
	return true;
}

sp<EIoSession> ENioDatagramConnector::newSession(EIoProcessor* processor,
		EDatagramChannel* handle) {
	sp < EIoSession > session(new ENioDatagramSession(this, processor, handle));
	session->getConfig()->setAll(getSessionConfig());
	return session;
}

void ENioDatagramConnector::close(EDatagramChannel* handle) {
	handle->disconnect();
	handle->close();
}

sp<EAbstractPollingIoConnector<EDatagramChannel*>::ConnectionRequest> ENioDatagramConnector::getConnectionRequest(
		EDatagramChannel* handle) {
	throw EUnsupportedOperationException(__FILE__, __LINE__);
}

bool ENioDatagramConnector::finishConnect(EDatagramChannel* handle) {
	throw EUnsupportedOperationException(__FILE__, __LINE__);
}

void ENioDatagramConnector::register_(EDatagramChannel* handle,
		sp<ConnectionRequest> request) {
	throw EUnsupportedOperationException(__FILE__, __LINE__);
}

int ENioDatagramConnector::select(int timeout) {
	return 0;
}

void ENioDatagramConnector::wakeup() {
	// Do nothing
}

sp<EConnectFuture> ENioDatagramConnector::connect() {
	return EAbstractIoConnector::connect();
}

sp<EConnectFuture> ENioDatagramConnector::connect(
		EIoSessionInitializer* sessionInitializer) {
	return EAbstractIoConnector::connect(sessionInitializer);
}

sp<EConnectFuture> ENioDatagramConnector::connect(
		EInetSocketAddress* remoteAddress) {
	return EAbstractIoConnector::connect(remoteAddress);
}

sp<EConnectFuture> ENioDatagramConnector::connect(
		EInetSocketAddress* remoteAddress,
		EIoSessionInitializer* sessionInitializer) {
	return EAbstractIoConnector::connect(remoteAddress, sessionInitializer);
}

sp<EConnectFuture> ENioDatagramConnector::connect(
		EInetSocketAddress* remoteAddress, EInetSocketAddress* localAddress) {
	return EAbstractIoConnector::connect(remoteAddress, localAddress);
}

sp<EConnectFuture> ENioDatagramConnector::connect(
		EInetSocketAddress* remoteAddress, EInetSocketAddress* localAddress,
		EIoSessionInitializer* sessionInitializer) {
	return EAbstractIoConnector::connect(remoteAddress, localAddress, sessionInitializer);
}

sp<EIterator<EDatagramChannel*> > ENioDatagramConnector::selectedHandles() {
	return new EEmptyIterator<EDatagramChannel*>();
}

sp<EIterator<EDatagramChannel*> > ENioDatagramConnector::allHandles() {
	return new EEmptyIterator<EDatagramChannel*>();
}

} /* namespace eio */
} /* namespace efc */
