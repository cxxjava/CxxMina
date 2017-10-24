/*
 * EHttpException.hh
 *
 *  Created on: 2016-1-19
 *      Author: cxxjava@163.com
 */

#ifndef EHTTPEXCEPTION_HH_
#define EHTTPEXCEPTION_HH_

#include "Efc.hh"

namespace efc {
namespace eio {

#define EHTTPEXCEPTION        EHttpException(__FILE__, __LINE__, errno)
#define EHTTPEXCEPTIONS(msg)  EHttpException(__FILE__, __LINE__, msg)

/**
 *
 */

class EHttpException: public ERuntimeException {
public:
	/**
	 * Constructs an <code>EHttpException</code> with no
	 * detail message.
	 *
	 * @param   _file_   __FILE__
	 * @param   _line_   __LINE__
	 * @param   errn     errno
	 */
	EHttpException(const char *_file_, int _line_, int statusCode, int errn = 0) :
		ERuntimeException(_file_, _line_, errn) {
		this->statusCode = statusCode;
	}

	/**
	 * Constructs an <code>EHttpException</code> with the
	 * specified detail message.
	 *
	 * @param   _file_   __FILE__.
	 * @param   _line_   __LINE__.
	 * @param   s   the detail message.
	 */
	EHttpException(const char *_file_, int _line_, int statusCode, const char *s, int errn = 0) :
		ERuntimeException(_file_, _line_, s, errn) {
		this->statusCode = statusCode;
	}

	/**
	 * Constructs an <code>EHttpException</code> with the specified detail message.
	 *
	 * @param   _file_   __FILE__
	 * @param   _line_   __LINE__
	 * @param   cause    the cause (which is saved for later retrieval by the
	 *         {@link #getCause()} method).  (A {@code null} value is
	 *         permitted, and indicates that the cause is nonexistent or
	 *         unknown.)
	 */
	EHttpException(const char *_file_, int _line_, int statusCode, EThrowable* cause) :
		ERuntimeException(_file_, _line_, cause) {
		this->statusCode = statusCode;
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
	EHttpException(const char *_file_, int _line_, int statusCode, const char *s, EThrowable* cause) :
		ERuntimeException(_file_, _line_, s, cause) {
		this->statusCode = statusCode;
	}

	/**
	 *
	 */
	int getStatusCode() {
		return statusCode;
	}

private:
	int statusCode;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EHTTPEXCEPTION_HH_ */
