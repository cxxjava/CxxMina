/*
 * EIoProcessor.hh
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#ifndef EIOPROCESSOR_HH_
#define EIOPROCESSOR_HH_

#include "./EIoSession.hh"
#include "./EWriteRequest.hh"

namespace efc {
namespace eio {

//class EIoSession;

/**
 * An internal interface to represent an 'I/O processor' that performs
 * actual I/O operations for {@link IoSession}s.  It abstracts existing
 * reactor frameworks such as Java NIO once again to simplify transport
 * implementations.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 *
 * @param <S> the type of the {@link IoSession} this processor can handle
 */

interface EIoProcessor : virtual public EObject {
	virtual ~EIoProcessor() {
	}

	/**
	 * Returns <tt>true</tt> if and if only {@link #dispose()} method has
	 * been called.  Please note that this method will return <tt>true</tt>
	 * even after all the related resources are released.
	 */
	virtual boolean isDisposing() = 0;

	/**
	 * Returns <tt>true</tt> if and if only all resources of this processor
	 * have been disposed.
	 */
	virtual boolean isDisposed() = 0;

	/**
	 * Releases any resources allocated by this processor.  Please note that
	 * the resources might not be released as long as there are any sessions
	 * managed by this processor.  Most implementations will close all sessions
	 * immediately and release the related resources.
	 */
	virtual void dispose() = 0;

	/**
	 * Adds the specified {@code session} to the I/O processor so that
	 * the I/O processor starts to perform any I/O operations related
	 * with the {@code session}.
	 *
	 * @param session The added session
	 */
	virtual void add(sp<EIoSession> session) = 0;

	/**
	 * Flushes the internal write request queue of the specified
	 * {@code session}.
	 *
	 * @param session The session we want the message to be written
	 */
	virtual void flush(sp<EIoSession>& session) = 0;

	/**
	 * Writes the WriteRequest for the specified {@code session}.
	 *
	 * @param session The session we want the message to be written
	 * @param writeRequest the WriteRequest to write
	 */
	virtual void write(sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) = 0;

	/**
	 * Controls the traffic of the specified {@code session} depending of the
	 * {@link IoSession#isReadSuspended()} and {@link IoSession#isWriteSuspended()}
	 * flags
	 *
	 * @param session The session to be updated
	 */
	virtual void updateTrafficControl(sp<EIoSession>& session) = 0;

	/**
	 * Removes and closes the specified {@code session} from the I/O
	 * processor so that the I/O processor closes the connection
	 * associated with the {@code session} and releases any other related
	 * resources.
	 *
	 * @param session The session to be removed
	 */
	virtual void remove(sp<EIoSession>& session) = 0;
};

} /* namespace eio */
} /* namespace efc */

#endif /* EIOPROCESSOR_HH_ */
