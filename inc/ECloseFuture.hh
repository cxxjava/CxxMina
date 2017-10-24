/*
 * ECloseFuture.hh
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#ifndef ECLOSEFUTURE_HH_
#define ECLOSEFUTURE_HH_

#include "./EIoFuture.hh"
#include "./EIoFutureListener.hh"
#include "Efc.hh"

namespace efc {
namespace eio {

/**
 * An {@link IoFuture} for asynchronous close requests.
 *
 * <h3>Example</h3>
 * <pre>
 * IoSession session = ...;
 * CloseFuture future = session.close(true);
 * // Wait until the connection is closed
 * future.awaitUninterruptibly();
 * // Now connection should be closed.
 * assert future.isClosed();
 * </pre>
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */
interface ECloseFuture : virtual public EIoFuture {
	virtual ~ECloseFuture() {
	}

	/**
	 * Returns <tt>true</tt> if the close request is finished and the session is closed.
	 */
	virtual boolean isClosed() = 0;

	/**
	 * Marks this future as closed and notifies all threads waiting for this
	 * future.  This method is invoked by MINA internally.  Please do not call
	 * this method directly.
	 */
	virtual void setClosed() = 0;
};

} // namespace efc
} // namespace eio

#endif /* ECLOSEFUTURE_HH_ */
