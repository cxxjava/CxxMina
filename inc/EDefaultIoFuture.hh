/*
 * EDefaultIoFuture.hh
 *
 *  Created on: 2016-1-11
 *      Author: cxxjava@163.com
 */

#ifndef EDEFAULTIOFUTURE_HH_
#define EDEFAULTIOFUTURE_HH_

#include "./EIoFuture.hh"
#include "./EIoSession.hh"
#include "./EIoFutureListener.hh"

namespace efc {
namespace eio {

/**
 * A default implementation of {@link IoFuture} associated with
 * an {@link IoSession}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EDefaultIoFuture: virtual public EIoFuture, public ESynchronizeable,
		public enable_shared_from_this<EDefaultIoFuture> {
public:
	virtual ~EDefaultIoFuture();

	sp<EIoFuture> tosp();

	/**
	 * Creates a new instance associated with an {@link IoSession}.
	 *
	 * @param session an {@link IoSession} which is associated with this future
	 */
	EDefaultIoFuture(sp<EIoSession> session);

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EIoSession> getSession();

	/**
	 * {@inheritDoc}
	 */
	virtual void await() THROWS(EInterruptedException);

	/**
	 * {@inheritDoc}
	 */
	virtual boolean await(llong timeout, ETimeUnit* unit) THROWS(EInterruptedException);

	/**
	 * {@inheritDoc}
	 */
	virtual boolean await(llong timeoutMillis) THROWS(EInterruptedException);

	/**
	 * {@inheritDoc}
	 */
	virtual void awaitUninterruptibly();

	/**
	 * {@inheritDoc}
	 */
	virtual boolean awaitUninterruptibly(llong timeout, ETimeUnit* unit);

	/**
	 * {@inheritDoc}
	 */
	virtual boolean awaitUninterruptibly(llong timeoutMillis);

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isDone();

	/**
	 * Sets the result of the asynchronous operation, and mark it as finished.
	 *
	 * @param newValue The result to store into the Future
	 * @return {@code true} if the value has been set, {@code false} if
	 * the future already has a value (thus is in ready state)
	 */
	virtual boolean setValue(EObject* newValue);

	/**
	 * {@inheritDoc}
	 */
	virtual void addListener(sp<EIoFutureListener> listener);

	/**
	 * {@inheritDoc}
	 */
	virtual void removeListener(EIoFutureListener* listener);

protected:
	/**
	 * Returns the result of the asynchronous operation.
	 *
	 * @return The stored value
	 */
	EObject* getValue();

private:
	/** A number of milliseconds to wait between two deadlock controls ( 5 seconds ) */
	static const llong DEAD_LOCK_CHECK_INTERVAL = 5000L;

	/** The associated session */
	wp<EIoSession> session;

	/** The first listener. This is easier to have this variable
	 * when we most of the time have one single listener */
	sp<EIoFutureListener> firstListener;

	/** All the other listeners, in case we have more than one */
	EArrayList<sp<EIoFutureListener> > otherListeners;

	EObject* result;

	/** The flag used to determinate if the Future is completed or not */
	boolean ready;

	/** A counter for the number of threads waiting on this future */
	int waiters;

	/**
	 * Wait for the Future to be ready. If the requested delay is 0 or
	 * negative, this method immediately returns the value of the
	 * 'ready' flag.
	 * Every 5 second, the wait will be suspended to be able to check if
	 * there is a deadlock or not.
	 *
	 * @param timeoutMillis The delay we will wait for the Future to be ready
	 * @param interruptable Tells if the wait can be interrupted or not
	 * @return <code>true</code> if the Future is ready
	 * @throws InterruptedException If the thread has been interrupted
	 * when it's not allowed.
	 */
	boolean await0(llong timeoutMillis, boolean interruptable) THROWS(EInterruptedException);

	/**
	 * Check for a deadlock, ie look into the stack trace that we don't have already an
	 * instance of the caller.
	 */
	void checkDeadLock();

	/**
	 * Notify the listeners, if we have some.
	 */
	void notifyListeners();
	void notifyListener(sp<EIoFutureListener> listener);
};

} /* namespace eio */
} /* namespace efc */
#endif /* EDEFAULTIOFUTURE_HH_ */
