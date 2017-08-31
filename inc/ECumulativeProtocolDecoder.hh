/*
 * ECumulativeProtocolDecoder.hh
 *
 *  Created on: 2016-5-23
 *      Author: cxxjava@163.com
 */

#ifndef ECUMULATIVEPROTOCOLDECODER_HH_
#define ECUMULATIVEPROTOCOLDECODER_HH_

#include "./EProtocolDecoderAdapter.hh"
#include "./EAttributeKey.hh"

namespace efc {
namespace eio {

/**
 * A {@link ProtocolDecoder} that cumulates the content of received buffers to a
 * <em>cumulative buffer</em> to help users implement decoders.
 * <p>
 * If the received {@link IoBuffer} is only a part of a message. decoders should
 * cumulate received buffers to make a message complete or to postpone decoding
 * until more buffers arrive.
 * <p>
 * Here is an example decoder that decodes CRLF terminated lines into
 * <code>Command</code> objects:
 *
 * <pre>
 * public class CrLfTerminatedCommandLineDecoder extends CumulativeProtocolDecoder {
 *
 *     private Command parseCommand(IoBuffer in) {
 *         // Convert the bytes in the specified buffer to a
 *         // Command object.
 *         ...
 *     }
 *
 *     protected boolean doDecode(IoSession session, IoBuffer in, ProtocolDecoderOutput out) throws Exception {
 *
 *         // Remember the initial position.
 *         int start = in.position();
 *
 *         // Now find the first CRLF in the buffer.
 *         byte previous = 0;
 *         while (in.hasRemaining()) {
 *             byte current = in.get();
 *
 *             if (previous == '\r' &amp;&amp; current == '\n') {
 *                 // Remember the current position and limit.
 *                 int position = in.position();
 *                 int limit = in.limit();
 *                 try {
 *                     in.position(start);
 *                     in.limit(position);
 *                     // The bytes between in.position() and in.limit()
 *                     // now contain a full CRLF terminated line.
 *                     out.write(parseCommand(in.slice()));
 *                 } finally {
 *                     // Set the position to point right after the
 *                     // detected line and set the limit to the old
 *                     // one.
 *                     in.position(position);
 *                     in.limit(limit);
 *                 }
 *                 // Decoded one line; CumulativeProtocolDecoder will
 *                 // call me again until I return false. So just
 *                 // return true until there are no more lines in the
 *                 // buffer.
 *                 return true;
 *             }
 *
 *             previous = current;
 *         }
 *
 *         // Could not find CRLF in the buffer. Reset the initial
 *         // position to the one we recorded above.
 *         in.position(start);
 *
 *         return false;
 *     }
 * }
 * </pre>
 * <p>
 * Please note that this decoder simply forward the call to
 * {@link #doDecode(IoSession, IoBuffer, ProtocolDecoderOutput)} if the
 * underlying transport doesn't have a packet fragmentation. Whether the
 * transport has fragmentation or not is determined by querying
 * {@link TransportMetadata}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

abstract class ECumulativeProtocolDecoder: public EProtocolDecoderAdapter {
public:
	/**
	 * Cumulates content of <tt>in</tt> into internal buffer and forwards
	 * decoding request to
	 * {@link #doDecode(IoSession, IoBuffer, ProtocolDecoderOutput)}.
	 * <tt>doDecode()</tt> is invoked repeatedly until it returns <tt>false</tt>
	 * and the cumulative buffer is compacted after decoding ends.
	 *
	 * @throws IllegalStateException
	 *             if your <tt>doDecode()</tt> returned <tt>true</tt> not
	 *             consuming the cumulative buffer.
	 */
	virtual void decode(sp<EIoSession>& session, EIoBuffer* in, sp<EProtocolDecoderOutput>& out) THROWS(EException);

	/**
	 * Releases the cumulative buffer used by the specified <tt>session</tt>.
	 * Please don't forget to call <tt>super.dispose( session )</tt> when you
	 * override this method.
	 */
	virtual void dispose(sp<EIoSession>& session) THROWS(EException);

protected:
	/**
	 * Creates a new instance.
	 */
	ECumulativeProtocolDecoder();

	/**
	 *
	 */
	sp<EIoBuffer>getRemainingInSession(sp<EIoSession>& session);

	/**
	 * Let the user change the way we handle fragmentation. If set to <tt>false</tt>, the
	 * decode() method will not check the TransportMetadata fragmentation capability
	 *
	 * @param transportMetadataFragmentation The flag to set.
	 */
	void setTransportMetadataFragmentation(boolean transportMetadataFragmentation);

	/**
	 * Implement this method to consume the specified cumulative buffer and
	 * decode its content into message(s).
	 *
	 * @param session The current Session
	 * @param in the cumulative buffer
	 * @param out The {@link ProtocolDecoderOutput} that will receive the decoded message
	 * @return <tt>true</tt> if and only if there's more to decode in the buffer
	 *         and you want to have <tt>doDecode</tt> method invoked again.
	 *         Return <tt>false</tt> if remaining data is not enough to decode,
	 *         then this method will be invoked again when more data is
	 *         cumulated.
	 * @throws Exception if cannot decode <tt>in</tt>.
	 */
	virtual boolean doDecode(sp<EIoSession>& session, EIoBuffer* in, sp<EProtocolDecoderOutput>& out) THROWS(EException) = 0;

private:
	sp<EAttributeKey> BUFFER;// = new AttributeKey(getClass(), "buffer");

	/** A flag set to true if we handle fragmentation accordingly to the TransportMetadata setting.
	 * It can be set to false if needed (UDP with fragments, for instance). the default value is 'true'
	 */
	boolean transportMetadataFragmentation;// = true;

	void removeSessionBuffer(sp<EIoSession>& session);

	void storeRemainingInSession(EIoBuffer* buf, sp<EIoSession>& session);
};

} /* namespace eio */
} /* namespace efc */
#endif /* ECUMULATIVEPROTOCOLDECODER_HH_ */
