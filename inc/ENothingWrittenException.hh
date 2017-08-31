/*
 * ENothingWrittenException.hh
 *
 *  Created on: 2016-5-20
 *      Author: cxxjava@163.com
 */

#ifndef ENOTHINGWRITTENEXCEPTION_HH_
#define ENOTHINGWRITTENEXCEPTION_HH_

#include "./EWriteException.hh"

namespace efc {
namespace eio {

#define ENOTHINGWRITTENEXCEPTION       ENothingWrittenException(__FILE__, __LINE__, errno)
#define ENOTHINGWRITTENEXCEPTIONS(msg) ENothingWrittenException(__FILE__, __LINE__, msg)

/**
 * An exception which is thrown when one or more write requests resulted
 * in no actual write operation.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class ENothingWrittenException: public EWriteException {
public:
	/**
	 * Constructs an <code>ENothingWrittenException</code> with no
	 * detail message.
	 *
	 * @param   _file_   __FILE__
	 * @param   _line_   __LINE__
	 * @param   errn     errno
	 */
	ENothingWrittenException(const char *_file_, int _line_, int errn = 0) :
		EWriteException(_file_, _line_, errn) {
	}

	/**
	 * Constructs an <code>ENothingWrittenException</code> with the
	 * specified detail message.
	 *
	 * @param   _file_   __FILE__.
	 * @param   _line_   __LINE__.
	 * @param   s   the detail message.
	 */
	ENothingWrittenException(const char *_file_, int _line_, const char *s, int errn = 0) :
		EWriteException(_file_, _line_, s, errn) {
	}

	/**
	 * Creates a new exception.
	 */
	ENothingWrittenException(const char *_file_, int _line_, sp<EWriteRequest> request) :
		EWriteException(_file_, _line_, request) {
	}

	/**
	 * Creates a new exception.
	 *
	 * @param request The associated {@link WriteRequest}
	 * @param message The detail message
	 */
	ENothingWrittenException(const char *_file_, int _line_, sp<EWriteRequest> request, const char *s) :
		EWriteException(_file_, _line_, request, s) {
	}

	/**
	 * Constructs an <code>ENothingWrittenException</code> with the specified detail message.
	 *
	 * @param   _file_   __FILE__
	 * @param   _line_   __LINE__
	 * @param request The associated {@link WriteRequest}
	 * @param   cause    the cause (which is saved for later retrieval by the
	 *         {@link #getCause()} method).  (A {@code null} value is
	 *         permitted, and indicates that the cause is nonexistent or
	 *         unknown.)
	 */
	ENothingWrittenException(const char *_file_, int _line_, sp<EWriteRequest> request, EThrowable* cause) :
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
	ENothingWrittenException(const char *_file_, int _line_, sp<EWriteRequest> request, const char *s, EThrowable* cause) :
		EWriteException(_file_, _line_, request, s, cause) {
	}
};

} /* namespace eio */
} /* namespace efc */
#endif /* ENOTHINGWRITTENEXCEPTION_HH_ */
