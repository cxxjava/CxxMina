/*
 * EIoSessionInitializationException.hh
 *
 *  Created on: 2013-3-14
 *      Author: cxxjava@163.com
 */

#ifndef EIOSESSIONINITIALIZATIONEXCEPTION_H_
#define EIOSESSIONINITIALIZATIONEXCEPTION_H_

#include "Efc.hh"

namespace efc {
namespace eio {

#define EIOSESSIONINITIALIZATIONEXCEPTION        EIoSessionInitializationException(__FILE__, __LINE__, errno)
#define EIOSESSIONINITIALIZATIONEXCEPTIONS(msg)  EIoSessionInitializationException(__FILE__, __LINE__, msg)

/**
 * A {@link RuntimeException} that is thrown when the initialization of
 * an {@link IoSession} fails.
 * 
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EIoSessionInitializationException: public ERuntimeException {
public:
	/**
	 * Constructs an <code>EIoSessionInitializationException</code> with no
	 * detail message.
	 *
	 * @param   _file_   __FILE__
	 * @param   _line_   __LINE__
	 * @param   errn     errno
	 */
	EIoSessionInitializationException(const char *_file_, int _line_, int errn = 0) :
		ERuntimeException(_file_, _line_, errn) {
	}

	/**
	 * Constructs an <code>EIoSessionInitializationException</code> with the
	 * specified detail message.
	 *
	 * @param   _file_   __FILE__.
	 * @param   _line_   __LINE__.
	 * @param   s   the detail message.
	 */
	EIoSessionInitializationException(const char *_file_, int _line_, const char *s, int errn = 0) :
		ERuntimeException(_file_, _line_, s, errn) {
	}

	/**
	 * Constructs an <code>EIoSessionInitializationException</code> with the specified detail message.
	 *
	 * @param   _file_   __FILE__
	 * @param   _line_   __LINE__
	 * @param   cause    the cause (which is saved for later retrieval by the
	 *         {@link #getCause()} method).  (A {@code null} value is
	 *         permitted, and indicates that the cause is nonexistent or
	 *         unknown.)
	 */
	EIoSessionInitializationException(const char *_file_, int _line_, EThrowable* cause) :
		ERuntimeException(_file_, _line_, cause) {
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
	EIoSessionInitializationException(const char *_file_, int _line_, const char *s, EThrowable* cause) :
		ERuntimeException(_file_, _line_, s, cause) {
	}
};

}
} // namespace efc::eio
#endif /* EIOSESSIONINITIALIZATIONEXCEPTION_H_ */
