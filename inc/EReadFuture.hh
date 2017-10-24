/*
 * EReadFuture.hh
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#ifndef EREADFUTURE_HH_
#define EREADFUTURE_HH_

#include "./EIoFuture.hh"
#include "./EIoFutureListener.hh"
#include "Efc.hh"

namespace efc {
namespace eio {

/**
 * An {@link IoFuture} for {@link IoSession#read() asynchronous read requests}.
 *
 * <h3>Example</h3>
 * <pre>
 * IoSession session = ...;
 * // useReadOperation must be enabled to use read operation.
 * session.getConfig().setUseReadOperation(true);
 *
 * ReadFuture future = session.read();
 * // Wait until a message is received.
 * future.await();
 * try {
 *     Object message = future.getMessage();
 * } catch (Exception e) {
 *     ...
 * }
 * </pre>
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */
interface EReadFuture : virtual public EIoFuture {
	virtual ~EReadFuture() {
	}

	/**
	 * Returns the received message.  It returns <tt>null</tt> if this
	 * future is not ready or the associated {@link IoSession} has been closed.
	 *
	 * @throws RuntimeException if read or any relevant operation has failed.
	 */
	virtual EObject* getMessage() = 0;

	/**
	 * Returns <tt>true</tt> if a message was received successfully.
	 */
	virtual boolean isRead() = 0;

	/**
	 * Returns <tt>true</tt> if the {@link IoSession} associated with this
	 * future has been closed.
	 */
	virtual boolean isClosed() = 0;

	/**
	 * Returns the cause of the read failure if and only if the read
	 * operation has failed due to an {@link Exception}.  Otherwise,
	 * <tt>null</tt> is returned.
	 */
	virtual EThrowable* getException() = 0;
    
    /**
     * Try throw the exception of the read failure.
     */
    virtual void tryThrowException() = 0;

	/**
	 * Sets the message is written, and notifies all threads waiting for
	 * this future.  This method is invoked by MINA internally.  Please do
	 * not call this method directly.
	 */
	virtual void setRead(EObject* message) = 0;

	/**
	 * Sets the associated {@link IoSession} is closed.  This method is invoked
	 * by MINA internally.  Please do not call this method directly.
	 */
	virtual void setClosed() = 0;

	/**
	 * Sets the cause of the read failure, and notifies all threads waiting
	 * for this future.  This method is invoked by MINA internally.  Please
	 * do not call this method directly.
	 */
	virtual void setException(EObject* cause) = 0;
};

}
} // namespace efc::eio

#endif /* EREADFUTURE_HH_ */
