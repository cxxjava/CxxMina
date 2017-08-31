/*
 * EDatagramConnector.hh
 *
 *  Created on: 2016-8-30
 *      Author: cxxjava@163.com
 */

#ifndef EDATAGRAMCONNECTOR_HH_
#define EDATAGRAMCONNECTOR_HH_

#include "./EIoConnector.hh"
#include "./EDatagramSessionConfig.hh"

namespace efc {
namespace eio {

/**
 * {@link IoConnector} for datagram transport (UDP/IP).
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

interface EDatagramConnector: virtual public EIoConnector {
	virtual ~EDatagramConnector(){
	}

	/**
	 * @return the default configuration of the new FatagramSessions created by
	 * this connect service.
	 */
	virtual EDatagramSessionConfig* getSessionConfig() = 0;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EDATAGRAMCONNECTOR_HH_ */
