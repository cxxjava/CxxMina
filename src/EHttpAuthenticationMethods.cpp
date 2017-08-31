/*
 * EHttpAuthenticationMethods.cpp
 *
 *  Created on: 2016-6-16
 *      Author: cxxjava@lotuseed.com
 */

#include "../inc/EHttpAuthenticationMethods.hh"
#include "../inc/EAbstractAuthLogicHandler.hh"
#include "../inc/EProxyIoSession.hh"
#include "../inc/EHttpBasicAuthLogicHandler.hh"
#include "../inc/EHttpDigestAuthLogicHandler.hh"
//#include "../inc/EHttpNTLMAuthLogicHandler.hh"
#include "../inc/EHttpNoAuthLogicHandler.hh"

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
