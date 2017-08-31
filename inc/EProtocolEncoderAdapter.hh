/*
 * EProtocolEncoderAdapter.hh
 *
 *  Created on: 2016-5-23
 *      Author: cxxjava@163.com
 */

#ifndef EPROTOCOLENCODERADAPTER_HH_
#define EPROTOCOLENCODERADAPTER_HH_

#include "./EProtocolEncoder.hh"

namespace efc {
namespace eio {

/**
 * An abstract {@link ProtocolEncoder} implementation for those who don't have any
 * resources to dispose.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

abstract class EProtocolEncoderAdapter: virtual public EProtocolEncoder {
public:
	/**
	 * Override this method dispose all resources related with this encoder.
	 * The default implementation does nothing.
	 */
	virtual void dispose(sp<EIoSession>& session) THROWS(EException) {
		// Do nothing
	}
};

} /* namespace eio */
} /* namespace efc */
#endif /* EPROTOCOLENCODERADAPTER_HH_ */
