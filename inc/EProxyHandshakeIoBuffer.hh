/*
 * EProxyHandshakeIoBuffer.hh
 *
 *  Created on: 2016-7-6
 *      Author: cxxjava@lotuseed.com
 */

#ifndef EPROXYHANDSHAKEIOBUFFER_HH_
#define EPROXYHANDSHAKEIOBUFFER_HH_

#include "./EIoBufferWrapper.hh"

namespace efc {
namespace eio {

/**
 * ProxyHandshakeIoBuffer.java - {@link IoBuffer} wrapper to indicate handshake
 * related messages which should not be passed upstream of the {@link ProxyFilter}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since MINA 2.0.0-M3
 */

class EProxyHandshakeIoBuffer: public EIoBufferWrapper {
public:
	EProxyHandshakeIoBuffer(EIoBuffer* buf) : EIoBufferWrapper(buf) {
	}
};

} /* namespace eio */
} /* namespace efc */
#endif /* EPROXYHANDSHAKEIOBUFFER_HH_ */
