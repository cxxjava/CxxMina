/*
 * EWriteToClosedSessionException.hh
 *
 *  Created on: 2016-1-18
 *      Author: cxxjava@163.com
 */

#ifndef EWRITETOCLOSEDSESSIONEXCEPTION_HH_
#define EWRITETOCLOSEDSESSIONEXCEPTION_HH_

#include "./EWriteException.hh"

namespace efc {
namespace eio {

#define EWRITETOCLOSEDSESSIONEXCEPTION       EWriteToClosedSessionException(__FILE__, __LINE__, errno)
#define EWRITETOCLOSEDSESSIONEXCEPTIONS(msg) EWriteToClosedSessionException(__FILE__, __LINE__, msg)


/**
 * An exception which is thrown when one or more write operations were
 * attempted on a closed session.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EWriteToClosedSessionException: public EWriteException {
public:
	/**
	 * Constructs an <code>EWriteToClosedSessionException</code> with no
	 * detail message.
	 *
	 * @param   _file_   __FILE__
	 * @param   _line_   __LINE__
	 * @param   errn     errno
	 */
	EWriteToClosedSessionException(const char *_file_, int _line_, int errn = 0) :
			EWriteException(_file_, _line_, errn) {
	}

	/**
	 * Constructs an <code>EWriteToClosedSessionException</code> with the
	 * specified detail message.
	 *
	 * @param   _file_   __FILE__.
	 * @param   _line_   __LINE__.
	 * @param   s   the detail message.
	 */
	EWriteToClosedSessionException(const char *_file_, int _line_, const char *s, int errn = 0) :
		EWriteException(_file_, _line_, s, errn) {
	}

	/**
	 * Creates a new exception.
	 */
	EWriteToClosedSessionException(const char *_file_, int _line_, sp<EWriteRequest> request) :
		EWriteException(_file_, _line_, request) {
	}

	/**
	 * Creates a new exception.
	 *
	 * @param request The associated {@link WriteRequest}
	 * @param message The detail message
	 */
	EWriteToClosedSessionException(const char *_file_, int _line_, sp<EWriteRequest> request, const char *s) :
		EWriteException(_file_, _line_, request, s) {
	}

	/**
	 * Constructs an <code>EWriteToClosedSessionException</code> with the specified detail message.
	 *
	 * @param   _file_   __FILE__
	 * @param   _line_   __LINE__
	 * @param request The associated {@link WriteRequest}
	 * @param   cause    the cause (which is saved for later retrieval by the
	 *         {@link #getCause()} method).  (A {@code null} value is
	 *         permitted, and indicates that the cause is nonexistent or
	 *         unknown.)
	 */
	EWriteToClosedSessionException(const char *_file_, int _line_, sp<EWriteRequest> request, EThrowable* cause) :
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
	EWriteToClosedSessionException(const char *_file_, int _line_, sp<EWriteRequest> request, const char *s, EThrowable* cause) :
		EWriteException(_file_, _line_, request, s, cause) {
	}
};

} /* namespace eio */
} /* namespace efc */
#endif /* EWRITETOCLOSEDSESSIONEXCEPTION_HH_ */
