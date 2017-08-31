/*
 * EHttpMessage.hh
 *
 *  Created on: 2017-1-4
 *      Author: cxxjava@163.com
 */

#ifndef EHTTPMESSAGE_HH_
#define EHTTPMESSAGE_HH_

#include "./EHttpVersion.hh"

namespace efc {
namespace eio {

/**
 * An HTTP message, the ancestor of HTTP request &amp; response.
 *
 * @author The Apache MINA Project (dev@mina.apache.org)
 */

interface EHttpMessage: virtual public EObject {
	virtual ~EHttpMessage(){}

	/**
	 * The HTTP version of the message
	 *
	 * @return HTTP/1.0 or HTTP/1.1
	 */
	virtual const char* getProtocolVersion() = 0;

	/**
	 * Gets the <tt>Content-Type</tt> header of the message.
	 *
	 * @return The content type.
	 */
	virtual sp<EString> getContentType() = 0;

	/**
	 * @return <tt>true</tt> if this message enables keep-alive connection.
	 */
	virtual boolean isKeepAlive() = 0;

	/**
	 * Returns the value of the HTTP header with the specified name. If more than one header with the given name is
	 * associated with this request, one is selected and returned.
	 *
	 * @param name The name of the desired header
	 * @return The header value - or null if no header is found with the specified name
	 */
	virtual sp<EString> getHeader(const char* name) = 0;

	/**
	 * @param name the Header's name we are looking for
	 * @return <tt>true</tt> if the HTTP header with the specified name exists in this request.
	 */
	virtual boolean containsHeader(const char* name) = 0;

	/**
	 * @return a read-only {@link Map} of HTTP headers whose key is a {@link String} and whose value is a {@link String}
	 * s.
	 */
	virtual sp<EMap<EString*, EString*> > getHeaders() = 0;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EHTTPMESSAGE_HH_ */
