/*
 * EHttpAuthenticationMethods.cpp
 *
 *  Created on: 2016-6-16
 *      Author: cxxjava@lotuseed.com
 */

#include "EHttpAuthenticationMethods.hh"
#include "EAbstractAuthLogicHandler.hh"
#include "EProxyIoSession.hh"
#include "EHttpBasicAuthLogicHandler.hh"
#include "EHttpDigestAuthLogicHandler.hh"
//#include "EHttpNTLMAuthLogicHandler.hh"
#include "EHttpNoAuthLogicHandler.hh"

namespace efc {
namespace eio {

EAbstractAuthLogicHandler* EHttpAuthenticationMethods::getNewHandler(int method, sp<EProxyIoSession>& proxyIoSession) {
	if (method == BASIC)
		return new EHttpBasicAuthLogicHandler(proxyIoSession);
	else if (method == DIGEST)
		return new EHttpDigestAuthLogicHandler(proxyIoSession);
	//else if (method == NTLM)
	//	return new EHttpNTLMAuthLogicHandler(proxyIoSession);
	else if (method == NO_AUTH)
		return new EHttpNoAuthLogicHandler(proxyIoSession);
	else
		return null;
}

} /* namespace eio */
} /* namespace efc */
