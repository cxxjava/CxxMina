/*
 * EProtocolDecoderException.hh
 *
 *  Created on: 2016-5-20
 *      Author: cxxjava@163.com
 */

#ifndef EPROTOCOLDECODEREXCEPTION_HH_
#define EPROTOCOLDECODEREXCEPTION_HH_

#include "./EProtocolCodecException.hh"

namespace efc {
namespace eio {

#define EPROTOCOLDECODEREXCEPTION       EProtocolDecoderException(__FILE__, __LINE__, errno)
#define EPROTOCOLDECODEREXCEPTIONS(msg) EProtocolDecoderException(__FILE__, __LINE__, msg)

/**
 * An exception that is thrown when {@link ProtocolDecoder}
 * cannot understand or failed to validate the specified {@link IoBuffer}
 * content.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EProtocolDecoderException: public EProtocolCodecException {
public:
	virtual ~EProtocolDecoderException();

	EProtocolDecoderException(const EException& that);
	EProtocolDecoderException(const EProtocolDecoderException& that);

	/**
	 * Constructs an <code>EProtocolDecoderException</code> with no specified detail message.
	 *
	 * @param   _file_   __FILE__
	 * @param   _line_   __LINE__
	 * @param   errn     errno
	 */
	EProtocolDecoderException(const char *_file_, int _line_, int errn = 0);

	/**
	 * Constructs an <code>EProtocolDecoderException</code> with the specified detail message.
	 *
	 * @param   _file_   __FILE__
	 * @param   _line_   __LINE__
	 * @param   s   the detail message.
	 */
	EProtocolDecoderException(const char *_file_, int _line_, const char *s, int errn = 0);

    /**
     * Constructs an <code>EProtocolDecoderException</code> with the specified detail message.
     *
     * @param   _file_   __FILE__
     * @param   _line_   __LINE__
     * @param   cause    the cause (which is saved for later retrieval by the
     *         {@link #getCause()} method).  (A {@code null} value is
     *         permitted, and indicates that the cause is nonexistent or
     *         unknown.)
     */
    EProtocolDecoderException(const char *_file_, int _line_, EThrowable* cause);
    
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
    EProtocolDecoderException(const char *_file_, int _line_, const char *s, EThrowable* cause);
    
	/**
	 * @return the hexdump of the unknown message part.
	 */
	const char* getHexdump();

	/**
	 * Sets the hexdump of the unknown message part.
	 *
	 * @param hexdump The hexadecimal String representation of the message
	 */
	void setHexdump(const char* hexdump);

	/**
	 * @return the message and the hexdump of the unknown part.
	 */
	virtual const char* getMessage();

private:
	EString* hexDump;
	EString* message;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EPROTOCOLDECODEREXCEPTION_HH_ */
