/*
 * EHttpBasicAuthLogicHandler.cpp
 *
 *  Created on: 2016-6-17
 *      Author: cxxjava@lotuseed.com
 */

#include "EHttpBasicAuthLogicHandler.hh"
#include "EHttpProxyConstants.hh"
#include "EStringUtilities.hh"

namespace efc {
namespace eio {

sp<ELogger> EHttpBasicAuthLogicHandler::logger = ELoggerManager::getLogger("EHttpBasicAuthLogicHandler");

EHttpBasicAuthLogicHandler::EHttpBasicAuthLogicHandler(sp<EProxyIoSession>& proxyIoSession):
		EAbstractAuthLogicHandler(proxyIoSession) {
	(dynamic_pointer_cast<EHttpProxyRequest>(request))->checkRequiredProperties(EHttpProxyConstants_USER_PROPERTY,
			EHttpProxyConstants_PWD_PROPERTY, NULL);
}

void EHttpBasicAuthLogicHandler::doHandshake(EIoFilter::NextFilter* nextFilter) {
	logger->debug(__FILE__, __LINE__, " doHandshake()");

	if (step > 0) {
		throw EProxyAuthException(__FILE__, __LINE__, "Authentication request already sent");
	}

	// Send request
	sp<EHttpProxyRequest> req = dynamic_pointer_cast<EHttpProxyRequest>(request);
	EMap<EString*, EList<EString*>*>* headers = req->getHeaders();

	EString uk(EHttpProxyConstants_USER_PROPERTY);
	EString* username = req->getProperties()->get(&uk);
	EString pw(EHttpProxyConstants_PWD_PROPERTY);
	EString* password = req->getProperties()->get(&pw);

    EString value("Basic " + createAuthorization(username->c_str(), password->c_str()));
    EAbstractAuthLogicHandler::addValueToHeader(headers, "Proxy-Authorization",
                                     value.c_str(), true);
	addKeepAliveHeaders(headers);

	writeRequest(nextFilter, req);
	step++;
}

void EHttpBasicAuthLogicHandler::handleResponse(sp<EHttpProxyResponse>& response) {
	if (response->getStatusCode() != 407) {
        EString msg("Received error response code ("); msg << response->getStatusLine() << ").";
		throw EProxyAuthException(__FILE__, __LINE__, msg.c_str());
	}
}

EString EHttpBasicAuthLogicHandler::createAuthorization(const char* username,
		const char* password) {
	EString s(username); s << ":"; s << password;
	return EBase64::encodeBase64((byte*)s.c_str(), s.length());
}

} /* namespace eio */
} /* namespace efc */
