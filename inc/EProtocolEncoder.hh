/*
 * EProtocolEncoder.hh
 *
 *  Created on: 2016-5-20
 *      Author: cxxjava@163.com
 */

#ifndef EPROTOCOLENCODER_HH_
#define EPROTOCOLENCODER_HH_

#include "./EIoSession.hh"
#include "./EProtocolEncoderOutput.hh"

namespace efc {
namespace eio {

/**
 * Encodes higher-level message objects into binary or protocol-specific data.
 * MINA invokes {@link #encode(IoSession, Object, ProtocolEncoderOutput)}
 * method with message which is popped from the session write queue, and then
 * the encoder implementation puts encoded messages (typically {@link IoBuffer}s)
 * into {@link ProtocolEncoderOutput} by calling {@link ProtocolEncoderOutput#write(Object)}.
 * <p>
 * Please refer to
 * <a href="../../../../../xref-examples/org/apache/mina/examples/reverser/TextLineEncoder.html"><code>TextLineEncoder</code></a>
 * example.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 *
 * @see ProtocolEncoderException
 */

interface EProtocolEncoder: virtual public EObject {
	virtual ~EProtocolEncoder(){}

	/**
	 * Encodes higher-level message objects into binary or protocol-specific data.
	 * MINA invokes {@link #encode(IoSession, Object, ProtocolEncoderOutput)}
	 * method with message which is popped from the session write queue, and then
	 * the encoder implementation puts encoded messages (typically {@link IoBuffer}s)
	 * into {@link ProtocolEncoderOutput}.
	 *
	 * @param session The current Session
	 * @param message the message to encode
	 * @param out The {@link ProtocolEncoderOutput} that will receive the encoded message
	 * @throws Exception if the message violated protocol specification
	 */
	virtual void encode(sp<EIoSession>& session, sp<EObject>& message, sp<EProtocolEncoderOutput>& out) THROWS(EException) = 0;

	/**
	 * Releases all resources related with this encoder.
	 *
	 * @param session The current Session
	 * @throws Exception if failed to dispose all resources
	 */
	virtual void dispose(sp<EIoSession>& session) THROWS(EException) = 0;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EPROTOCOLENCODER_HH_ */
