/*
 * EAbstractDatagramSessionConfig.hh
 *
 *  Created on: 2016-8-30
 *      Author: cxxjava@163.com
 */

#ifndef EABSTRACTDATAGRAMSESSIONCONFIG_HH_
#define EABSTRACTDATAGRAMSESSIONCONFIG_HH_

#include "./EAbstractIoSessionConfig.hh"
#include "./EDatagramSessionConfig.hh"

namespace efc {
namespace eio {

/**
 * The Datagram transport session configuration.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

abstract class EAbstractDatagramSessionConfig: public EAbstractIoSessionConfig,
		virtual public EDatagramSessionConfig {
public:
	/**
	 * {@inheritDoc}
	 */
	virtual boolean isCloseOnPortUnreachable();

	/**
	 * {@inheritDoc}
	 */
	virtual void setCloseOnPortUnreachable(boolean closeOnPortUnreachable);

protected:
	EAbstractDatagramSessionConfig();

	/**
	 * {@inheritDoc}
	 */
	virtual void doSetAll(EIoSessionConfig* config);

	/**
	 * @return <tt>true</tt> if and only if the <tt>broadcast</tt> property
	 * has been changed by its setter method.  The system call related with
	 * the property is made only when this method returns <tt>true</tt>.  By
	 * default, this method always returns <tt>true</tt> to simplify implementation
	 * of subclasses, but overriding the default behavior is always encouraged.
	 */
	virtual boolean isBroadcastChanged();

	/**
	 * @return <tt>true</tt> if and only if the <tt>receiveBufferSize</tt> property
	 * has been changed by its setter method.  The system call related with
	 * the property is made only when this method returns <tt>true</tt>.  By
	 * default, this method always returns <tt>true</tt> to simplify implementation
	 * of subclasses, but overriding the default behavior is always encouraged.
	 */
	virtual boolean isReceiveBufferSizeChanged();

	/**
	 * @return <tt>true</tt> if and only if the <tt>reuseAddress</tt> property
	 * has been changed by its setter method.  The system call related with
	 * the property is made only when this method returns <tt>true</tt>.  By
	 * default, this method always returns <tt>true</tt> to simplify implementation
	 * of subclasses, but overriding the default behavior is always encouraged.
	 */
	virtual boolean isReuseAddressChanged();

	/**
	 * @return <tt>true</tt> if and only if the <tt>sendBufferSize</tt> property
	 * has been changed by its setter method.  The system call related with
	 * the property is made only when this method returns <tt>true</tt>.  By
	 * default, this method always returns <tt>true</tt> to simplify implementation
	 * of subclasses, but overriding the default behavior is always encouraged.
	 */
	virtual boolean isSendBufferSizeChanged();

	/**
	 * @return <tt>true</tt> if and only if the <tt>trafficClass</tt> property
	 * has been changed by its setter method.  The system call related with
	 * the property is made only when this method returns <tt>true</tt>.  By
	 * default, this method always returns <tt>true</tt> to simplify implementation
	 * of subclasses, but overriding the default behavior is always encouraged.
	 */
	virtual boolean isTrafficClassChanged();

private:
	static const boolean DEFAULT_CLOSE_ON_PORT_UNREACHABLE = true;

	/** Tells if we should close the session if the port is unreachable. Default to true */
	boolean closeOnPortUnreachable;// = DEFAULT_CLOSE_ON_PORT_UNREACHABLE;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EABSTRACTDATAGRAMSESSIONCONFIG_HH_ */
