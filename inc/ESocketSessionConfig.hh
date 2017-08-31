/*
 * ESocketSessionConfig.hh
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#ifndef ESOCKETSESSIONCONFIG_HH_
#define ESOCKETSESSIONCONFIG_HH_

#include "./EIoSessionConfig.hh"

namespace efc {
namespace eio {

/**
 * An {@link IoSessionConfig} for socket transport type.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */
interface ESocketSessionConfig : virtual public EIoSessionConfig
{
public:
	virtual ~ESocketSessionConfig(){
	}

    /**
     * @see Socket#getReuseAddress()
     */
	virtual boolean isReuseAddress() = 0;

    /**
     * @see Socket#setReuseAddress(boolean)
     */
	virtual void setReuseAddress(boolean reuseAddress) = 0;

    /**
     * @see Socket#getReceiveBufferSize()
     */
	virtual int getReceiveBufferSize() = 0;

    /**
     * @see Socket#setReceiveBufferSize(int)
     */
	virtual void setReceiveBufferSize(int receiveBufferSize) = 0;

    /**
     * @see Socket#getSendBufferSize()
     */
	virtual int getSendBufferSize() = 0;

    /**
     * @see Socket#setSendBufferSize(int)
     */
	virtual void setSendBufferSize(int sendBufferSize) = 0;

    /**
     * @see Socket#getTrafficClass()
     */
	virtual int getTrafficClass() = 0;

    /**
     * @see Socket#setTrafficClass(int)
     */
	virtual void setTrafficClass(int trafficClass) = 0;

    /**
     * @see Socket#getKeepAlive()
     */
	virtual boolean isKeepAlive() = 0;

    /**
     * @see Socket#setKeepAlive(boolean)
     */
	virtual void setKeepAlive(boolean keepAlive) = 0;

    /**
     * @see Socket#getOOBInline()
     */
	virtual boolean isOobInline() = 0;

    /**
     * @see Socket#setOOBInline(boolean)
     */
	virtual void setOobInline(boolean oobInline) = 0;

    /**
     * Please note that enabling <tt>SO_LINGER</tt> in Java NIO can result
     * in platform-dependent behavior and unexpected blocking of I/O thread.
     *
     * @see Socket#getSoLinger()
     * @see <a href="http://bugs.sun.com/bugdatabase/view_bug.do?bug_id=6179351">Sun Bug Database</a>
     */
	virtual int getSoLinger() = 0;

    /**
     * Please note that enabling <tt>SO_LINGER</tt> in Java NIO can result
     * in platform-dependent behavior and unexpected blocking of I/O thread.
     *
     * @param soLinger Please specify a negative value to disable <tt>SO_LINGER</tt>.
     *
     * @see Socket#setSoLinger(boolean, int)
     * @see <a href="http://bugs.sun.com/bugdatabase/view_bug.do?bug_id=6179351">Sun Bug Database</a>
     */
	virtual void setSoLinger(int soLinger) = 0;

    /**
     * @see Socket#getTcpNoDelay()
     */
	virtual boolean isTcpNoDelay() = 0;

    /**
     * @see Socket#setTcpNoDelay(boolean)
     */
	virtual void setTcpNoDelay(boolean tcpNoDelay) = 0;
};

} /* namespace eio */
} /* namespace efc */

#endif /* EIOSESSIONCONFIG_HH_ */
