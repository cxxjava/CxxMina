/*
 * EHttpNoAuthLogicHandler.hh
 *
 *  Created on: 2016-6-17
 *      Author: cxxjava@lotuseed.com
 */

#ifndef EHTTPNOAUTHLOGICHANDLER_HH_
#define EHTTPNOAUTHLOGICHANDLER_HH_

#include "./EAbstractAuthLogicHandler.hh"

namespace efc {
namespace eio {

/**
 * HttpNoAuthLogicHandler.java - HTTP 'no auth' mechanism logic handler.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since MINA 2.0.0-M3
 */

class EHttpNoAuthLogicHandler: public EAbstractAuthLogicHandler {
public:
	/**
	 * Build an HttpNoAuthLogicHandler
	 *
	 * @param proxyIoSession The proxy session
	 * @throws ProxyAuthException If we had a probelm during the proxy authentication
	 */
	EHttpNoAuthLogicHandler(sp<EProxyIoSession>& proxyIoSession) THROWS(EProxyAuthException);

	/**
	 * {@inheritDoc}
	 */
	virtual void doHandshake(EIoFilter::NextFilter* nextFilter) THROWS(EProxyAuthException);

	/**
	 * {@inheritDoc}
	 */
	virtual void handleResponse(sp<EHttpProxyResponse>& response) THROWS(EProxyAuthException);

private:
	static sp<ELogger> logger;// = LoggerFactory.getLogger(HttpBasicAuthLogicHandler.class);
};

} /* namespace eio */
} /* namespace efc */
#endif /* EHTTPNOAUTHLOGICHANDLER_HH_ */
