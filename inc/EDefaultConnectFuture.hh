/*
 * EDefaultConnectFuture.hh
 *
 *  Created on: 2016-3-22
 *      Author: cxxjava@163.com
 */

#ifndef EDEFAULTCONNECTFUTURE_HH_
#define EDEFAULTCONNECTFUTURE_HH_

#include "./EDefaultIoFuture.hh"
#include "./EConnectFuture.hh"

namespace efc {
namespace eio {

/**
 * A default implementation of {@link ConnectFuture}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EDefaultConnectFuture: public EDefaultIoFuture,
		public virtual EConnectFuture {
public:
	/**
	 * Creates a new instance.
	 */
	EDefaultConnectFuture();

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EIoSession> getSession();

	/**
	 * {@inheritDoc}
	 */
	virtual EThrowable* getException();

	/**
	 * {@inheritDoc}
	 */
	virtual void tryThrowException();

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isConnected();

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isCanceled();

	/**
	 * {@inheritDoc}
	 */
	virtual void setSession(sp<EIoSession> session);

	/**
	 * {@inheritDoc}
	 */
	virtual void setException(EObject* exception);

	/**
	 * {@inheritDoc}
	 */
	virtual boolean cancel();

	/**
	 * Creates a new instance of a Connection Failure, with the associated cause.
	 *
	 * @param exception The exception that caused the failure
	 * @return a new {@link ConnectFuture} which is already marked as 'failed to connect'.
	 */
	static EConnectFuture* newFailedFuture(EObject* exception);
};

} /* namespace eio */
} /* namespace efc */
#endif /* EDEFAULTCONNECTFUTURE_HH_ */
