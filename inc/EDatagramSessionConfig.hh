/*
 * EDatagramSessionConfig.hh
 *
 *  Created on: 2016-8-30
 *      Author: cxxjava@163.com
 */

#ifndef EDATAGRAMSESSIONCONFIG_HH_
#define EDATAGRAMSESSIONCONFIG_HH_

#include "./EIoSessionConfig.hh"

namespace efc {
namespace eio {

/**
 * An {@link IoSessionConfig} for datagram transport type.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

interface EDatagramSessionConfig: virtual public EIoSessionConfig {
	virtual ~EDatagramSessionConfig() {
	}

	/**
	 * @see DatagramSocket#getBroadcast()
	 *
	 * @return <tt>true</tt> if SO_BROADCAST is enabled.
	 */
	virtual boolean isBroadcast() = 0;

	/**
	 * @see DatagramSocket#setBroadcast(boolean)
	 *
	 * @param broadcast Tells if SO_BROACAST is enabled or not
	 */
	virtual void setBroadcast(boolean broadcast) = 0;

	/**
	 * @see DatagramSocket#getReuseAddress()
	 *
	 * @return <tt>true</tt> if SO_REUSEADDR is enabled.
	 */
	virtual boolean isReuseAddress() = 0;

	/**
	 * @see DatagramSocket#setReuseAddress(boolean)
	 *
	 * @param reuseAddress Tells if SO_REUSEADDR is enabled or disabled
	 */
	virtual void setReuseAddress(boolean reuseAddress) = 0;

	/**
	 * @see DatagramSocket#getReceiveBufferSize()
	 *
	 * @return the size of the receive buffer
	 */
	virtual int getReceiveBufferSize() = 0;

	/**
	 * @see DatagramSocket#setReceiveBufferSize(int)
	 *
	 * @param receiveBufferSize The size of the receive buffer
	 */
	virtual void setReceiveBufferSize(int receiveBufferSize) = 0;

	/**
	 * @see DatagramSocket#getSendBufferSize()
	 *
	 * @return the size of the send buffer
	 */
	virtual int getSendBufferSize() = 0;

	/**
	 * @see DatagramSocket#setSendBufferSize(int)
	 *
	 * @param sendBufferSize The size of the send buffer
	 */
	virtual void setSendBufferSize(int sendBufferSize) = 0;

	/**
	 * @see DatagramSocket#getTrafficClass()
	 *
	 * @return the traffic class
	 */
	virtual int getTrafficClass() = 0;

	/**
	 * @see DatagramSocket#setTrafficClass(int)
	 *
	 * @param trafficClass The traffic class to set, one of IPTOS_LOWCOST (0x02)
	 * IPTOS_RELIABILITY (0x04), IPTOS_THROUGHPUT (0x08) or IPTOS_LOWDELAY (0x10)
	 */
	virtual void setTrafficClass(int trafficClass) = 0;

	/**
	 * If method returns true, it means session should be closed when a
	 * {@link PortUnreachableException} occurs.
	 *
	 * @return Tells if we should close if the port is unreachable
	 */
	virtual boolean isCloseOnPortUnreachable() = 0;

	/**
	 * Sets if the session should be closed if an {@link PortUnreachableException}
	 * occurs.
	 *
	 * @param closeOnPortUnreachable <tt>true</tt> if we should close if the port is unreachable
	 */
	virtual void setCloseOnPortUnreachable(boolean closeOnPortUnreachable) = 0;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EDATAGRAMSESSIONCONFIG_HH_ */
