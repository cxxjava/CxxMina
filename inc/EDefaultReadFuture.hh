/*
 * EDefaultReadFuture.hh
 *
 *  Created on: 2016-1-11
 *      Author: cxxjava@163.com
 */

#ifndef EDEFAULTREADFUTURE_HH_
#define EDEFAULTREADFUTURE_HH_

#include "./EDefaultIoFuture.hh"
#include "./EReadFuture.hh"

namespace efc {
namespace eio {

/**
 * A default implementation of {@link WriteFuture}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EDefaultReadFuture: public EDefaultIoFuture, virtual public EReadFuture {
//public:
//	DECLARE_STATIC_INITZZ;

public:
	virtual ~EDefaultReadFuture();

	/**
	 * Creates a new instance.
	 *
	 * @param session The associated session
	 */
	EDefaultReadFuture(sp<EIoSession> session);

	/**
	 * {@inheritDoc}
	 */
	virtual EObject* getMessage();

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isRead();

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isClosed();

	/**
	 * {@inheritDoc}
	 */
	virtual EThrowable* getException();
    
	/**
	 * Try throw the exception of the connection failure.
	 */
	virtual void tryThrowException();

	/**
	 * {@inheritDoc}
	 */
	virtual void setRead(EObject* message);

	/**
	 * {@inheritDoc}
	 */
	virtual void setClosed();

	/**
	 * {@inheritDoc}
	 */
	virtual void setException(EObject* cause);

//private:
//	/** A static object used when the session is closed */
//	static EObject* CLOSED;// = new Object();
};

} /* namespace eio */
} /* namespace efc */
#endif /* EDEFAULTREADFUTURE_HH_ */
