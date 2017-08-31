/*
 * EHttpAuthenticationMethods.hh
 *
 *  Created on: 2016-6-16
 *      Author: cxxjava@lotuseed.com
 */

#ifndef EHTTPAUTHENTICATIONMETHODS_HH_
#define EHTTPAUTHENTICATIONMETHODS_HH_

#include "Efc.hh"
#include "./EProxyAuthException.hh"

namespace efc {
namespace eio {

/**
 * HttpAuthenticationMethods.java - Enumerates all known http authentication methods.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since MINA 2.0.0-M3
 */

class EProxyIoSession;
class EAbstractAuthLogicHandler;

class EHttpAuthenticationMethods {
public:
	enum METHOD {
		NO_AUTH = 1,
		BASIC = 2,
		NTLM = 3,
		DIGEST = 4
	};

public:
	/**
	 * Creates an {@link AbstractAuthLogicHandler} to handle the authentication mechanism.
	 *
	 * @param method the authentication mechanism to use
	 * @param proxyIoSession the proxy session object
	 * @return a new logic handler
	 * @throws ProxyAuthException If we get an error during the proxy authentication
	 */
	static EAbstractAuthLogicHandler* getNewHandler(int method, sp<EProxyIoSession>& proxyIoSession) THROWS(EProxyAuthException);

private:
	EHttpAuthenticationMethods(); //private
};

} /* namespace eio */
} /* namespace efc */
#endif /* EHTTPAUTHENTICATIONMETHODS_HH_ */
