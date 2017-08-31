/*
 * EIoFuture.hh
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#ifndef EIOFUTURE_HH_
#define EIOFUTURE_HH_

#include "./EIoSession.hh"
#include "Efc.hh"

namespace efc {
namespace eio {

class EIoFutureListener;

/**
 * Represents the completion of an asynchronous I/O operation on an 
 * {@link IoSession}.
 * Can be listened for completion using a {@link IoFutureListener}.
 * 
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */
interface EIoFuture : virtual public EObject {
	virtual ~EIoFuture() {
	}

	/**
     * Returns the {@link IoSession} which is associated with this future.
     */
	virtual sp<EIoSession> getSession() = 0;

    /**
     * Wait for the asynchronous operation to complete.
     * The attached listeners will be notified when the operation is 
     * completed.
     */
	virtual void await() THROWS(EInterruptedException) = 0;

    /**
     * Wait for the asynchronous operation to complete with the specified timeout.
     *
     * @return <tt>true</tt> if the operation is completed.
     */
	virtual boolean await(llong timeout, ETimeUnit* unit) THROWS(EInterruptedException) = 0;

    /**
     * Wait for the asynchronous operation to complete with the specified timeout.
     *
     * @return <tt>true</tt> if the operation is completed.
     */
	virtual boolean await(llong timeoutMillis) THROWS(EInterruptedException) = 0;

    /**
     * Wait for the asynchronous operation to complete uninterruptibly.
     * The attached listeners will be notified when the operation is 
     * completed.
     * 
     * @return the current IoFuture
     */
	virtual void awaitUninterruptibly() = 0;

    /**
     * Wait for the asynchronous operation to complete with the specified timeout
     * uninterruptibly.
     *
     * @return <tt>true</tt> if the operation is completed.
     */
	virtual boolean awaitUninterruptibly(llong timeout, ETimeUnit* unit) = 0;

    /**
     * Wait for the asynchronous operation to complete with the specified timeout
     * uninterruptibly.
     *
     * @return <tt>true</tt> if the operation is finished.
     */
	virtual boolean awaitUninterruptibly(llong timeoutMillis) = 0;

    /**
     * Returns if the asynchronous operation is completed.
     */
	virtual boolean isDone() = 0;

    /**
     * Adds an event <tt>listener</tt> which is notified when
     * this future is completed. If the listener is added
     * after the completion, the listener is directly notified.
     */
	virtual void addListener(sp<EIoFutureListener> listener) = 0;

    /**
     * Removes an existing event <tt>listener</tt> so it won't be notified when
     * the future is completed.
     */
	virtual void removeListener(EIoFutureListener* listener) = 0;
};

}
} // namespace efc::eio

#endif /* EIOFUTURE_HH_ */
