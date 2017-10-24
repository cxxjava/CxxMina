/*
 * EWriteException.hh
 *
 *  Created on: 2016-1-18
 *      Author: cxxjava@163.com
 */

#ifndef EWRITEEXCEPTION_HH_
#define EWRITEEXCEPTION_HH_

#include "Efc.hh"

namespace efc {
namespace eio {

#define EWRITEEXCEPTION       EWriteException(__FILE__, __LINE__, errno)
#define EWRITEEXCEPTIONS(msg) EWriteException(__FILE__, __LINE__, msg)

/**
 * An exception which is thrown when one or more write operations were failed.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EWriteException: public EIOException {
public:
	/**
	 * Constructs an <code>EWriteException</code> with no
	 * detail message.
	 *
	 * @param   _file_   __FILE__
	 * @param   _line_   __LINE__
	 * @param   errn     errno
	 */
	EWriteException(const char *_file_, int _line_, int errn = 0) :
			EIOException(_file_, _line_, errn) {
	}

	/**
	 * Constructs an <code>EWriteException</code> with the
	 * specified detail message.
	 *
	 * @param   _file_   __FILE__.
	 * @param   _line_   __LINE__.
	 * @param   s   the detail message.
	 */
	EWriteException(const char *_file_, int _line_, const char *s, int errn = 0) :
		EIOException(_file_, _line_, s, errn) {
	}

	/**
	 * Creates a new exception.
	 */
	EWriteException(const char *_file_, int _line_, sp<EWriteRequest> request) :
			EIOException(_file_, _line_), r(request) {
	}

	/**
	 * Creates a new exception.
	 *
	 * @param request The associated {@link WriteRequest}
	 * @param message The detail message
	 */
	EWriteException(const char *_file_, int _line_, sp<EWriteRequest> request, const char *s) :
			EIOException(_file_, _line_, s), r(request) {
	}

	/**
	 * Constructs an <code>EWriteException</code> with the specified detail message.
	 *
	 * @param   _file_   __FILE__
	 * @param   _line_   __LINE__
     * @param request The associated {@link WriteRequest}
	 * @param   cause    the cause (which is saved for later retrieval by the
	 *         {@link #getCause()} method).  (A {@code null} value is
	 *         permitted, and indicates that the cause is nonexistent or
	 *         unknown.)
	 */
	EWriteException(const char *_file_, int _line_, sp<EWriteRequest> request, EThrowable* cause) :
			EIOException(_file_, _line_, cause), r(request) {
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
	EWriteException(const char *_file_, int _line_, sp<EWriteRequest> request, const char *s, EThrowable* cause) :
			EIOException(_file_, _line_, s, cause), r(request) {
	}

	/**
	 * Returns the firstly failed {@link WriteRequest}.
	 */
	sp<EWriteRequest> getRequest() {
		return r.lock();
	}

private:
	wp<EWriteRequest> r;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EWRITEEXCEPTION_HH_ */
