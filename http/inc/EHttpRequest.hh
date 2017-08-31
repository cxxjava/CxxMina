/*
 * EHttpRequest.hh
 *
 *  Created on: 2017-1-4
 *      Author: cxxjava@163.com
 */

#ifndef EHTTPREQUEST_HH_
#define EHTTPREQUEST_HH_

#include "./EHttpMessage.hh"
#include "./EHttpMethod.hh"

namespace efc {
namespace eio {

/**
 * An HTTP request
 *
 * @author jvermillar
 *
 */

interface EHttpRequest: virtual public EHttpMessage {
	virtual ~EHttpRequest(){}

	/**
	 * Determines whether this request contains at least one parameter with the specified name
	 *
	 * @param name The parameter name
	 * @return <tt>true</tt> if this request contains at least one parameter with the specified name
	 */
	virtual boolean containsParameter(const char* name) = 0;

	/**
	 * Returns the value of a request parameter as a String, or null if the parameter does not exist.
	 *
	 * If the request contained multiple parameters with the same name, this method returns the first parameter
	 * encountered in the request with the specified name
	 *
	 * @param name The parameter name
	 * @return The value
	 */
	virtual sp<EString> getParameter(const char* name) = 0;

	virtual sp<EString> getQueryString() = 0;

	/**
	 * @return a read only {@link Map} of query parameters whose key is a {@link String} and whose value is a
	 * {@link List} of {@link String}s.
	 */
	virtual sp<EMap<EString*, EList<EString*>*> > getParameters() = 0;

	/**
	 * Return the HTTP method used for this message {@link HttpMethod}
	 *
	 * @return the method
	 */
	virtual const char* getMethod() = 0;

	/**
	 * Retrurn the HTTP request path
	 * @return the request path
	 */
	virtual sp<EString> getRequestPath() = 0;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EHTTPREQUEST_HH_ */
