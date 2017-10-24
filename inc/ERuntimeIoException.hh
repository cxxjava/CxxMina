/*
 * ERuntimeIoException.hh
 *
 *  Created on: 2013-3-14
 *      Author: cxxjava@163.com
 */

#ifndef ERUNTIMEIOEXCEPTION_H_
#define ERUNTIMEIOEXCEPTION_H_

#include "Efc.hh"

namespace efc {
namespace eio {

#define ERUNTIMEIOEXCEPTION        ERuntimeIoException(__FILE__, __LINE__, errno)
#define ERUNTIMEIOEXCEPTIONS(msg)  ERuntimeIoException(__FILE__, __LINE__, msg)

/**
 * A unchecked version of {@link IOException}.
 * <p>
 * Please note that {@link RuntimeIoException} is different from
 * {@link IOException} in that doesn't trigger force session close,
 * while {@link IOException} forces disconnection.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class ERuntimeIoException: public ERuntimeException {
public:
	/**
	 * Constructs an <code>ERuntimeIoException</code> with no
	 * detail message.
	 *
	 * @param   _file_   __FILE__
	 * @param   _line_   __LINE__
	 * @param   errn     errno
	 */
	ERuntimeIoException(const char *_file_, int _line_, int errn = 0) :
		ERuntimeException(_file_, _line_, errn) {
	}

	/**
	 * Constructs an <code>ERuntimeIoException</code> with the
	 * specified detail message.
	 *
	 * @param   _file_   __FILE__.
	 * @param   _line_   __LINE__.
	 * @param   s   the detail message.
	 */
	ERuntimeIoException(const char *_file_, int _line_, const char *s, int errn = 0) :
		ERuntimeException(_file_, _line_, s, errn) {
	}

	/**
	 * Constructs an <code>ERuntimeIoException</code> with the specified detail message.
	 *
	 * @param   _file_   __FILE__
	 * @param   _line_   __LINE__
	 * @param   cause    the cause (which is saved for later retrieval by the
	 *         {@link #getCause()} method).  (A {@code null} value is
	 *         permitted, and indicates that the cause is nonexistent or
	 *         unknown.)
	 */
	ERuntimeIoException(const char *_file_, int _line_, EThrowable* cause) :
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
	ERuntimeIoException(const char *_file_, int _line_, const char *s, EThrowable* cause) :
		ERuntimeException(_file_, _line_, s, cause) {
	}
};

}
} // namespace efc::eio
#endif /* ERUNTIMEIOEXCEPTION_H_ */
