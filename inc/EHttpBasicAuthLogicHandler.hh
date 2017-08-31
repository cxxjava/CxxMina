/*
 * EHttpBasicAuthLogicHandler.hh
 *
 *  Created on: 2016-6-17
 *      Author: cxxjava@lotuseed.com
 */

#ifndef EHTTPBASICAUTHLOGICHANDLER_HH_
#define EHTTPBASICAUTHLOGICHANDLER_HH_

#include "./EAbstractAuthLogicHandler.hh"

namespace efc {
namespace eio {

/**
 * HttpBasicAuthLogicHandler.java - HTTP Basic authentication mechanism logic handler.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since MINA 2.0.0-M3
 */

class EHttpBasicAuthLogicHandler: public EAbstractAuthLogicHandler {
public:
	/**
	 * Build an HttpBasicAuthLogicHandler
	 *
	 * @param proxyIoSession The proxy session
	 * @throws ProxyAuthException If we had a probelm during the proxy authentication
	 */
	EHttpBasicAuthLogicHandler(sp<EProxyIoSession>& proxyIoSession) THROWS(EProxyAuthException);

	/**
	 * {@inheritDoc}
	 */
	virtual void doHandshake(EIoFilter::NextFilter* nextFilter) THROWS(EProxyAuthException);

	/**
	 * {@inheritDoc}
	 */
	virtual void handleResponse(sp<EHttpProxyResponse>& response) THROWS(EProxyAuthException);

	/**
	 * Computes the authorization header value.
	 *
	 * @param username the user name
	 * @param password the user password
	 * @return the authorization header value as a string
	 */
	static EString createAuthorization(const char* username, const char* password);

private:
	static sp<ELogger> logger;// = LoggerFactory.getLogger(HttpBasicAuthLogicHandler.class);
};

} /* namespace eio */
} /* namespace efc */
#endif /* EHTTPBASICAUTHLOGICHANDLER_HH_ */
