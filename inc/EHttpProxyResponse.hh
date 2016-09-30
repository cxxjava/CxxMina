/*
 * EHttpProxyResponse.hh
 *
 *  Created on: 2016-6-16
 *      Author: cxxjava@lotuseed.com
 */

#ifndef EHTTPPROXYRESPONSE_HH_
#define EHTTPPROXYRESPONSE_HH_

#include "Efc.hh"

namespace efc {
namespace eio {

/**
 * HttpProxyResponse.java - Wrapper class for HTTP requests.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since MINA 2.0.0-M3
 */

class EHttpProxyResponse: public EObject {
public:
	/**
	 * @return the HTTP response protocol version.
	 */
	const char* getHttpVersion();

	/**
	 * @return the HTTP response status code.
	 */
	int getStatusCode();

	/**
	 * @return the HTTP response status line.
	 */
	const char* getStatusLine();

	/**
	 * @return the HTTP response body.
	 */
	const char* getBody();

	/**
	 * Sets the HTTP response body.
	 *
	 * @param body The HTTP Body
	 */
	void setBody(const char* body);

	/**
	 * @return the HTTP response headers.
	 */
	EMap<EString*, EList<EString*>*>* getHeaders();

/*protected:*/
	/**
	 * Constructor of an HTTP proxy response.
	 *
	 * @param httpVersion the protocol version
	 * @param statusLine the response status line
	 * @param headers the response headers
	 */
	EHttpProxyResponse(const char* httpVersion, const char* statusLine,
			EMap<EString*, EList<EString*>*>* headers);

private:
	/**
	 * The HTTP response protocol version.
	 */
	EString httpVersion;

	/**
	 * The HTTP response status line.
	 */
	EString statusLine;

	/**
	 * The HTTP response status code;
	 */
	int statusCode;

	/**
	 * The HTTP response headers.
	 */
	EHashMap<EString*, EList<EString*>* > headers;

	/**
	 * The HTTP response body.
	 */
	EString body;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EHTTPPROXYRESPONSE_HH_ */
