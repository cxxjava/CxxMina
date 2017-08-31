/*
 * EIoHandler.hh
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#ifndef EIOHANDLER_HH_
#define EIOHANDLER_HH_

#include "./EIdleStatus.hh"
#include "./EIoSession.hh"
#include "Efc.hh"

namespace efc {
namespace eio {

/**
 * Handles all I/O events fired by MINA.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 *
 * @see IoHandlerAdapter
 */
interface EIoHandler : virtual public EObject {
	virtual ~EIoHandler() {
	}

	/**
	 * Invoked from an I/O processor thread when a new connection has been created.
	 * Because this method is supposed to be called from the same thread that
	 * handles I/O of multiple sessions, please implement this method to perform
	 * tasks that consumes minimal amount of time such as socket parameter
	 * and user-defined session attribute initialization.
	 *
	 * @param session The session being created
     * @throws Exception If we get an exception while processing the create event
	 */
	virtual void sessionCreated(sp<EIoSession>& session) THROWS(EException) = 0;

	/**
	 * Invoked when a connection has been opened.  This method is invoked after
	 * {@link #sessionCreated(IoSession)}.  The biggest difference from
	 * {@link #sessionCreated(IoSession)} is that it's invoked from other thread
	 * than an I/O processor thread once thread model is configured properly.
	 *
	 * @param session The session being opened
     * @throws Exception If we get an exception while processing the open event
	 */
	virtual void sessionOpened(sp<EIoSession>& session) THROWS(EException) = 0;

	/**
	 * Invoked when a connection is closed.
	 *
	 * @param session The session being closed
     * @throws Exception If we get an exception while processing the close event
	 */
	virtual void sessionClosed(sp<EIoSession>& session) THROWS(EException) = 0;

	/**
	 * Invoked with the related {@link IdleStatus} when a connection becomes idle.
	 * This method is not invoked if the transport type is UDP; it's a known bug,
	 * and will be fixed in 2.0.
	 *
	 * @param session The idling session
     * @param status The session's status
     * @throws Exception If we get an exception while processing the idle event
	 */
	virtual void sessionIdle(sp<EIoSession>& session, EIdleStatus status) THROWS(EException) = 0;

	/**
	 * Invoked when any exception is thrown by user {@link IoHandler}
	 * implementation or by MINA.  If <code>cause</code> is an instance of
	 * {@link IOException}, MINA will close the connection automatically.
	 *
	 * @param session The session for which we have got an exception
     * @param cause The exception that has been caught
     * @throws Exception If we get an exception while processing the caught exception
	 */
	virtual void exceptionCaught(sp<EIoSession>& session, sp<EThrowableType>& cause) THROWS(EException) = 0;

	/**
	 * Invoked when a message is received.
	 *
	 * @param session The session that is receiving a message
     * @param message The received message
     * @throws Exception If we get an exception while processing the received message
	 */
	virtual void messageReceived(sp<EIoSession>& session, sp<EObject>& message) THROWS(EException) = 0;

	/**
	 * Invoked when a message written by {@link IoSession#write(Object)} is
	 * sent out.
	 *
	 * @param session The session that has sent a full message
     * @param message The sent message
     * @throws Exception If we get an exception while processing the sent message
	 */
	virtual void messageSent(sp<EIoSession>& session, sp<EObject>& message) THROWS(EException) = 0;

	/**
	 * Handle the closure of an half-duplex TCP channel
	 *
	 * @param session The session which input is being closed
     * @throws Exception If we get an exception while closing the input
	 */
	virtual void inputClosed(sp<EIoSession>& session) THROWS(EException) = 0;
};

} /* namespace eio */
} /* namespace efc */

#endif /* EIOHANDLER_HH_ */
