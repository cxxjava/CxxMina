/*
 * EWriteTimeoutException.hh
 *
 *  Created on: 2016-1-18
 *      Author: cxxjava@163.com
 */

#ifndef EWRITETIMEOUTEXCEPTION_HH_
#define EWRITETIMEOUTEXCEPTION_HH_

#include "./EWriteException.hh"

namespace efc {
namespace eio {

#define EWRITETIMEOUTEXCEPTION       EWriteTimeoutException(__FILE__, __LINE__, errno)
#define EWRITETIMEOUTEXCEPTIONS(msg) EWriteTimeoutException(__FILE__, __LINE__, msg)

/**
 * An exception which is thrown when write buffer is not flushed for
 * {@link IoSessionConfig#getWriteTimeout()} seconds.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EWriteTimeoutException: public EWriteException {
public:
	/**
	 * Constructs an <code>EWriteTimeoutException</code> with no
	 * detail message.
	 *
	 * @param   _file_   __FILE__
	 * @param   _line_   __LINE__
	 * @param   errn     errno
	 */
	EWriteTimeoutException(const char *_file_, int _line_, int errn = 0) :
			EWriteException(_file_, _line_, errn) {
	}

	/**
	 * Constructs an <code>EWriteTimeoutException</code> with the
	 * specified detail message.
	 *
	 * @param   _file_   __FILE__.
	 * @param   _line_   __LINE__.
	 * @param   s   the detail message.
	 */
	EWriteTimeoutException(const char *_file_, int _line_, const char *s, int errn = 0) :
		EWriteException(_file_, _line_, s, errn) {
	}

	/**
	 * Creates a new exception.
	 */
	EWriteTimeoutException(const char *_file_, int _line_, sp<EWriteRequest> request) :
		EWriteException(_file_, _line_, request) {
	}

	/**
	 * Creates a new exception.
	 *
	 * @param request The associated {@link WriteRequest}
	 * @param message The detail message
	 */
	EWriteTimeoutException(const char *_file_, int _line_, sp<EWriteRequest> request, const char *s) :
		EWriteException(_file_, _line_, request, s) {
	}

	/**
	 * Constructs an <code>EWriteTimeoutException</code> with the specified detail message.
	 *
	 * @param   _file_   __FILE__
	 * @param   _line_   __LINE__
	 * @param request The associated {@link WriteRequest}
	 * @param   cause    the cause (which is saved for later retrieval by the
	 *         {@link #getCause()} method).  (A {@code null} value is
	 *         permitted, and indicates that the cause is nonexistent or
	 *         unknown.)
	 */
	EWriteTimeoutException(const char *_file_, int _line_, sp<EWriteRequest> request, EThrowable* cause) :
		EWriteException(_file_, _line_, request, cause) {
	}

	/**
	 * Constructs a new exception with the specified detail message and
	 * cause.
	 *
	 * @param   _file_   __FILE__
	 * @param   _line_   __LINE__
	 * @param request The associated {@link WriteRequest}
	 * @param   s   the detail message.
	 * @param   cause    the cause (which is saved for later retrieval by the
	 *         {@link #getCause()} method).  (A {@code null} value is
	 *         permitted, and indicates that the cause is nonexistent or
	 *         unknown.)
	 */
	EWriteTimeoutException(const char *_file_, int _line_, sp<EWriteRequest> request, const char *s, EThrowable* cause) :
		EWriteException(_file_, _line_, request, s, cause) {
	}
};

} /* namespace eio */
} /* namespace efc */
#endif /* EWRITETIMEOUTEXCEPTION_HH_ */
