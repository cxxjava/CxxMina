/*
 * EProxyAuthException.hh
 *
 *  Created on: 2013-3-14
 *      Author: cxxjava@163.com
 */

#ifndef EPROXYAUTHEXCEPTION_HH_
#define EPROXYAUTHEXCEPTION_HH_

#include "Efc.hh"

namespace efc {
namespace eio {

#define EPROXYAUTHEXCEPTION       EProxyAuthException(__FILE__, __LINE__, errno)
#define EPROXYAUTHEXCEPTIONS(msg) EProxyAuthException(__FILE__, __LINE__, msg)

/**
 * ProxyAuthException.java - This class extends {@link SaslException} and represents an
 * authentication failure to the proxy.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since MINA 2.0.0-M3
 */

class EProxyAuthException: public ESaslException {
public:
	/**
	 * Constructs an <code>EProxyAuthException</code> with no
	 * detail message.
	 *
	 * @param   _file_   __FILE__
	 * @param   _line_   __LINE__
	 * @param   errn     errno
	 */
	EProxyAuthException(const char *_file_, int _line_, int errn = 0) :
		ESaslException(_file_, _line_, errn) {
	}

	/**
	 * Constructs an <code>EProxyAuthException</code> with the
	 * specified detail message.
	 *
	 * @param   _file_   __FILE__.
	 * @param   _line_   __LINE__.
	 * @param   s   the detail message.
	 */
	EProxyAuthException(const char *_file_, int _line_, const char *s, int errn = 0) :
		ESaslException(_file_, _line_, s, errn) {
	}

	/**
	 * Constructs an <code>EProxyAuthException</code> with the specified detail message.
	 *
	 * @param   _file_   __FILE__
	 * @param   _line_   __LINE__
	 * @param   cause    the cause (which is saved for later retrieval by the
	 *         {@link #getCause()} method).  (A {@code null} value is
	 *         permitted, and indicates that the cause is nonexistent or
	 *         unknown.)
	 */
	EProxyAuthException(const char *_file_, int _line_, EThrowable* cause) :
		ESaslException(_file_, _line_, cause) {
	}

	/**
	 * Constructs a new exception with the specified detail message and
	 * cause.
	 *
	 * @param   _file_   __FILE__
	 * @param   _line_   __LINE__
	 * @param   s   the detail message.
	 * @param   cause    the cause (which is saved for later retrieval by the
	 *         {@link #getCause()} method).  (A {@code null} value is
	 *         permitted, and indicates that the cause is nonexistent or
	 *         unknown.)
	 */
	EProxyAuthException(const char *_file_, int _line_, const char *s, EThrowable* cause) :
		ESaslException(_file_, _line_, s, cause) {
	}
};

} /* namespace eio */
} /* namespace efc */
#endif /* EPROXYAUTHEXCEPTION_HH_ */
