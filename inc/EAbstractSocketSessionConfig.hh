/*
 * EAbstractSocketSessionConfig.hh
 *
 *  Created on: 2016-1-19
 *      Author: cxxjava@163.com
 */

#ifndef EABSTRACTSOCKETSESSIONCONFIG_HH_
#define EABSTRACTSOCKETSESSIONCONFIG_HH_

#include "./EAbstractIoSessionConfig.hh"
#include "./ESocketSessionConfig.hh"

namespace efc {
namespace eio {

/**
 * The TCP transport session configuration.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

abstract class EAbstractSocketSessionConfig: public EAbstractIoSessionConfig,
		virtual public ESocketSessionConfig {
protected:
	EAbstractSocketSessionConfig() {
        // Do nothing
    }

	virtual void doSetAll(EIoSessionConfig* config);

	/**
	 * Returns <tt>true</tt> if and only if the <tt>keepAlive</tt> property
	 * has been changed by its setter method.  The system call related with
	 * the property is made only when this method returns <tt>true</tt>.  By
	 * default, this method always returns <tt>true</tt> to simplify implementation
	 * of subclasses, but overriding the default behavior is always encouraged.
	 */
	virtual boolean isKeepAliveChanged() {
        return true;
    }

	/**
	 * Returns <tt>true</tt> if and only if the <tt>oobInline</tt> property
	 * has been changed by its setter method.  The system call related with
	 * the property is made only when this method returns <tt>true</tt>.  By
	 * default, this method always returns <tt>true</tt> to simplify implementation
	 * of subclasses, but overriding the default behavior is always encouraged.
	 */
	virtual boolean isOobInlineChanged() {
		return true;
	}

	/**
	 * Returns <tt>true</tt> if and only if the <tt>receiveBufferSize</tt> property
	 * has been changed by its setter method.  The system call related with
	 * the property is made only when this method returns <tt>true</tt>.  By
	 * default, this method always returns <tt>true</tt> to simplify implementation
	 * of subclasses, but overriding the default behavior is always encouraged.
	 */
	virtual boolean isReceiveBufferSizeChanged() {
		return true;
	}

	/**
	 * Returns <tt>true</tt> if and only if the <tt>reuseAddress</tt> property
	 * has been changed by its setter method.  The system call related with
	 * the property is made only when this method returns <tt>true</tt>.  By
	 * default, this method always returns <tt>true</tt> to simplify implementation
	 * of subclasses, but overriding the default behavior is always encouraged.
	 */
	virtual boolean isReuseAddressChanged() {
		return true;
	}

	/**
	 * Returns <tt>true</tt> if and only if the <tt>sendBufferSize</tt> property
	 * has been changed by its setter method.  The system call related with
	 * the property is made only when this method returns <tt>true</tt>.  By
	 * default, this method always returns <tt>true</tt> to simplify implementation
	 * of subclasses, but overriding the default behavior is always encouraged.
	 */
	virtual boolean isSendBufferSizeChanged() {
		return true;
	}

	/**
	 * Returns <tt>true</tt> if and only if the <tt>soLinger</tt> property
	 * has been changed by its setter method.  The system call related with
	 * the property is made only when this method returns <tt>true</tt>.  By
	 * default, this method always returns <tt>true</tt> to simplify implementation
	 * of subclasses, but overriding the default behavior is always encouraged.
	 */
	virtual boolean isSoLingerChanged() {
		return true;
	}

	/**
	 * Returns <tt>true</tt> if and only if the <tt>tcpNoDelay</tt> property
	 * has been changed by its setter method.  The system call related with
	 * the property is made only when this method returns <tt>true</tt>.  By
	 * default, this method always returns <tt>true</tt> to simplify implementation
	 * of subclasses, but overriding the default behavior is always encouraged.
	 */
	virtual boolean isTcpNoDelayChanged() {
		return true;
	}

	/**
	 * Returns <tt>true</tt> if and only if the <tt>trafficClass</tt> property
	 * has been changed by its setter method.  The system call related with
	 * the property is made only when this method returns <tt>true</tt>.  By
	 * default, this method always returns <tt>true</tt> to simplify implementation
	 * of subclasses, but overriding the default behavior is always encouraged.
	 */
	virtual boolean isTrafficClassChanged() {
        return true;
    }
};

} /* namespace eio */
} /* namespace efc */
#endif /* EABSTRACTSOCKETSESSIONCONFIG_HH_ */
