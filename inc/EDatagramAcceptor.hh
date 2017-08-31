/*
 * EDatagramAcceptor.hh
 *
 *  Created on: 2016-8-29
 *      Author: cxxjava@163.com
 */

#ifndef EDATAGRAMACCEPTOR_HH_
#define EDATAGRAMACCEPTOR_HH_

#include "./EIoAcceptor.hh"
#include "./EIoSessionRecycler.hh"
#include "./EDatagramSessionConfig.hh"

namespace efc {
namespace eio {

/**
 * {@link IoAcceptor} for datagram transport (UDP/IP).
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

interface EDatagramAcceptor: virtual public EIoAcceptor {
	virtual ~EDatagramAcceptor() {
	}

	/**
	 * @return the local InetSocketAddress which is bound currently.  If more than one
	 * address are bound, only one of them will be returned, but it's not
	 * necessarily the firstly bound address.
	 * This method overrides the {@link IoAcceptor#getLocalAddress()} method.
	 */
	virtual EInetSocketAddress* getLocalAddress() = 0;

	/**
	 * @return a {@link Set} of the local InetSocketAddress which are bound currently.
	 * This method overrides the {@link IoAcceptor#getDefaultLocalAddress()} method.
	 */
	virtual EInetSocketAddress* getDefaultLocalAddress() = 0;

	/**
	 * Sets the default local InetSocketAddress to bind when no argument is specified in
	 * {@link #bind()} method. Please note that the default will not be used
	 * if any local InetSocketAddress is specified.
	 * This method overrides the {@link IoAcceptor#setDefaultLocalAddress(java.net.SocketAddress)} method.
	 *
	 * @param localAddress The local address
	 */
	virtual void setDefaultLocalAddress(EInetSocketAddress* localAddress) = 0;

	/**
	 * @return the {@link IoSessionRecycler} for this service.
	 */
	virtual EIoSessionRecycler* getSessionRecycler() = 0;

	/**
	 * Sets the {@link IoSessionRecycler} for this service.
	 *
	 * @param sessionRecycler <tt>null</tt> to use the default recycler
	 */
	virtual void setSessionRecycler(EIoSessionRecycler* sessionRecycler) = 0;

	/**
	 * @return the default Datagram configuration of the new {@link IoSession}s
	 * created by this service.
	 */
	virtual EDatagramSessionConfig* getSessionConfig() = 0;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EDATAGRAMACCEPTOR_HH_ */
