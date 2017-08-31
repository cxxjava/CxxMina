/*
 * EDefaultSocketSessionConfig.hh
 *
 *  Created on: 2016-1-19
 *      Author: cxxjava@163.com
 */

#ifndef EDEFAULTSOCKETSESSIONCONFIG_HH_
#define EDEFAULTSOCKETSESSIONCONFIG_HH_

#include "./EAbstractSocketSessionConfig.hh"
#include "./EIoService.hh"

namespace efc {
namespace eio {

class EDefaultSocketSessionConfig: public EAbstractSocketSessionConfig {
public:
	/**
	 * Creates a new instance.
	 */
	EDefaultSocketSessionConfig();

	/**
	 * Initialize this configuration.
	 *
	 * @param parent The parent IoService.
	 */
	void init(EIoService* parent);

	virtual boolean isReuseAddress();

	virtual void setReuseAddress(boolean reuseAddress);

	virtual int getReceiveBufferSize();

	virtual void setReceiveBufferSize(int receiveBufferSize);

	virtual int getSendBufferSize();

	virtual void setSendBufferSize(int sendBufferSize);

	virtual int getTrafficClass();

	virtual void setTrafficClass(int trafficClass);

	virtual boolean isKeepAlive();

	virtual void setKeepAlive(boolean keepAlive);

	virtual boolean isOobInline();

	virtual void setOobInline(boolean oobInline);

	virtual int getSoLinger();

	virtual void setSoLinger(int soLinger);

	virtual boolean isTcpNoDelay();

	virtual void setTcpNoDelay(boolean tcpNoDelay);

protected:
	virtual boolean isKeepAliveChanged();

	virtual boolean isOobInlineChanged();

	virtual boolean isReceiveBufferSizeChanged();

	virtual boolean isReuseAddressChanged();

	virtual boolean isSendBufferSizeChanged();

	virtual boolean isSoLingerChanged();

	virtual boolean isTcpNoDelayChanged();

	virtual boolean isTrafficClassChanged();

private:
	static const boolean DEFAULT_REUSE_ADDRESS = false;

    static const int DEFAULT_TRAFFIC_CLASS = 0;

    static const boolean DEFAULT_KEEP_ALIVE = false;

    static const boolean DEFAULT_OOB_INLINE = false;

    static const int DEFAULT_SO_LINGER = -1;

    static const boolean DEFAULT_TCP_NO_DELAY = false;

    EIoService* parent;

    boolean defaultReuseAddress;//= false;

    boolean reuseAddress;//= false;

    /* The SO_RCVBUF parameter. Set to -1 (ie, will default to OS default) */
    int receiveBufferSize;// = -1;

    /* The SO_SNDBUF parameter. Set to -1 (ie, will default to OS default) */
    int sendBufferSize;// = -1;

    int trafficClass;// = DEFAULT_TRAFFIC_CLASS;

    boolean keepAlive;// = DEFAULT_KEEP_ALIVE;

    boolean oobInline;// = DEFAULT_OOB_INLINE;

    int soLinger;// = DEFAULT_SO_LINGER;

    boolean tcpNoDelay;// = DEFAULT_TCP_NO_DELAY;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EDEFAULTSOCKETSESSIONCONFIG_HH_ */
