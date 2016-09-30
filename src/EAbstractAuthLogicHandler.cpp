/*
 * EAbstractAuthLogicHandler.cpp
 *
 *  Created on: 2016-6-16
 *      Author: cxxjava@lotuseed.com
 */

#include "EAbstractAuthLogicHandler.hh"
#include "EAbstractHttpLogicHandler.hh"
#include "EHttpProxyConstants.hh"
#include "EProxyIoSession.hh"

namespace efc {
namespace eio {

sp<ELogger> EAbstractAuthLogicHandler::logger = ELoggerManager::getLogger("EAbstractAuthLogicHandler");

EAbstractAuthLogicHandler::~EAbstractAuthLogicHandler() {
	//
}

EAbstractAuthLogicHandler::EAbstractAuthLogicHandler(
		sp<EProxyIoSession>& proxyIoSession): step(0) {
	this->proxyIoSession = proxyIoSession;
	this->request = proxyIoSession->getRequest();

	if (this->request == null || !(instanceof<EHttpProxyRequest>(this->request.get()))) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "request parameter should be a non null HttpProxyRequest instance");
	}
}

//@see: StringUtils.java
EString EAbstractAuthLogicHandler::getSingleValuedHeader(EMap<EString*, EList<EString*>*>* headers, const char* key) {
	EString k(key);
	EList<EString*>* values = headers->get(&k);

	if (values == null) {
		return null;
	}

	if (values->size() > 1) {
		EString msg("Header with key [\"" + k + "\"] isn't single valued !");
		throw EIllegalArgumentException(__FILE__, __LINE__, msg.c_str());
	}

	return values->getAt(0);
}

//@see: StringUtils.java
void EAbstractAuthLogicHandler::addValueToHeader(EMap<EString*, EList<EString*>*>* headers, const char* key, const char* value,
        boolean singleValued) {
	sp<EString> k(new EString(key));
	EList<EString*>* values = headers->get(k.get());
	if (values == null) {
		values = new EArrayList<EString*>(1);
		headers->put(k.dismiss(), values);
	}

	if (singleValued && values->size() == 1) {
		values->setAt(0, new EString(value));
	} else {
		values->add(new EString(value));
	}
}

void EAbstractAuthLogicHandler::addKeepAliveHeaders(
		EMap<EString*, EList<EString*>*>* headers) {
	addValueToHeader(headers, "Keep-Alive", EHttpProxyConstants_DEFAULT_KEEP_ALIVE_TIME, true);
	addValueToHeader(headers, "Proxy-Connection", "keep-Alive", true);
}

void EAbstractAuthLogicHandler::writeRequest(EIoFilter::NextFilter* nextFilter,
		sp<EHttpProxyRequest>& request) {
	logger->debug(__FILE__, __LINE__, "  sending HTTP request");

	(dynamic_cast<EAbstractHttpLogicHandler*>(proxyIoSession->getHandler()))->writeRequest(nextFilter, request);
}

} /* namespace eio */
} /* namespace efc */
