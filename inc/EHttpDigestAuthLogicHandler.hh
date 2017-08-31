/*
 * EHttpDigestAuthLogicHandler.hh
 *
 *  Created on: 2016-6-17
 *      Author: cxxjava@lotuseed.com
 */

#ifndef EHTTPDIGESTAUTHLOGICHANDLER_HH_
#define EHTTPDIGESTAUTHLOGICHANDLER_HH_

#include "./EAbstractAuthLogicHandler.hh"

namespace efc {
namespace eio {

/**
 * HttpDigestAuthLogicHandler.java - HTTP Digest authentication mechanism logic handler.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since MINA 2.0.0-M3
 */

class EHttpDigestAuthLogicHandler: public EAbstractAuthLogicHandler {
public:
	/**
	 * Build an HttpDigestAuthLogicHandler
	 *
	 * @param proxyIoSession The proxy session
	 * @throws ProxyAuthException If we had a probelm during the proxy authentication
	 */
	EHttpDigestAuthLogicHandler(sp<EProxyIoSession>& proxyIoSession) THROWS(EProxyAuthException);

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

	/**
	 * The challenge directives provided by the server.
	 */
	sp<EHashMap<EString*, EString*> > directives;// = null;

	/**
	 * The response received to the last request.
	 */
	sp<EHttpProxyResponse> response;

	static ESecureRandom rnd;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EHTTPDIGESTAUTHLOGICHANDLER_HH_ */
