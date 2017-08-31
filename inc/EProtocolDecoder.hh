/*
 * EProtocolDecoder.hh
 *
 *  Created on: 2016-5-20
 *      Author: cxxjava@163.com
 */

#ifndef EPROTOCOLDECODER_HH_
#define EPROTOCOLDECODER_HH_

#include "./EIoSession.hh"
#include "./EIoBuffer.hh"
#include "./EProtocolDecoderOutput.hh"

namespace efc {
namespace eio {

/**
 * Decodes binary or protocol-specific data into higher-level message objects.
 * MINA invokes {@link #decode(IoSession, IoBuffer, ProtocolDecoderOutput)}
 * method with read data, and then the decoder implementation puts decoded
 * messages into {@link ProtocolDecoderOutput} by calling
 * {@link ProtocolDecoderOutput#write(Object)}.
 * <p>
 * Please refer to
 * <a href="../../../../../xref-examples/org/apache/mina/examples/reverser/TextLineDecoder.html"><code>TextLineDecoder</code></a>
 * example.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 *
 * @see ProtocolDecoderException
 */

interface EProtocolDecoder: virtual public EObject {
	virtual ~EProtocolDecoder(){}

	/**
	 * Decodes binary or protocol-specific content into higher-level message objects.
	 * MINA invokes {@link #decode(IoSession, IoBuffer, ProtocolDecoderOutput)}
	 * method with read data, and then the decoder implementation puts decoded
	 * messages into {@link ProtocolDecoderOutput}.
	 *
	 * @param session The current Session
	 * @param in the buffer to decode
	 * @param out The {@link ProtocolDecoderOutput} that will receive the decoded message
	 * @throws Exception if the read data violated protocol specification
	 */
	virtual void decode(sp<EIoSession>& session, EIoBuffer* in, sp<EProtocolDecoderOutput>& out) THROWS(EException) = 0;

	/**
	 * Invoked when the specified <tt>session</tt> is closed.  This method is useful
	 * when you deal with the protocol which doesn't specify the length of a message
	 * such as HTTP response without <tt>content-length</tt> header. Implement this
	 * method to process the remaining data that {@link #decode(IoSession, IoBuffer, ProtocolDecoderOutput)}
	 * method didn't process completely.
	 *
	 * @param session The current Session
	 * @param out The {@link ProtocolDecoderOutput} that contains the decoded message
	 * @throws Exception if the read data violated protocol specification
	 */
	virtual void finishDecode(sp<EIoSession>& session, sp<EProtocolDecoderOutput>& out) THROWS(EException) = 0;

	/**
	 * Releases all resources related with this decoder.
	 *
	 * @param session The current Session
	 * @throws Exception if failed to dispose all resources
	 */
	virtual void dispose(sp<EIoSession>& session) THROWS(EException) = 0;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EPROTOCOLDECODER_HH_ */
