/*
 * ETransportMetadata.hh
 *
 *  Created on: 2016-3-14
 *      Author: cxxjava@163.com
 */

#ifndef ETRANSPORTMETADATA_HH_
#define ETRANSPORTMETADATA_HH_

#include "Efc.hh"

namespace efc {
namespace eio {

/**
 * Provides meta-information that describes an {@link IoService}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

interface ETransportMetadata: virtual public EObject {
	virtual ~ETransportMetadata() {
	}

	 /**
	 * @return the name of the service provider (e.g. "nio", "apr" and "rxtx").
	 */
	virtual const char* getProviderName() = 0;

	/**
	 * @return the name of the service.
	 */
	virtual const char* getName() = 0;

	/**
	 * @return <tt>true</tt> if the session of this transport type is
	 * <a href="http://en.wikipedia.org/wiki/Connectionless">connectionless</a>.
	 */
	virtual boolean isConnectionless() = 0;

	/**
	 * @return {@code true} if the messages exchanged by the service can be
	 * <a href="http://en.wikipedia.org/wiki/IPv4#Fragmentation_and_reassembly">fragmented
	 * or reassembled</a> by its underlying transport.
	 */
	virtual boolean hasFragmentation() = 0;
};

} /* namespace eio */
} /* namespace efc */
#endif /* ETRANSPORTMETADATA_HH_ */
