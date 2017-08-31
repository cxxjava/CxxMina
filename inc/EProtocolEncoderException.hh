/*
 * EProtocolEncoderException.hh
 *
 *  Created on: 2016-5-20
 *      Author: cxxjava@163.com
 */

#ifndef EPROTOCOLENCODEREXCEPTION_HH_
#define EPROTOCOLENCODEREXCEPTION_HH_

#include "./EProtocolCodecException.hh"

namespace efc {
namespace eio {

#define EPROTOCOLENCODEREXCEPTION       EProtocolEncoderException(__FILE__, __LINE__, errno)
#define EPROTOCOLENCODEREXCEPTIONS(msg) EProtocolEncoderException(__FILE__, __LINE__, msg)

/**
 * An exception that is thrown when {@link ProtocolEncoder}
 * cannot understand or failed to validate the specified message object.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EProtocolEncoderException: public EProtocolCodecException {
public:
	/**
	 * Constructs an <code>EProtocolEncoderException</code> with no specified detail message.
	 *
	 * @param   _file_   __FILE__
	 * @param   _line_   __LINE__
	 * @param   errn     errno
	 */
	EProtocolEncoderException(const char *_file_, int _line_, int errn = 0) :
		EProtocolCodecException(_file_, _line_, errn) {
	}

	/**
	 * Constructs an <code>EProtocolEncoderException</code> with the
	 * specified detail message.
	 *
	 * @param   _file_   __FILE__.
	 * @param   _line_   __LINE__.
	 * @param   s   the detail message.
	 */
	EProtocolEncoderException(const char *_file_, int _line_, const char *s, int errn = 0) :
		EProtocolCodecException(_file_, _line_, s, errn) {
	}

	/**
	 * Constructs an <code>EProtocolEncoderException</code> with the specified detail message.
	 *
	 * @param   _file_   __FILE__
	 * @param   _line_   __LINE__
	 * @param   cause    the cause (which is saved for later retrieval by the
	 *         {@link #getCause()} method).  (A {@code null} value is
	 *         permitted, and indicates that the cause is nonexistent or
	 *         unknown.)
	 */
	EProtocolEncoderException(const char *_file_, int _line_, EThrowable* cause) :
		EProtocolCodecException(_file_, _line_, cause) {
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
	EProtocolEncoderException(const char *_file_, int _line_, const char *s, EThrowable* cause) :
		EProtocolCodecException(_file_, _line_, s, cause) {
	}
};

} /* namespace eio */
} /* namespace efc */
#endif /* EPROTOCOLENCODEREXCEPTION_HH_ */
