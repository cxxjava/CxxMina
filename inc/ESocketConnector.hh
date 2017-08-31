/*
 * ESocketConnector.hh
 *
 *  Created on: 2016-3-22
 *      Author: cxxjava@163.com
 */

#ifndef ESOCKETCONNECTOR_HH_
#define ESOCKETCONNECTOR_HH_

#include "./EIoConnector.hh"
#include "./ESocketSessionConfig.hh"

namespace efc {
namespace eio {

/**
 * {@link IoConnector} for socket transport (TCP/IP).
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

interface ESocketConnector: public virtual EIoConnector {
	/**
	 * @return the default remote InetSocketAddress to connect to when no argument
	 * is specified in {@link #connect()} method.
	 * This method overrides the {@link IoConnector#getDefaultRemoteAddress()} method.
	 */
	virtual EInetSocketAddress* getDefaultRemoteAddress() = 0;

	/**
	 * @return the default configuration of the new SocketSessions created by
	 * this connect service.
	 */
	virtual ESocketSessionConfig* getSessionConfig() = 0;

	/**
	 * Sets the default remote InetSocketAddress to connect to when no argument is
	 * specified in {@link #connect()} method.
	 * This method overrides the {@link IoConnector#setDefaultRemoteAddress(java.net.SocketAddress)} method.
	 *
	 * @param remoteAddress The remote address to set
	 */
	virtual void setDefaultRemoteAddress(EInetSocketAddress* remoteAddress) = 0;
};

} /* namespace eio */
} /* namespace efc */
#endif /* ESOCKETCONNECTOR_HH_ */
