/*
 * EWriteFuture.hh
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#ifndef EWRITEFUTURE_HH_
#define EWRITEFUTURE_HH_

#include "./EIoFuture.hh"
#include "./EIoFutureListener.hh"
#include "Efc.hh"

namespace efc {
namespace eio {

/**
 * An {@link IoFuture} for asynchronous write requests.
 *
 * <h3>Example</h3>
 * <pre>
 * IoSession session = ...;
 * WriteFuture future = session.write(...);
 * // Wait until the message is completely written out to the O/S buffer.
 * future.join();
 * if( future.isWritten() )
 * {
 *     // The message has been written successfully.
 * }
 * else
 * {
 *     // The messsage couldn't be written out completely for some reason.
 *     // (e.g. Connection is closed)
 * }
 * </pre>
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */
interface EWriteFuture : virtual public EIoFuture {
	virtual ~EWriteFuture() {
	}

	/**
	 * Returns <tt>true</tt> if the write operation is finished successfully.
	 */
	virtual boolean isWritten() = 0;

	/**
	 * Returns the cause of the write failure if and only if the write
	 * operation has failed due to an {@link Exception}.  Otherwise,
	 * <tt>null</tt> is returned.
	 */
	virtual EThrowable* getException() = 0;
    
    /**
     * Try throw the exception of the write failure.
     */
    virtual void tryThrowException() = 0;

	/**
	 * Sets the message is written, and notifies all threads waiting for
	 * this future.  This method is invoked by MINA internally.  Please do
	 * not call this method directly.
	 */
	virtual void setWritten() = 0;

	/**
	 * Sets the cause of the write failure, and notifies all threads waiting
	 * for this future.  This method is invoked by MINA internally.  Please
	 * do not call this method directly.
	 */
	virtual void setException(EObject* cause) = 0;
};

}
} // namespace efc::eio

#endif /* EWRITEFUTURE_HH_ */
