/*
 * ENioDatagramConnector.cpp
 *
 *  Created on: 2016-8-30
 *      Author: cxxjava@163.com
 */

#include "../inc/ENioDatagramConnector.hh"
#include "../inc/EDefaultDatagramSessionConfig.hh"
#include "../inc/ENioDatagramSession.hh"

namespace efc {
namespace eio {

ENioDatagramConnector::~ENioDatagramConnector() {
	delete ddsc;
}

ENioDatagramConnector::ENioDatagramConnector():
		EAbstractPollingIoConnector<sp<EDatagramChannel> >(ddsc = new EDefaultDatagramSessionConfig()) {
	this->init();
}

ENioDatagramConnector::ENioDatagramConnector(int processorCount) :
		EAbstractPollingIoConnector<sp<EDatagramChannel> >(ddsc = new EDefaultDatagramSessionConfig(), processorCount) {
	this->init();
}

ENioDatagramConnector::ENioDatagramConnector(EIoProcessor* processor):
		EAbstractPollingIoConnector<sp<EDatagramChannel> >(ddsc = new EDefaultDatagramSessionConfig(), processor) {
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

sp<EDatagramChannel> ENioDatagramConnector::newHandle(
		EInetSocketAddress* localAddress) {
	sp<EDatagramChannel> ch = EDatagramChannel::open();

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

bool ENioDatagramConnector::connect(sp<EDatagramChannel> handle,
		EInetSocketAddress* remoteAddress) {
	handle->connect(remoteAddress);
	return true;
}

sp<EIoSession> ENioDatagramConnector::newSession(EIoProcessor* processor,
		sp<EDatagramChannel> handle) {
	sp <EIoSession> session(new ENioDatagramSession(this, processor, handle));
	session->getConfig()->setAll(getSessionConfig());
	return session;
}

void ENioDatagramConnector::close(sp<EDatagramChannel> handle) {
	handle->disconnect();
	handle->close();
}

sp<EAbstractPollingIoConnector<sp<EDatagramChannel> >::ConnectionRequest> ENioDatagramConnector::getConnectionRequest(
		sp<EDatagramChannel> handle) {
	throw EUnsupportedOperationException(__FILE__, __LINE__);
}

bool ENioDatagramConnector::finishConnect(sp<EDatagramChannel> handle) {
	throw EUnsupportedOperationException(__FILE__, __LINE__);
}

void ENioDatagramConnector::register_(sp<EDatagramChannel> handle,
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

sp<EIterator<sp<EDatagramChannel> > > ENioDatagramConnector::selectedHandles() {
	return new EEmptyIterator<sp<EDatagramChannel> >();
}

sp<EIterator<sp<EDatagramChannel> > > ENioDatagramConnector::allHandles() {
	return new EEmptyIterator<sp<EDatagramChannel> >();
}

} /* namespace eio */
} /* namespace efc */
