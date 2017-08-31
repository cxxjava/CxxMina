/*
 * EDefaultDatagramSessionConfig.hh
 *
 *  Created on: 2016-8-30
 *      Author: cxxjava@163.com
 */

#ifndef EDEFAULTDATAGRAMSESSIONCONFIG_HH_
#define EDEFAULTDATAGRAMSESSIONCONFIG_HH_

#include "./EAbstractDatagramSessionConfig.hh"

namespace efc {
namespace eio {

/**
 * A default implementation of {@link DatagramSessionConfig}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EDefaultDatagramSessionConfig: public EAbstractDatagramSessionConfig {
public:
	/**
	 * Creates a new instance.
	 */
	EDefaultDatagramSessionConfig(): broadcast(DEFAULT_BROADCAST),
		reuseAddress(DEFAULT_REUSE_ADDRESS),
		receiveBufferSize(DEFAULT_RECEIVE_BUFFER_SIZE),
		sendBufferSize(DEFAULT_SEND_BUFFER_SIZE),
		trafficClass(DEFAULT_TRAFFIC_CLASS) {
	}

	/**
	 * @see DatagramSocket#getBroadcast()
	 */
	virtual boolean isBroadcast() {
		return broadcast;
	}

	/**
	 * @see DatagramSocket#setBroadcast(boolean)
	 */
	virtual void setBroadcast(boolean broadcast) {
		this->broadcast = broadcast;
	}

	/**
	 * @see DatagramSocket#getReuseAddress()
	 */
	virtual boolean isReuseAddress() {
		return reuseAddress;
	}

	/**
	 * @see DatagramSocket#setReuseAddress(boolean)
	 */
	virtual void setReuseAddress(boolean reuseAddress) {
		this->reuseAddress = reuseAddress;
	}

	/**
	 * @see DatagramSocket#getReceiveBufferSize()
	 */
	virtual int getReceiveBufferSize() {
		return receiveBufferSize;
	}

	/**
	 * @see DatagramSocket#setReceiveBufferSize(int)
	 */
	virtual void setReceiveBufferSize(int receiveBufferSize) {
		this->receiveBufferSize = receiveBufferSize;
	}

	/**
	 * @see DatagramSocket#getSendBufferSize()
	 */
	virtual int getSendBufferSize() {
		return sendBufferSize;
	}

	/**
	 * @see DatagramSocket#setSendBufferSize(int)
	 */
	virtual void setSendBufferSize(int sendBufferSize) {
		this->sendBufferSize = sendBufferSize;
	}

	/**
	 * @see DatagramSocket#getTrafficClass()
	 */
	virtual int getTrafficClass() {
		return trafficClass;
	}

	/**
	 * @see DatagramSocket#setTrafficClass(int)
	 */
	virtual void setTrafficClass(int trafficClass) {
		this->trafficClass = trafficClass;
	}

protected:
	virtual boolean isBroadcastChanged() {
		return broadcast != DEFAULT_BROADCAST;
	}

	virtual boolean isReceiveBufferSizeChanged() {
		return receiveBufferSize != DEFAULT_RECEIVE_BUFFER_SIZE;
	}

	virtual boolean isReuseAddressChanged() {
		return reuseAddress != DEFAULT_REUSE_ADDRESS;
	}

	virtual boolean isSendBufferSizeChanged() {
		return sendBufferSize != DEFAULT_SEND_BUFFER_SIZE;
	}

	virtual boolean isTrafficClassChanged() {
		return trafficClass != DEFAULT_TRAFFIC_CLASS;
	}

private:
	static const boolean DEFAULT_BROADCAST = false;

    static const boolean DEFAULT_REUSE_ADDRESS = false;

    /* The SO_RCVBUF parameter. Set to -1 (ie, will default to OS default) */
    static const int DEFAULT_RECEIVE_BUFFER_SIZE = -1;

    /* The SO_SNDBUF parameter. Set to -1 (ie, will default to OS default) */
    static const int DEFAULT_SEND_BUFFER_SIZE = -1;

    static const int DEFAULT_TRAFFIC_CLASS = 0;

    boolean broadcast;// = DEFAULT_BROADCAST;

    boolean reuseAddress;// = DEFAULT_REUSE_ADDRESS;

    int receiveBufferSize;// = DEFAULT_RECEIVE_BUFFER_SIZE;

    int sendBufferSize;// = DEFAULT_SEND_BUFFER_SIZE;

    int trafficClass;// = DEFAULT_TRAFFIC_CLASS;

};

} /* namespace eio */
} /* namespace efc */
#endif /* EDEFAULTDATAGRAMSESSIONCONFIG_HH_ */
