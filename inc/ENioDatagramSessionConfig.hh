/*
 * ENioDatagramSessionConfig.hh
 *
 *  Created on: 2016-8-30
 *      Author: cxxjava@163.com
 */

#ifndef ENIODATAGRAMSESSIONCONFIG_HH_
#define ENIODATAGRAMSESSIONCONFIG_HH_

#include "./EAbstractDatagramSessionConfig.hh"

namespace efc {
namespace eio {

/**
 * Define the configuration for a Datagram based session.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class ENioDatagramSessionConfig: public EAbstractDatagramSessionConfig {
public:
	/**
	 * Creates a new instance of NioDatagramSessionConfig, associated
	 * with the given DatagramChannel.
	 *
	 * @param channel The associated DatagramChannel
	 */
	ENioDatagramSessionConfig(sp<EDatagramChannel> channel);

	/**
	 * Get the Socket receive buffer size for this DatagramChannel.
	 *
	 * @return the DatagramChannel receive buffer size.
	 * @throws RuntimeIoException if the socket is closed or if we
	 * had a SocketException
	 *
	 * @see DatagramSocket#getReceiveBufferSize()
	 */
	virtual int getReceiveBufferSize();

	/**
	 * Set the Socket receive buffer size for this DatagramChannel. <br>
	 * <br>
	 * Note : The underlying Socket may not accept the new buffer's size.
	 * The user has to check that the new value has been set.
	 *
	 * @param receiveBufferSize the DatagramChannel receive buffer size.
	 * @throws RuntimeIoException if the socket is closed or if we
	 * had a SocketException
	 *
	 * @see DatagramSocket#setReceiveBufferSize()
	 */
	virtual void setReceiveBufferSize(int receiveBufferSize);

	/**
	 * Tells if SO_BROADCAST is enabled.
	 *
	 * @return <tt>true</tt> if SO_BROADCAST is enabled
	 * @throws RuntimeIoException If the socket is closed or if we get an
	 * {@link SocketException}
	 */
	virtual boolean isBroadcast();

	virtual void setBroadcast(boolean broadcast);

	/**
	 *
	 * @throws RuntimeIoException If the socket is closed or if we get an
	 * {@link SocketException}
	 */
	virtual int getSendBufferSize();

	/**
	 *
	 * @throws RuntimeIoException If the socket is closed or if we get an
	 * {@link SocketException}
	 */
	virtual void setSendBufferSize(int sendBufferSize);

	/**
	 * Tells if SO_REUSEADDR is enabled.
	 *
	 * @return <tt>true</tt> if SO_REUSEADDR is enabled
	 * @throws RuntimeIoException If the socket is closed or if we get an
	 * {@link SocketException}
	 */
	virtual boolean isReuseAddress();

	/**
	 *
	 * @throws RuntimeIoException If the socket is closed or if we get an
	 * {@link SocketException}
	 */
	virtual void setReuseAddress(boolean reuseAddress);

	/**
	 * Get the current Traffic Class for this Socket, if any. As this is
	 * not a mandatory feature, the returned value should be considered as
	 * a hint.
	 *
	 * @return The Traffic Class supported by this Socket
	 * @throws RuntimeIoException If the socket is closed or if we get an
	 * {@link SocketException}
	 */
	virtual int getTrafficClass();

	/**
	 * {@inheritDoc}
	 * @throws RuntimeIoException If the socket is closed or if we get an
	 * {@link SocketException}
	 */
	virtual void setTrafficClass(int trafficClass);

private:
	/** The associated channel */
	sp<EDatagramChannel> channel;
};

} /* namespace eio */
} /* namespace efc */
#endif /* ENIODATAGRAMSESSIONCONFIG_HH_ */
