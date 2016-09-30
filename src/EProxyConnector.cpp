/*
 * EProxyConnector.cpp
 *
 *  Created on: 2016-7-5
 *      Author: cxxjava@lotuseed.com
 */

#include "EProxyConnector.hh"
#include "ESocksProxyRequest.hh"
#include "EAbstractProxyIoHandler.hh"
#include "EProxyIoSessionInitializer.hh"
#include "EDefaultSocketSessionConfig.hh"

namespace efc {
namespace eio {

EProxyConnector::~EProxyConnector() {
	delete sessionConfig;
}

EProxyConnector::EProxyConnector() :
		EAbstractIoConnector(sessionConfig = new EDefaultSocketSessionConfig(), null) {
}

EProxyConnector::EProxyConnector(ESocketConnector* connector) :
		EAbstractIoConnector(sessionConfig = new EDefaultSocketSessionConfig(), null) {
	setConnector(connector);
}

EProxyConnector::EProxyConnector(ESocketConnector* connector,
		EIoSessionConfig* config, EExecutorService* executor) :
		EAbstractIoConnector(config, executor), sessionConfig(null) {
	setConnector(connector);
}

EIoSessionConfig* EProxyConnector::getSessionConfig() {
	return connector->getSessionConfig();
}

sp<EProxyIoSession> EProxyConnector::getProxyIoSession() {
	return proxyIoSession;
}

void EProxyConnector::setProxyIoSession(sp<EProxyIoSession>& proxyIoSession) {
	if (proxyIoSession->getProxyAddress() == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "proxySession.proxyAddress cannot be null");
	}

	proxyIoSession->setConnector(this);
	setDefaultRemoteAddress(proxyIoSession->getProxyAddress());
	this->proxyIoSession = proxyIoSession;
}

void EProxyConnector::cancelConnectFuture() {
	future->cancel();
}

sp<EConnectFuture> EProxyConnector::connect0(EInetSocketAddress* remoteAddress,
		EInetSocketAddress* localAddress,
		EIoSessionInitializer* sessionInitializer) {
	if (!proxyIoSession->isReconnectionNeeded()) {
		// First connection
		EIoHandler* handler = getHandler();
		if (!(instanceof<EAbstractProxyIoHandler>(handler))) {
			throw EIllegalArgumentException(__FILE__, __LINE__, "IoHandler must be an instance of AbstractProxyIoHandler");
		}

		connector->setHandler(handler);
		future = new EDefaultConnectFuture();
	}

	sp<EConnectFuture> conFuture = connector->connect(proxyIoSession->getProxyAddress(), new EProxyIoSessionInitializer(
			sessionInitializer, proxyIoSession));

	// If proxy does not use reconnection like socks the connector's
	// future is returned. If we're in the middle of a reconnection
	// then we send back the connector's future which is only used
	// internally while <code>future</code> will be used to notify
	// the user of the connection state.
	sp<EProxyRequest> pr = proxyIoSession->getRequest();
	if (dynamic_pointer_cast<ESocksProxyRequest>(pr) != null || proxyIoSession->isReconnectionNeeded()) {
		return conFuture;
	}

	return future;
}

sp<EConnectFuture> EProxyConnector::fireConnected(sp<EIoSession>& session) {
	future->setSession(session);
	return future;
}

ESocketConnector* EProxyConnector::getConnector() {
	return connector;
}

void EProxyConnector::dispose0() {
	if (connector != null) {
		connector->dispose();
	}
}

void EProxyConnector::setConnector(ESocketConnector* connector) {
	if (connector == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "connector cannot be null");
	}

	this->connector = connector;
	const char* className = "efc::eio::EProxyFilter";

	// Removes an old ProxyFilter instance from the chain
	if (connector->getFilterChain()->contains(className)) {
		connector->getFilterChain()->remove(className);
	}

	// Insert the ProxyFilter as the first filter in the filter chain builder
	connector->getFilterChain()->addFirst(className, proxyFilter);
}

} /* namespace eio */
} /* namespace efc */
