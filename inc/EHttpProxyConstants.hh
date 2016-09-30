/*
 * EHttpProxyConstants.hh
 *
 *  Created on: 2016-6-8
 *      Author: cxxjava@163.com
 */

#ifndef EHTTPPROXYCONSTANTS_HH_
#define EHTTPPROXYCONSTANTS_HH_

namespace efc {
namespace eio {

/**
 * HttpProxyConstants.java - HTTP Proxy constants.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since MINA 2.0.0-M3
 */

/**
 * The HTTP CONNECT verb.
 */
#define EHttpProxyConstants_CONNECT "CONNECT"

/**
 * The HTTP GET verb.
 */
#define EHttpProxyConstants_GET "GET"

/**
 * The HTTP PUT verb.
 */
#define EHttpProxyConstants_PUT "PUT"

/**
 * The HTTP 1.0 protocol version string.
 */
#define EHttpProxyConstants_HTTP_1_0 "HTTP/1.0"

/**
 * The HTTP 1.1 protocol version string.
 */
#define EHttpProxyConstants_HTTP_1_1 "HTTP/1.1"

/**
 * The CRLF character sequence used in HTTP protocol to end each line.
 */
#define EHttpProxyConstants_CRLF "\r\n"

/**
 * The default keep-alive timeout we set to make proxy
 * connection persistent. Set to 300 ms.
 */
#define EHttpProxyConstants_DEFAULT_KEEP_ALIVE_TIME "300"

// ProxyRequest properties

/**
 * The username property. Used in auth mechs.
 */
#define EHttpProxyConstants_USER_PROPERTY "USER"

/**
 * The password property. Used in auth mechs.
 */
#define EHttpProxyConstants_PWD_PROPERTY "PWD"

/**
 * The domain name property. Used in auth mechs.
 */
#define EHttpProxyConstants_DOMAIN_PROPERTY "DOMAIN"

/**
 * The workstation name property. Used in auth mechs.
 */
#define EHttpProxyConstants_WORKSTATION_PROPERTY "WORKSTATION"

} /* namespace eio */
} /* namespace efc */

#endif /* EHTTPPROXYCONSTANTS_HH_ */
