/*
 * EHttpSmartProxyHandler.hh
 *
 *  Created on: 2016-7-8
 *      Author: cxxjava@163.com
 */

#ifndef EHTTPSMARTPROXYHANDLER_HH_
#define EHTTPSMARTPROXYHANDLER_HH_

#include "./EAbstractHttpLogicHandler.hh"
#include "ELog.hh"

namespace efc {
namespace eio {

/**
 * HttpSmartProxyHandler.java - HTTP proxy handler that automatically handles forwarding a request
 * to the appropriate authentication mechanism logic handler.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since MINA 2.0.0-M3
 */

class EHttpSmartProxyHandler: public EAbstractHttpLogicHandler {
public:
	virtual ~EHttpSmartProxyHandler();

	EHttpSmartProxyHandler(sp<EProxyIoSession>& proxyIoSession);

	/**
	 * Performs the handshake processing.
	 *
	 * @param nextFilter the next filter
	 */
	virtual void doHandshake(EIoFilter::NextFilter* nextFilter) THROWS(EProxyAuthException);

	/**
	 * Handle a HTTP response from the proxy server.
	 *
	 * @param response The proxy response.
	 */
	virtual void handleResponse(sp<EHttpProxyResponse>& response) THROWS(EProxyAuthException);

private:
	static sp<ELogger> logger;// = LoggerFactory.getLogger(HttpSmartProxyHandler.class);

	/**
	 * Has the HTTP proxy request been sent ?
	 */
	boolean requestSent;// = false;

	/**
	 * The automatically selected http authentication logic handler.
	 */
	EAbstractAuthLogicHandler* authHandler;

	/**
	 * Automatic selection of the authentication algorithm. If <code>preferedOrder</code> is set then
	 * algorithms are selected from the list order otherwise the algorithm tries to select the most
	 * secured algorithm available first.
	 *
	 * @param response the proxy response
	 */
	void autoSelectAuthHandler(sp<EHttpProxyResponse>& response) THROWS(EProxyAuthException);
};

} /* namespace eio */
} /* namespace efc */
#endif /* EHTTPSMARTPROXYHANDLER_HH_ */
