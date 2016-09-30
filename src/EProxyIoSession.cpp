/*
 * EProxyIoSession.cpp
 *
 *  Created on: 2016-6-16
 *      Author: cxxjava@lotuseed.com
 */

#include "EProxyIoSession.hh"

namespace efc {
namespace eio {

sp<EString> EProxyIoSession::PROXY_SESSION = new EString("ProxyConnector.ProxySession");

DEFINE_STATIC_INITZZ_BEGIN(EProxyIoSession)
ESystem::_initzz_();
printf("EProxyIoSession::initzz()\n");
DEFINE_STATIC_INITZZ_END

EProxyIoSession::~EProxyIoSession() {
	delete proxyAddress;
	delete eventQueue;
}

EProxyIoSession::EProxyIoSession(EInetSocketAddress* proxyAddress, sp<EProxyRequest>& request) :
		handler(null), proxyFilter(null), connector(null), proxyAddress(null),
		reconnectionNeeded(false), authenticationFailed(false) {
	setProxyAddress(proxyAddress);
	setRequest(request);
	eventQueue = new EIoSessionEventQueue(this);
}

EIoSessionEventQueue* EProxyIoSession::getEventQueue() {
	return eventQueue;
}

EList<int>* EProxyIoSession::getPreferedOrder() {
	return &preferedOrder;
}

void EProxyIoSession::setPreferedOrder(EList<int>* preferedOrder) {
	if (preferedOrder) {
		sp<EIterator<int> > iter = preferedOrder->iterator();
		while (iter->hasNext()) {
			this->preferedOrder.add(iter->next());
		}
	}
}

EProxyLogicHandler* EProxyIoSession::getHandler() {
	return handler;
}

void EProxyIoSession::setHandler(EProxyLogicHandler* handler) {
	this->handler = handler;
}

EProxyFilter* EProxyIoSession::getProxyFilter() {
	return proxyFilter;
}

void EProxyIoSession::setProxyFilter(EProxyFilter* proxyFilter) {
	this->proxyFilter = proxyFilter;
}

sp<EProxyRequest> EProxyIoSession::getRequest() {
	return request;
}

sp<EIoSession> EProxyIoSession::getSession() {
	return session;
}

void EProxyIoSession::setSession(sp<EIoSession>& session) {
	this->session = session;
}

EProxyConnector* EProxyIoSession::getConnector() {
	return connector;
}

void EProxyIoSession::setConnector(EProxyConnector* connector) {
	this->connector = connector;
}

EInetSocketAddress* EProxyIoSession::getProxyAddress() {
	return proxyAddress;
}

boolean EProxyIoSession::isReconnectionNeeded() {
	return reconnectionNeeded;
}

void EProxyIoSession::setReconnectionNeeded(boolean reconnectionNeeded) {
	this->reconnectionNeeded = reconnectionNeeded;
}

boolean EProxyIoSession::isAuthenticationFailed() {
	return authenticationFailed;
}

void EProxyIoSession::setAuthenticationFailed(boolean authenticationFailed) {
	this->authenticationFailed = authenticationFailed;
}

void EProxyIoSession::setRequest(sp<EProxyRequest> request) {
	if (request == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "request cannot be null");
	}

	this->request = request;
}

void EProxyIoSession::setProxyAddress(EInetSocketAddress* proxyAddress) {
	if (proxyAddress == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "proxyAddress object cannot be null");
	}

	this->proxyAddress = new EInetSocketAddress(*proxyAddress);
}

} /* namespace eio */
} /* namespace efc */
