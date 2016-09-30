/*
 * EProxyIoSessionInitializer.cpp
 *
 *  Created on: 2016-7-5
 *      Author: cxxjava@163.com
 */

#include "EProxyIoSessionInitializer.hh"

namespace efc {
namespace eio {

EProxyIoSessionInitializer::EProxyIoSessionInitializer(EIoSessionInitializer* wrappedSessionInitializer,
	            sp<EProxyIoSession>& proxyIoSession) {
	this->wrappedSessionInitializer = wrappedSessionInitializer;
	this->proxyIoSession = proxyIoSession;
}

sp<EProxyIoSession> EProxyIoSessionInitializer::getProxySession() {
	return proxyIoSession;
}

void EProxyIoSessionInitializer::initializeSession(sp<EIoSession> session, sp<EIoFuture> future) {
	if (wrappedSessionInitializer != null) {
		wrappedSessionInitializer->initializeSession(session, future);
	}

	if (proxyIoSession != null) {
		proxyIoSession->setSession(session);
		session->setAttribute(EProxyIoSession::PROXY_SESSION, proxyIoSession);
	}
}

} /* namespace eio */
} /* namespace efc */
