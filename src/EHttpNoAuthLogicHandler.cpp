/*
 * EHttpNoAuthLogicHandler.cpp
 *
 *  Created on: 2016-6-17
 *      Author: cxxjava@lotuseed.com
 */

#include "../inc/EHttpNoAuthLogicHandler.hh"
#include "../inc/EHttpProxyConstants.hh"
#include "../inc/EProxyAuthException.hh"

namespace efc {
namespace eio {

sp<ELogger> EHttpNoAuthLogicHandler::logger = ELoggerManager::getLogger("EHttpNoAuthLogicHandler");

EHttpNoAuthLogicHandler::EHttpNoAuthLogicHandler(sp<EProxyIoSession>& proxyIoSession):
		EAbstractAuthLogicHandler(proxyIoSession) {
}

void EHttpNoAuthLogicHandler::doHandshake(EIoFilter::NextFilter* nextFilter) {
	logger->debug(__FILE__, __LINE__, " doHandshake()");

	// Just send the request, no authentication needed
	sp<EHttpProxyRequest> req = dynamic_pointer_cast<EHttpProxyRequest>(request);
	writeRequest(nextFilter, req);
	step++;
}

void EHttpNoAuthLogicHandler::handleResponse(sp<EHttpProxyResponse>& response) {
	// Should never get here !
	EString s("Received error response code ("); s << response->getStatusLine(); s << ").";
	throw EProxyAuthException(__FILE__, __LINE__, s.c_str());
}

} /* namespace eio */
} /* namespace efc */
