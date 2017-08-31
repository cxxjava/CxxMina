/*
 * EProtocolDecoderAdapter.hh
 *
 *  Created on: 2016-5-23
 *      Author: cxxjava@163.com
 */

#ifndef EPROTOCOLDECODERADAPTER_HH_
#define EPROTOCOLDECODERADAPTER_HH_

#include "./EProtocolDecoder.hh"

namespace efc {
namespace eio {

/**
 * An abstract {@link ProtocolDecoder} implementation for those who don't need
 * {@link ProtocolDecoder#finishDecode(IoSession, ProtocolDecoderOutput)} nor
 * {@link ProtocolDecoder#dispose(IoSession)} method.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

abstract class EProtocolDecoderAdapter: virtual public EProtocolDecoder {
public:
	/**
	 * Override this method to deal with the closed connection.
	 * The default implementation does nothing.
	 */
	virtual void finishDecode(sp<EIoSession>& session, sp<EProtocolDecoderOutput>& out) THROWS(EException) {
		// Do nothing
	}

	/**
	 * Override this method to dispose all resources related with this decoder.
	 * The default implementation does nothing.
	 */
	virtual void dispose(sp<EIoSession>& session) THROWS(EException) {
		// Do nothing
	}
};

} /* namespace eio */
} /* namespace efc */
#endif /* EPROTOCOLDECODERADAPTER_HH_ */
