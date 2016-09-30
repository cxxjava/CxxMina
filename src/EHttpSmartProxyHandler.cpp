/*
 * EHttpSmartProxyHandler.cpp
 *
 *  Created on: 2016-7-8
 *      Author: cxxjava@163.com
 */

#include "EHttpSmartProxyHandler.hh"
#include "EAbstractAuthLogicHandler.hh"

namespace efc {
namespace eio {

sp<ELogger> EHttpSmartProxyHandler::logger = ELoggerManager::getLogger("EHttpSmartProxyHandler");

EHttpSmartProxyHandler::~EHttpSmartProxyHandler() {

}

EHttpSmartProxyHandler::EHttpSmartProxyHandler(
		sp<EProxyIoSession>& proxyIoSession) : EAbstractHttpLogicHandler(proxyIoSession),
		requestSent(false),
		authHandler(null) {
}

void EHttpSmartProxyHandler::doHandshake(EIoFilter::NextFilter* nextFilter) {
	logger->debug(__FILE__, __LINE__, " doHandshake()");

	if (authHandler != null) {
		authHandler->doHandshake(nextFilter);
	} else {
		if (requestSent) {
			// Safety check
			throw EProxyAuthException(__FILE__, __LINE__, "Authentication request already sent");
		}

		logger->debug(__FILE__, __LINE__, "  sending HTTP request");

		// Compute request headers
		sp<EHttpProxyRequest> req = dynamic_pointer_cast<EHttpProxyRequest>(getProxyIoSession()->getRequest());
		/* @see:
		Map<String, List<String>> headers = req->getHeaders() != null ? req.getHeaders()
				: new HashMap<String, List<String>>();
		*/
		EMap<EString*, EList<EString*>*>* headers = req->getHeaders();
		EAbstractAuthLogicHandler::addKeepAliveHeaders(headers);
		req->setHeaders(headers);

		// Write request to the proxy
		writeRequest(nextFilter, req);
		requestSent = true;
	}
}

void EHttpSmartProxyHandler::handleResponse(sp<EHttpProxyResponse>& response) {
	if (!isHandshakeComplete()
			&& (EAbstractAuthLogicHandler::getSingleValuedHeader(response->getHeaders(),
                                                                 "Proxy-Connection").equalsIgnoreCase("close") || EAbstractAuthLogicHandler::getSingleValuedHeader(
							response->getHeaders(), "Connection").equalsIgnoreCase("close"))) {
		getProxyIoSession()->setReconnectionNeeded(true);
	}

	if (response->getStatusCode() == 407) {
		if (authHandler == null) {
			autoSelectAuthHandler(response);
		}
		authHandler->handleResponse(response);
	} else {
        EString msg("Error: unexpected response code "); msg << response->getStatusLine() << " received from proxy.";
		throw EProxyAuthException(__FILE__, __LINE__, msg.c_str());
	}
}

void EHttpSmartProxyHandler::autoSelectAuthHandler(
		sp<EHttpProxyResponse>& response) {
	// Get the Proxy-Authenticate header
	EString k("Proxy-Authenticate");
	EList<EString*>* values = response->getHeaders()->get(&k);
	sp<EProxyIoSession> proxyIoSession = getProxyIoSession();

	if (values == null || values->size() == 0) {
		authHandler = EHttpAuthenticationMethods::getNewHandler(EHttpAuthenticationMethods::NO_AUTH, proxyIoSession);

	} else if (getProxyIoSession()->getPreferedOrder() == null) {
		// No preference order set for auth mechanisms
		int method = -1;

		// Test which auth mechanism to use. First found is the first used
		// that's why we test in a decreasing security quality order.
		sp<EIterator<EString*> > iter = values->iterator();
		while (iter->hasNext()) {
			EString* proxyAuthHeader = iter->next();
			proxyAuthHeader->toLowerCase();

			if (proxyAuthHeader->contains("ntlm")) {
				method = EHttpAuthenticationMethods::NTLM;
				break;
			} else if (proxyAuthHeader->contains("digest") && method != EHttpAuthenticationMethods::NTLM) {
				method = EHttpAuthenticationMethods::DIGEST;
			} else if (proxyAuthHeader->contains("basic") && method == -1) {
				method = EHttpAuthenticationMethods::BASIC;
			}
		}

		if (method != -1) {
			try {
				authHandler = EHttpAuthenticationMethods::getNewHandler(method, proxyIoSession);
			} catch (EException& ex) {
				logger->debug(__FILE__, __LINE__, "Following exception occured:", ex);
			}
		}

		if (authHandler == null) {
			authHandler = EHttpAuthenticationMethods::getNewHandler(EHttpAuthenticationMethods::NO_AUTH, proxyIoSession);
		}

	} else {
		EList<int>* methods = proxyIoSession->getPreferedOrder();
		sp<EIterator<int> > it = methods->iterator();
		while (it->hasNext()) {
			int method = it->next();

			if (authHandler != null) {
				break;
			}

			if (method == EHttpAuthenticationMethods::NO_AUTH) {
				authHandler = EHttpAuthenticationMethods::getNewHandler(EHttpAuthenticationMethods::NO_AUTH, proxyIoSession);
				break;
			}

			sp<EIterator<EString*> > iter = values->iterator();
			while (iter->hasNext()) {
				EString* proxyAuthHeader = iter->next();
				proxyAuthHeader->toLowerCase();

				try {
					// test which auth mechanism to use
					if (proxyAuthHeader->contains("basic") && method == EHttpAuthenticationMethods::BASIC) {
						authHandler = EHttpAuthenticationMethods::getNewHandler(EHttpAuthenticationMethods::BASIC, proxyIoSession);
						break;
					} else if (proxyAuthHeader->contains("digest") && method == EHttpAuthenticationMethods::DIGEST) {
						authHandler = EHttpAuthenticationMethods::getNewHandler(EHttpAuthenticationMethods::DIGEST, proxyIoSession);
						break;
					} else if (proxyAuthHeader->contains("ntlm") && method == EHttpAuthenticationMethods::NTLM) {
						authHandler = EHttpAuthenticationMethods::getNewHandler(EHttpAuthenticationMethods::NTLM, proxyIoSession);
						break;
					}
				} catch (EException& ex) {
					logger->debug(__FILE__, __LINE__, "Following exception occured:", ex);
				}
			}
		}

	}

	if (authHandler == null) {
		EString msg("Unknown authentication mechanism(s): "); msg << values->toString();
		throw EProxyAuthException(__FILE__, __LINE__, msg.c_str());
	}
}

} /* namespace eio */
} /* namespace efc */
