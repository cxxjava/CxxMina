/*
 * EProtocolCodecException.hh
 *
 *  Created on: 2016-5-20
 *      Author: cxxjava@163.com
 */

#ifndef EPROTOCOLCODECEXCEPTION_HH_
#define EPROTOCOLCODECEXCEPTION_HH_

#include "Efc.hh"

namespace efc {
namespace eio {

#define EPROTOCOLCODECEXCEPTION       EProtocolCodecException(__FILE__, __LINE__, errno)
#define EPROTOCOLCODECEXCEPTIONS(msg) EProtocolCodecException(__FILE__, __LINE__, msg)

/**
 * An exception that is thrown when {@link ProtocolEncoder} or
 * {@link ProtocolDecoder} cannot understand or failed to validate
 * data to process.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EProtocolCodecException: public EException {
public:
	/**
	 * Constructs an <code>EProtocolCodecException</code> with no specified detail message.
	 *
	 * @param   _file_   __FILE__
	 * @param   _line_   __LINE__
	 * @param   errn     errno
	 */
	EProtocolCodecException(const char *_file_, int _line_, int errn = 0) :
		EException(_file_, _line_, errn) {
	}

	/**
	 * Constructs an <code>EProtocolCodecException</code> with the
	 * specified detail message.
	 *
	 * @param   _file_   __FILE__.
	 * @param   _line_   __LINE__.
	 * @param   s   the detail message.
	 */
	EProtocolCodecException(const char *_file_, int _line_, const char *s, int errn = 0) :
			EException(_file_, _line_, s, errn) {
	}

	/**
	 * Constructs an <code>EProtocolCodecException</code> with the specified detail message.
	 *
	 * @param   _file_   __FILE__
	 * @param   _line_   __LINE__
	 * @param   cause    the cause (which is saved for later retrieval by the
	 *         {@link #getCause()} method).  (A {@code null} value is
	 *         permitted, and indicates that the cause is nonexistent or
	 *         unknown.)
	 */
	EProtocolCodecException(const char *_file_, int _line_, EThrowable* cause) :
			EException(_file_, _line_, cause) {
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
	EProtocolCodecException(const char *_file_, int _line_, const char *s, EThrowable* cause) :
			EException(_file_, _line_, s, cause) {
	}
};

} /* namespace eio */
} /* namespace efc */
#endif /* EPROTOCOLCODECEXCEPTION_HH_ */
