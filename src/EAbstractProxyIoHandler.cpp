/*
 * EAbstractProxyIoHandler.cpp
 *
 *  Created on: 2016-7-6
 *      Author: cxxjava@163.com
 */

#include "../inc/EAbstractProxyIoHandler.hh"
#include "../inc/EProxyIoSession.hh"
#include "../inc/ESocksProxyRequest.hh"

namespace efc {
namespace eio {

sp<ELogger> EAbstractProxyIoHandler::logger = ELoggerManager::getLogger("EAbstractProxyIoHandler");

void EAbstractProxyIoHandler::sessionOpened(sp<EIoSession>& session) {
	sp<EProxyIoSession> proxyIoSession = dynamic_pointer_cast<EProxyIoSession>(session->getAttribute(EProxyIoSession::PROXY_SESSION.get()));

	if (instanceof<ESocksProxyRequest>(proxyIoSession->getRequest().get()) || proxyIoSession->isAuthenticationFailed()
			|| proxyIoSession->getHandler()->isHandshakeComplete()) {
		proxySessionOpened(session);
	} else {
		logger->debug(__FILE__, __LINE__, "Filtered session opened event !");
	}
}

} /* namespace eio */
} /* namespace efc */
