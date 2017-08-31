/*
 * EDefaultWriteFuture.hh
 *
 *  Created on: 2016-1-12
 *      Author: cxxjava@163.com
 */

#ifndef EDEFAULTWRITEFUTURE_HH_
#define EDEFAULTWRITEFUTURE_HH_

#include "./EDefaultIoFuture.hh"
#include "./EWriteFuture.hh"
#include "./EIoSession.hh"

namespace efc {
namespace eio {

/**
 * A default implementation of {@link WriteFuture}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EDefaultWriteFuture: public EDefaultIoFuture, virtual public EWriteFuture {
public:
	/**
	 * Returns a new {@link DefaultWriteFuture} which is already marked as 'written'.
	 *
	 * @param session The associated session
	 * @return A new future for a written message
	 */
	static EWriteFuture* newWrittenFuture(sp<EIoSession> session);

	/**
	 * Returns a new {@link DefaultWriteFuture} which is already marked as 'not written'.
	 *
	 * @param session The associated session
	 * @param cause The reason why the message has not be written
	 * @return A new future for not written message
	 */
	static EWriteFuture* newNotWrittenFuture(sp<EIoSession> session, EObject* cause);

public:
	virtual ~EDefaultWriteFuture();

	/**
	 * Creates a new instance.
	 *
	 * @param session The associated session
	 */
	EDefaultWriteFuture(sp<EIoSession> session);

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isWritten();

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
	virtual void setWritten();

	/**
	 * {@inheritDoc}
	 */
	virtual void setException(EObject* cause);
};

} /* namespace eio */
} /* namespace efc */
#endif /* EDEFAULTWRITEFUTURE_HH_ */
