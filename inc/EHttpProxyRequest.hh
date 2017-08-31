/*
 * EHttpProxyRequest.hh
 *
 *  Created on: 2016-6-8
 *      Author: cxxjava@163.com
 */

#ifndef EHTTPPROXYREQUEST_HH_
#define EHTTPPROXYREQUEST_HH_

#include "./EProxyRequest.hh"
#include "./EProxyAuthException.hh"
#include "ELog.hh"

namespace efc {
namespace eio {

/**
 * HttpProxyRequest.java - Wrapper class for HTTP requests.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since MINA 2.0.0-M3
 */

class EHttpProxyRequest: public EProxyRequest {
public:
	virtual ~EHttpProxyRequest();

	/**
	 * Constructor which creates a HTTP/1.0 CONNECT request to the specified
	 * endpoint.
	 *
	 * @param endpointAddress the endpoint to connect to
	 */
	EHttpProxyRequest(EInetSocketAddress* endpointAddress);

	/**
	 * Constructor which creates a CONNECT request to the specified endpoint
	 * using the provided protocol version.
	 *
	 * @param endpointAddress the endpoint to connect to
	 * @param httpVersion the HTTP protocol version
	 */
	EHttpProxyRequest(EInetSocketAddress* endpointAddress, const char* httpVersion);

	/**
	 * Constructor which creates a HTTP/1.0 GET request to the specified
	 * http URI.
	 *
	 * @param httpURI the target URI
	 */
	EHttpProxyRequest(const char* httpURI);

	/**
	 * Constructor which creates a GET request to the specified http URI
	 * using the provided protocol version
	 *
	 * @param httpURI the target URI
	 * @param httpVersion the HTTP protocol version
	 */
	EHttpProxyRequest(const char* httpURI, const char* httpVersion);

	/**
	 * Constructor which creates a request using the provided HTTP verb targeted at
	 * the specified http URI using the provided protocol version.
	 *
	 * @param httpVerb the HTTP verb to use
	 * @param httpURI the target URI
	 * @param httpVersion the HTTP protocol version
	 */
	EHttpProxyRequest(const char* httpVerb, const char* httpURI, const char* httpVersion);

	/**
	 * @return the HTTP request verb.
	 */
	const char* getHttpVerb();

	/**
	 * @return the HTTP version.
	 */
	const char* getHttpVersion();

	/**
	 * Sets the HTTP version.
	 *
	 * @param httpVersion the HTTP protocol version
	 */
	void setHttpVersion(const char* httpVersion);

	/**
	 * @return the host to which we are connecting.
	 */
	/*synchronized*/ const char* getHost();

	/**
	 * @return the request HTTP URI.
	 */
	const char* getHttpURI();

	/**
	 * @return the HTTP headers.
	 */
	EMap<EString*, EList<EString*>*>* getHeaders();

	/**
	 * Set the HTTP headers.
	 *
	 * @param headers The HTTP headers to set
	 */
	void setHeaders(EMap<EString*, EList<EString*>*>* headers);

	/**
	 * @return additional properties for the request.
	 */
	EMap<EString*, EString*>* getProperties();

	/**
	 * Set additional properties for the request.
	 *
	 * @param properties The properties to add to the reqyest
	 */
	void setProperties(EMap<EString*, EString*>* properties);

	/**
	 * Check if the given property(ies) is(are) set. Otherwise throws a
	 * {@link ProxyAuthException}.
	 *
	 * @param propNames The list of property name to check
	 * @throws ProxyAuthException If we get an error during the proxy authentication
	 * @remark last propName must be a null value!
	 */
	void checkRequiredProperties(const char* propNames, ...) THROWS(EProxyAuthException);

	 /**
	 * @return the string representation of the HTTP request .
	 */
	EString toHttpString();

private:
	static sp<ELogger> logger;// = LoggerFactory.getLogger(HttpProxyRequest.class);

    /**
     * The HTTP verb.
     */
    EString httpVerb;

    /**
     * The HTTP URI.
     */
    EString httpURI;

    /**
     * The HTTP protocol version.
     */
    EString httpVersion;

    /**
     * The target hostname.
     */
    sp<EString> host;

    /**
     * The request headers.
     */
    EHashMap<EString*, EList<EString*>* > headers;

    /**
     * The additionnal properties supplied to use with the proxy for
     * authentication for example.
     */
    EHashMap<EString*, EString*> properties;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EHTTPPROXYREQUEST_HH_ */
