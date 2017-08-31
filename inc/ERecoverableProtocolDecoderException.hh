/*
 * ERecoverableProtocolDecoderException.hh
 *
 *  Created on: 2016-5-21
 *      Author: cxxjava@163.com
 */

#ifndef ERECOVERABLEPROTOCOLDECODEREXCEPTION_HH_
#define ERECOVERABLEPROTOCOLDECODEREXCEPTION_HH_

#include "./EProtocolDecoderException.hh"

namespace efc {
namespace eio {

#define ERECOVERABLEPROTOCOLDECODEREXCEPTION       ERecoverableProtocolDecoderException(__FILE__, __LINE__, errno)
#define ERECOVERABLEPROTOCOLDECODEREXCEPTIONS(msg) ERecoverableProtocolDecoderException(__FILE__, __LINE__, msg)

/**
 * A special exception that tells the {@link ProtocolDecoder} can keep
 * decoding even after this exception is thrown.
 * <p>
 * Once {@link ProtocolCodecFilter} catches any other type of exception
 * than {@link RecoverableProtocolDecoderException}, it stops calling
 * the {@link ProtocolDecoder#decode(org.apache.mina.core.session.IoSession,
 *        org.apache.mina.core.buffer.IoBuffer, ProtocolDecoderOutput)}
 * immediately and fires an <tt>exceptionCaught</tt> event.
 * <p>
 * On the other hand, if {@link RecoverableProtocolDecoderException} is thrown,
 * it doesn't stop immediately but keeps calling the {@link ProtocolDecoder}
 * as long as the position of the read buffer changes.
 * <p>
 * {@link RecoverableProtocolDecoderException} is useful for a robust
 * {@link ProtocolDecoder} that can continue decoding even after any
 * protocol violation.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class ERecoverableProtocolDecoderException: public EProtocolDecoderException {
public:
	/**
	 * Constructs an <code>ERecoverableProtocolDecoderException</code> with no specified detail message.
	 *
	 * @param   _file_   __FILE__
	 * @param   _line_   __LINE__
	 * @param   errn     errno
	 */
	ERecoverableProtocolDecoderException(const char *_file_, int _line_, int errn = 0) :
		EProtocolDecoderException(_file_, _line_, errn) {
	}

	/**
	 * Constructs an <code>ERecoverableProtocolDecoderException</code> with the
	 * specified detail message.
	 *
	 * @param   _file_   __FILE__.
	 * @param   _line_   __LINE__.
	 * @param   s   the detail message.
	 */
	ERecoverableProtocolDecoderException(const char *_file_, int _line_, const char *s, int errn = 0) :
		EProtocolDecoderException(_file_, _line_, s, errn) {
	}

	/**
	 * Constructs an <code>ERecoverableProtocolDecoderException</code> with the specified detail message.
	 *
	 * @param   _file_   __FILE__
	 * @param   _line_   __LINE__
	 * @param   cause    the cause (which is saved for later retrieval by the
	 *         {@link #getCause()} method).  (A {@code null} value is
	 *         permitted, and indicates that the cause is nonexistent or
	 *         unknown.)
	 */
	ERecoverableProtocolDecoderException(const char *_file_, int _line_, EThrowable* cause) :
		EProtocolDecoderException(_file_, _line_, cause) {
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
	ERecoverableProtocolDecoderException(const char *_file_, int _line_, const char *s, EThrowable* cause) :
		EProtocolDecoderException(_file_, _line_, s, cause) {
	}
};

} /* namespace eio */
} /* namespace efc */
#endif /* ERECOVERABLEPROTOCOLDECODEREXCEPTION_HH_ */
