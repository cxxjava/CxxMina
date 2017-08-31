/*
 * EProtocolEncoderOutput.hh
 *
 *  Created on: 2016-5-20
 *      Author: cxxjava@163.com
 */

#ifndef EPROTOCOLENCODEROUTPUT_HH_
#define EPROTOCOLENCODEROUTPUT_HH_

#include "./EWriteFuture.hh"

namespace efc {
namespace eio {

/**
 * Callback for {@link ProtocolEncoder} to generate encoded messages such as
 * {@link IoBuffer}s.  {@link ProtocolEncoder} must call {@link #write(Object)}
 * for each encoded message.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

interface EProtocolEncoderOutput: virtual public EObject {
	virtual ~EProtocolEncoderOutput(){}

	/**
	 * Callback for {@link ProtocolEncoder} to generate an encoded message such
	 * as an {@link IoBuffer}. {@link ProtocolEncoder} must call
	 * {@link #write(Object)} for each encoded message.
	 *
	 * @param encodedMessage the encoded message, typically an {@link IoBuffer}
	 *                       or a {@link FileRegion}.
	 */
	virtual void write(sp<EObject> encodedMessage) = 0;

//	/**
//	 * Merges all buffers you wrote via {@link #write(Object)} into
//	 * one {@link IoBuffer} and replaces the old fragmented ones with it.
//	 * This method is useful when you want to control the way MINA generates
//	 * network packets.  Please note that this method only works when you
//	 * called {@link #write(Object)} method with only {@link IoBuffer}s.
//	 *
//	 * @throws IllegalStateException if you wrote something else than {@link IoBuffer}
//	 */
//	virtual void mergeAll() = 0;

	/**
	 * Flushes all buffers you wrote via {@link #write(Object)} to
	 * the session.  This operation is asynchronous; please wait for
	 * the returned {@link WriteFuture} if you want to wait for
	 * the buffers flushed.
	 *
	 * @return <tt>null</tt> if there is nothing to flush at all.
	 */
	virtual sp<EWriteFuture> flush() = 0;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EPROTOCOLENCODEROUTPUT_HH_ */
