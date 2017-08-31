/*
 * ESocketAcceptor.hh
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#ifndef ESOCKETACCEPTOR_HH_
#define ESOCKETACCEPTOR_HH_

#include "./EIoAcceptor.hh"

namespace efc {
namespace eio {

/**
 * {@link IoAcceptor} for socket transport (TCP/IP).  This class
 * handles incoming TCP/IP based socket connections.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */
interface ESocketAcceptor: virtual public EIoAcceptor {
	virtual ~ESocketAcceptor() {
	}

	/**
	 * @see ServerSocket#getReuseAddress()
	 */
	virtual boolean isReuseAddress() = 0;

	/**
	 * @see ServerSocket#setReuseAddress(boolean)
	 */
	virtual void setReuseAddress(boolean reuseAddress) = 0;

	/**
	 * Returns the size of the backlog.
	 */
	virtual int getBacklog() = 0;

	/**
	 * Sets the size of the backlog.  This can only be done when this
	 * class is not bound
	 */
	virtual void setBacklog(int backlog) = 0;
};

}
} // namespace efc::eio

#endif /* ESOCKETACCEPTOR_HH_ */
