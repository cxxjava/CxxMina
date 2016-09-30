/*
 * EProxyRequest.hh
 *
 *  Created on: 2016-6-8
 *      Author: cxxjava@163.com
 */

#ifndef EPROXYREQUEST_HH_
#define EPROXYREQUEST_HH_

#include "Efc.hh"

namespace efc {
namespace eio {

/**
 * ProxyRequest.java - Abstract wrapper class for proxy requests.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since MINA 2.0.0-M3
 */

abstract class EProxyRequest: public EObject {
private:
	/**
	 * The address of the request endpoint.
	 */
	EInetSocketAddress* endpointAddress;// = null;

public:
	virtual ~EProxyRequest() {
		delete endpointAddress;
	}

	/**
	 * Implicit constructor.
	 */
	EProxyRequest(): endpointAddress(null) {
		// Do nothing
	}

	/**
	 * Basic constructor of a {@link ProxyRequest} that only sets the
	 * address of the request endpoint.
	 *
	 * @param endpointAddress the address of the request endpoint.
	 */
	EProxyRequest(EInetSocketAddress* endpointAddress) {
		if (endpointAddress) {
			this->endpointAddress = new EInetSocketAddress(*endpointAddress);
		}
	}

	/**
	 * @return the address of the request endpoint
	 */
	EInetSocketAddress* getEndpointAddress() {
		return endpointAddress;
	}
};

} /* namespace eio */
} /* namespace efc */
#endif /* EPROXYREQUEST_HH_ */
