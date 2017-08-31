/*
 * EAbstractAuthLogicHandler.hh
 *
 *  Created on: 2016-6-16
 *      Author: cxxjava@lotuseed.com
 */

#ifndef EABSTRACTAUTHLOGICHANDLER_HH_
#define EABSTRACTAUTHLOGICHANDLER_HH_

#include "./EIoFilter.hh"
#include "./EProxyRequest.hh"
#include "./EHttpProxyRequest.hh"
#include "./EHttpProxyResponse.hh"
#include "./EProxyAuthException.hh"
#include "ELog.hh"

namespace efc {
namespace eio {

/**
 * AbstractAuthLogicHandler.java - Abstract class that handles an authentication
 * mechanism logic.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since MINA 2.0.0-M3
 */

class EProxyIoSession;

class EAbstractAuthLogicHandler: public EObject {
public:
	virtual ~EAbstractAuthLogicHandler();

	/**
	 * Method called at each step of the handshaking process.
	 *
	 * @param nextFilter the next filter
	 * @throws ProxyAuthException If we get an error during the proxy authentication
	 */
	virtual void doHandshake(EIoFilter::NextFilter* nextFilter) THROWS(EProxyAuthException) = 0;

	/**
	 * Handles a HTTP response from the proxy server.
	 *
	 * @param response The HTTP response.
	 * @throws ProxyAuthException If we get an error during the proxy authentication
	 */
	virtual void handleResponse(sp<EHttpProxyResponse>& response) THROWS(EProxyAuthException) = 0;

public:
	/**
	 * Returns the value of the named header. If it has multiple values
	 * then an {@link IllegalArgumentException} is thrown
	 *
	 * @param headers the http headers map
	 * @param key the key of the header
	 * @return the value of the http header
	 */
	static EString getSingleValuedHeader(EMap<EString*, EList<EString*>*>* headers, const char* key);

	/**
	 * Adds an header to the provided map of headers.
	 *
	 * @param headers the http headers map
	 * @param key the name of the new header to add
	 * @param value the value of the added header
	 * @param singleValued if true and the map already contains one value
	 * then it is replaced by the new value. Otherwise it simply adds a new
	 * value to this multi-valued header.
	 */
	static void addValueToHeader(EMap<EString*, EList<EString*>*>* headers, const char* key, const char* value,
            boolean singleValued);

	/**
	 * Try to force proxy connection to be kept alive.
	 *
	 * @param headers the request headers
	 */
	static void addKeepAliveHeaders(EMap<EString*, EList<EString*>*>* headers);

protected:
	/**
	 * The request to be handled by the proxy.
	 */
	sp<EProxyRequest> request;

	/**
	 * Object that contains all the proxy authentication session informations.
	 */
	sp<EProxyIoSession> proxyIoSession;

	/**
	 * The current handshake step.
	 */
	int step;// = 0;

	/**
	 * Instantiates a handler for the given proxy session.
	 *
	 * @param proxyIoSession the proxy session object
	 * @throws ProxyAuthException If we get an error during the proxy authentication
	 */
	EAbstractAuthLogicHandler(sp<EProxyIoSession>& proxyIoSession) THROWS(EProxyAuthException);

	/**
	 * Sends an HTTP request.
	 *
	 * @param nextFilter the next filter
	 * @param request the request to write
	 * @throws ProxyAuthException If we get an error during the proxy authentication
	 */
	void writeRequest(EIoFilter::NextFilter* nextFilter, sp<EHttpProxyRequest>& request) THROWS(EProxyAuthException);

private:
	static sp<ELogger> logger;// = LoggerFactory.getLogger(AbstractAuthLogicHandler.class);
};

} /* namespace eio */
} /* namespace efc */
#endif /* EABSTRACTAUTHLOGICHANDLER_HH_ */
