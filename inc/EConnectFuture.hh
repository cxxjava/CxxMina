/*
 * EConnectFuture.hh
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#ifndef ECONNECTFUTURE_HH_
#define ECONNECTFUTURE_HH_

#include "./EIoFuture.hh"
#include "./EIoSession.hh"
#include "./EIoFutureListener.hh"

namespace efc {
namespace eio {

/**
 * An {@link IoFuture} for asynchronous connect requests.
 *
 * <h3>Example</h3>
 * <pre>
 * IoConnector connector = ...;
 * ConnectFuture future = connector.connect(...);
 * future.join(); // Wait until the connection attempt is finished.
 * IoSession session = future.getSession();
 * session.write(...);
 * </pre>
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */
interface EConnectFuture : virtual public EIoFuture {
	virtual ~EConnectFuture() {
	}

	/**
	 * Returns {@link IoSession} which is the result of connect operation.
	 *
	 * @return <tt>null</tt> if the connect operation is not finished yet
	 * @throws RuntimeException if connection attempt failed by an exception
	 */
	virtual sp<EIoSession> getSession() = 0;

	/**
	 * Returns the cause of the connection failure.
	 *
	 * @return <tt>null</tt> if the connect operation is not finished yet,
	 *         or if the connection attempt is successful.
	 */
	virtual EThrowable* getException() = 0;
    
    /**
     * Try throw the exception of the connection failure.
     */
    virtual void tryThrowException() = 0;

	/**
	 * Returns <tt>true</tt> if the connect operation is finished successfully.
	 */
	virtual boolean isConnected() = 0;

	/**
	 * Returns {@code true} if the connect operation has been canceled by
	 * {@link #cancel()} method.
	 */
	virtual boolean isCanceled() = 0;

	/**
	 * Sets the newly connected session and notifies all threads waiting for
	 * this future.  This method is invoked by MINA internally.  Please do not
	 * call this method directly.
	 */
	virtual void setSession(sp<EIoSession> session) = 0;

	/**
	 * Sets the exception caught due to connection failure and notifies all
	 * threads waiting for this future.  This method is invoked by MINA
	 * internally.  Please do not call this method directly.
	 */
	virtual void setException(EObject* exception) = 0;

	/**
	 * Cancels the connection attempt and notifies all threads waiting for
	 * this future.
	 */
	virtual boolean cancel() = 0;
};

}
} // namespace efc::eio

#endif /* ECONNECTFUTURE_HH_ */
