/*
 * EIoSession.hh
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#ifndef EIOSESSION_HH_
#define EIOSESSION_HH_

#include "./EIoSessionConfig.hh"
#include "./EIdleStatus.hh"
#include "./EThrowableObject.hh"
#include "./ETransportMetadata.hh"
#include "Efc.hh"

namespace efc {
namespace eio {

class EIoService;
class EIoHandler;
class ECloseFuture;
class EIoFilterChain;
class EReadFuture;
class EWriteFuture;
class EWriteRequest;
class EWriteRequestQueue;

/**
 * A handle which represents connection between two end-points regardless of
 * transport types.
 * <p/>
 * {@link IoSession} provides user-defined attributes.  User-defined attributes
 * are application-specific data which are associated with a session.
 * It often contains objects that represents the state of a higher-level protocol
 * and becomes a way to exchange data between filters and handlers.
 * <p/>
 * <h3>Adjusting Transport Type Specific Properties</h3>
 * <p/>
 * You can simply downcast the session to an appropriate subclass.
 * </p>
 * <p/>
 * <h3>Thread Safety</h3>
 * <p/>
 * {@link IoSession} is thread-safe.  But please note that performing
 * more than one {@link #write(Object)} calls at the same time will
 * cause the {@link IoFilter#filterWrite(IoFilter.NextFilter,IoSession,WriteRequest)}
 * to be executed simultaneously, and therefore you have to make sure the
 * {@link IoFilter} implementations you're using are thread-safe, too.
 * </p>
 * <p/>
 * <h3>Equality of Sessions</h3>
 * TODO : The getId() method is totally wrong. We can't base
 * a method which is designed to create a unique ID on the hashCode method.
 * {@link #equals(Object)} and {@link #hashCode()} shall not be overriden
 * to the default behavior that is defined in {@link Object}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */
interface EIoSession : virtual public EObject {
	virtual ~EIoSession() {
	}

	/**
	 * @return a unique identifier for this session.  Every session has its own
	 * ID which is different from each other.
	 *
	 * TODO : The way it's implemented does not guarantee that the contract is
	 * respected. It uses the HashCode() method which don't guarantee the key
	 * unicity.
	 */
	virtual long getId() = 0;

	/**
	 * @return the {@link IoService} which provides I/O service to this session.
	 */
	virtual EIoService* getService() = 0;

	/**
	 * @return the {@link IoHandler} which handles this session.
	 */
	virtual EIoHandler* getHandler() = 0;

	/**
	 * @return the configuration of this session.
	 */
	virtual EIoSessionConfig* getConfig() = 0;

	/**
	 * @return the filter chain that only affects this session.
	 */
	virtual EIoFilterChain* getFilterChain() = 0;

	/**
	 * Get the queue that contains the message waiting for being written.
	 * As the reader might not be ready, it's frequent that the messages
	 * aren't written completely, or that some older messages are waiting
	 * to be written when a new message arrives. This queue is used to manage
	 * the backlog of messages.
	 *
	 * @return The queue containing the pending messages.
	 */
	virtual EWriteRequestQueue* getWriteRequestQueue() = 0;

	/**
	 * @return the {@link TransportMetadata} that this session runs on.
	 */
	virtual ETransportMetadata* getTransportMetadata() = 0;

	/**
	 * TODO This javadoc is wrong. The return tag should be short.
	 *
	 * @return a {@link ReadFuture} which is notified when a new message is
	 * received, the connection is closed or an exception is caught.  This
	 * operation is especially useful when you implement a client application.
	 * TODO : Describe here how we enable this feature.
	 * However, please note that this operation is disabled by default and
	 * throw {@link IllegalStateException} because all received events must be
	 * queued somewhere to support this operation, possibly leading to memory
	 * leak.  This means you have to keep calling {@link #read()} once you
	 * enabled this operation.  To enable this operation, please call
	 * {@link IoSessionConfig#setUseReadOperation(boolean)} with <tt>true</tt>.
	 *
	 * @throws IllegalStateException if
	 * {@link IoSessionConfig#setUseReadOperation(boolean) useReadOperation}
	 * option has not been enabled.
	 */
	virtual sp<EReadFuture> read() = 0;

	/**
	 * Writes the specified <code>message</code> to remote peer.  This
	 * operation is asynchronous; {@link IoHandler#messageSent(IoSession,Object)}
	 * will be invoked when the message is actually sent to remote peer.
	 * You can also wait for the returned {@link WriteFuture} if you want
	 * to wait for the message actually written.
	 *
	 * @param message The message to write
     * @return The associated WriteFuture
	 */
	virtual sp<EWriteFuture> write(sp<EObject> message) = 0;

	/**
	 * (Optional) Writes the specified <tt>message</tt> to the specified <tt>destination</tt>.
	 * This operation is asynchronous; {@link IoHandler#messageSent(IoSession, Object)}
	 * will be invoked when the message is actually sent to remote peer. You can
	 * also wait for the returned {@link WriteFuture} if you want to wait for
	 * the message actually written.
	 * <p>
	 * When you implement a client that receives a broadcast message from a server
	 * such as DHCP server, the client might need to send a response message for the
	 * broadcast message the server sent.  Because the remote address of the session
	 * is not the address of the server in case of broadcasting, there should be a
	 * way to specify the destination when you write the response message.
	 * This interface provides {@link #write(Object, SocketAddress)} method so you
	 * can specify the destination.
	 *
	 * @param message The message to write
     * @param destination <tt>null</tt> if you want the message sent to the
     *                    default remote address
     * @return The associated WriteFuture
	 */
	virtual sp<EWriteFuture> write(sp<EObject> message, EInetSocketAddress* destination) = 0;

	/**
	 * Closes this session immediately or after all queued write requests
	 * are flushed.  This operation is asynchronous.  Wait for the returned
	 * {@link CloseFuture} if you want to wait for the session actually closed.
	 *
	 * @param immediately {@code true} to close this session immediately
	 *                    . The pending write requests
	 *                    will simply be discarded.
	 *                    {@code false} to close this session after all queued
	 *                    write requests are flushed.
	 * @return The associated CloseFuture
	 * @deprecated Use either the closeNow() or the flushAndClose() methods
	 */
	virtual sp<ECloseFuture> close(boolean immediately) = 0;

	/**
	 * Closes this session immediately.  This operation is asynchronous, it
	 * returns a {@link CloseFuture}.
     *
     * @return The {@link CloseFuture} that can be use to wait for the completion of this operation
  	 */
	virtual sp<ECloseFuture> closeNow() = 0;

	/**
	 * Closes this session after all queued write requests are flushed.  This operation
	 * is asynchronous.  Wait for the returned {@link CloseFuture} if you want to wait
	 * for the session actually closed.
	 *
	 * @return The associated CloseFuture
	 */
	virtual sp<ECloseFuture> closeOnFlush() = 0;

	/**
	 * Returns the value of the user-defined attribute of this session.
	 *
	 * @param key the key of the attribute
	 * @return <tt>null</tt> if there is no attribute with the specified key
	 */
	virtual sp<EObject> getAttribute(EObject* key) = 0;

	/**
	 * Returns the value of user defined attribute associated with the
	 * specified key.  If there's no such attribute, the specified default
	 * value is associated with the specified key, and the default value is
	 * returned.  This method is same with the following code except that the
	 * operation is performed atomically.
	 * <pre>
	 * if (containsAttribute(key)) {
	 *     return getAttribute(key);
	 * } else {
	 *     setAttribute(key, defaultValue);
	 *     return defaultValue;
	 * }
	 * </pre>
	 *
	 * @param key the key of the attribute we want to retreive
     * @param defaultValue the default value of the attribute
     * @return The retrieved attribute or <tt>null</tt> if not found
	 */
	virtual sp<EObject> getAttribute(EObject* key, EObject* defaultValue) = 0;

	/**
	 * Sets a user-defined attribute.
	 *
	 * @param key   the key of the attribute
	 * @param value the value of the attribute
	 * @return The old value of the attribute.  <tt>null</tt> if it is new.
	 */
	virtual sp<EObject> setAttribute(sp<EObject> key, sp<EObject> value) = 0;

	/**
	 * Sets a user defined attribute without a value.  This is useful when
	 * you just want to put a 'mark' attribute.  Its value is set to
	 * {@link Boolean#TRUE}.
	 *
	 * @param key the key of the attribute
	 * @return The old value of the attribute.  <tt>null</tt> if it is new.
	 */
	virtual sp<EObject> setAttribute(sp<EObject> key) = 0;

	/**
	 * Sets a user defined attribute if the attribute with the specified key
	 * is not set yet.  This method is same with the following code except
	 * that the operation is performed atomically.
	 * <pre>
	 * if (containsAttribute(key)) {
	 *     return getAttribute(key);
	 * } else {
	 *     return setAttribute(key, value);
	 * }
	 * </pre>
	 *
	 * @param key The key of the attribute we want to set
     * @param value The value we want to set
     * @return The old value of the attribute.  <tt>null</tt> if not found.
	 */
	virtual sp<EObject> setAttributeIfAbsent(sp<EObject> key, sp<EObject> value) = 0;

	/**
	 * Sets a user defined attribute without a value if the attribute with
	 * the specified key is not set yet.  This is useful when you just want to
	 * put a 'mark' attribute.  Its value is set to {@link Boolean#TRUE}.
	 * This method is same with the following code except that the operation
	 * is performed atomically.
	 * <pre>
	 * if (containsAttribute(key)) {
	 *     return getAttribute(key);  // might not always be Boolean.TRUE.
	 * } else {
	 *     return setAttribute(key);
	 * }
	 * </pre>
	 *
	 * @param key The key of the attribute we want to set
     * @return The old value of the attribute.  <tt>null</tt> if not found.
	 */
	virtual sp<EObject> setAttributeIfAbsent(sp<EObject> key) = 0;

	/**
	 * Removes a user-defined attribute with the specified key.
	 *
	 * @param key The key of the attribute we want to remove
	 * @return The old value of the attribute.  <tt>null</tt> if not found.
	 */
	virtual sp<EObject> removeAttribute(EObject* key) = 0;

	/**
	 * Removes a user defined attribute with the specified key if the current
	 * attribute value is equal to the specified value.  This method is same
	 * with the following code except that the operation is performed
	 * atomically.
	 * <pre>
	 * if (containsAttribute(key) && getAttribute(key).equals(value)) {
	 *     removeAttribute(key);
	 *     return true;
	 * } else {
	 *     return false;
	 * }
	 * </pre>
	 *
	 * @param key The key we want to remove
     * @param value The value we want to remove
     * @return <tt>true</tt> if the removal was successful
	 */
	virtual boolean removeAttribute(EObject* key, EObject* value) = 0;

	/**
	 * Replaces a user defined attribute with the specified key if the
	 * value of the attribute is equals to the specified old value.
	 * This method is same with the following code except that the operation
	 * is performed atomically.
	 * <pre>
	 * if (containsAttribute(key) && getAttribute(key).equals(oldValue)) {
	 *     setAttribute(key, newValue);
	 *     return true;
	 * } else {
	 *     return false;
	 * }
	 * </pre>
	 *
	 * @param key The key we want to replace
     * @param oldValue The previous value
     * @param newValue The new value
     * @return <tt>true</tt> if the replacement was successful
	 */
	virtual boolean replaceAttribute(EObject* key, EObject* oldValue,
			sp<EObject> newValue) = 0;

	/**
	 * @param key The key of the attribute we are looking for in the session
	 * @return <tt>true</tt> if this session contains the attribute with
	 * the specified <tt>key</tt>.
	 */
	virtual boolean containsAttribute(EObject* key) = 0;

	/**
	 * @return the set of keys of all user-defined attributes.
	 */
	virtual ESet<sp<EObject> >* getAttributeKeys() = 0;

	/**
	 * @return <code>true</code> if this session is connected with remote peer.
	 */
	virtual boolean isConnected() = 0;

	/**
	 * @return <code>true</tt> if and only if this session is being closed
	 * (but not disconnected yet) or is closed.
	 */
	virtual boolean isClosing() = 0;

	/**
	 * @return <code>true</tt> if the session has started and initialized a SslEngine,
	 * <code>false</code> if the session is not yet secured (the handshake is not completed)
	 * or if SSL is not set for this session, or if SSL is not even an option.
	 */
	virtual boolean isSecured() = 0;

	/**
	 *
	 */
	virtual boolean isDestroyed() = 0;

	/**
	 *
	 */
	virtual void destroy() = 0;

	/**
	 * Returns the {@link CloseFuture} of this session.  This method returns
	 * the same instance whenever user calls it.
	 */
	virtual sp<ECloseFuture> getCloseFuture() = 0;

	/**
	 * Returns the socket address of remote peer.
	 */
	virtual EInetSocketAddress* getRemoteAddress() = 0;

	/**
	 * Returns the socket address of local machine which is associated with this
	 * session.
	 */
	virtual EInetSocketAddress* getLocalAddress() = 0;

	/**
	 * Returns the socket address of the {@link IoService} listens to to manage
	 * this session.  If this session is managed by {@link IoAcceptor}, it
	 * returns the {@link SocketAddress} which is specified as a parameter of
	 * {@link IoAcceptor#bind()}.  If this session is managed by
	 * {@link IoConnector}, this method returns the same address with
	 * that of {@link #getRemoteAddress()}.
	 */
	virtual EInetSocketAddress* getServiceAddress() = 0;

	/**
	 *
	 * TODO setWriteRequestQueue.
	 *
	 * @param writeRequestQueue
	 */
	virtual void setCurrentWriteRequest(sp<EWriteRequest> currentWriteRequest) = 0;

	/**
	 * Suspends read operations for this session.
	 */
	virtual void suspendRead() = 0;

	/**
	 * Suspends write operations for this session.
	 */
	virtual void suspendWrite() = 0;

	/**
	 * Resumes read operations for this session.
	 */
	virtual void resumeRead() = 0;

	/**
	 * Resumes write operations for this session.
	 */
	virtual void resumeWrite() = 0;

	/**
	 * Is read operation is suspended for this session.
	 * @return <code>true</code> if suspended
	 */
	virtual boolean isReadSuspended() = 0;

	/**
	 * Is write operation is suspended for this session.
	 * @return <code>true</code> if suspended
	 */
	virtual boolean isWriteSuspended() = 0;

	/**
	 * Update all statistical properties related with throughput assuming
	 * the specified time is the current time.  By default this method returns
	 * silently without updating the throughput properties if they were
	 * calculated already within last
	 * {@link IoSessionConfig#getThroughputCalculationInterval() calculation interval}.
	 * If, however, <tt>force</tt> is specified as <tt>true</tt>, this method
	 * updates the throughput properties immediately.

	 * @param currentTime the current time in milliseconds
	 * @param force Force the update if <tt>true</tt>
	 */
	virtual void updateThroughput(llong currentTime, boolean force) = 0;

	/**
	 * Returns the total number of bytes which were read from this session.
	 */
	virtual long getReadBytes() = 0;

	/**
	 * Returns the total number of bytes which were written to this session.
	 */
	virtual long getWrittenBytes() = 0;

	/**
	 * Returns the total number of messages which were read and decoded from this session.
	 */
	virtual long getReadMessages() = 0;

	/**
	 * Returns the total number of messages which were written and encoded by this session.
	 */
	virtual long getWrittenMessages() = 0;

	/**
	 * Returns the number of read bytes per second.
	 */
	virtual double getReadBytesThroughput() = 0;

	/**
	 * Returns the number of written bytes per second.
	 */
	virtual double getWrittenBytesThroughput() = 0;

	/**
	 * Returns the number of read messages per second.
	 */
	virtual double getReadMessagesThroughput() = 0;

	/**
	 * Returns the number of written messages per second.
	 */
	virtual double getWrittenMessagesThroughput() = 0;

	/**
	 * Returns the number of messages which are scheduled to be written to this session.
	 */
	virtual int getScheduledWriteMessages() = 0;

	/**
	 * Returns the number of bytes which are scheduled to be written to this
	 * session.
	 */
	virtual long getScheduledWriteBytes() = 0;

	/**
	 * Returns the message which is being written by {@link IoService}.
	 * @return <tt>null</tt> if and if only no message is being written
	 */
	virtual sp<EObject> getCurrentWriteMessage() = 0;

	/**
	 * Returns the {@link WriteRequest} which is being processed by
	 * {@link IoService}.
	 *
	 * @return <tt>null</tt> if and if only no message is being written
	 */
	virtual sp<EWriteRequest> getCurrentWriteRequest() = 0;

	/**
	 * @return the session's creation time in milliseconds
	 */
	virtual llong getCreationTime() = 0;

	/**
	 * Returns the time in millis when I/O occurred lastly.
	 */
	virtual llong getLastIoTime() = 0;

	/**
	 * Returns the time in millis when read operation occurred lastly.
	 */
	virtual llong getLastReadTime() = 0;

	/**
	 * Returns the time in millis when write operation occurred lastly.
	 */
	virtual llong getLastWriteTime() = 0;

	/**
	 * Returns <code>true</code> if this session is idle for the specified
	 * {@link IdleStatus}.
	 */
	virtual boolean isIdle(EIdleStatus status) = 0;

	/**
	 * Returns <code>true</code> if this session is {@link IdleStatus#READER_IDLE}.
	 * @see #isIdle(IdleStatus)
	 */
	virtual boolean isReaderIdle() = 0;

	/**
	 * Returns <code>true</code> if this session is {@link IdleStatus#WRITER_IDLE}.
	 * @see #isIdle(IdleStatus)
	 */
	virtual boolean isWriterIdle() = 0;

	/**
	 * Returns <code>true</code> if this session is {@link IdleStatus#BOTH_IDLE}.
	 * @see #isIdle(IdleStatus)
	 */
	virtual boolean isBothIdle() = 0;

	/**
	 * @param status The researched idle status
	 * @return the number of the fired continuous <tt>sessionIdle</tt> events
	 * for the specified {@link IdleStatus}.
	 * <p>
	 * If <tt>sessionIdle</tt> event is fired first after some time after I/O,
	 * <tt>idleCount</tt> becomes <tt>1</tt>.  <tt>idleCount</tt> resets to
	 * <tt>0</tt> if any I/O occurs again, otherwise it increases to
	 * <tt>2</tt> and so on if <tt>sessionIdle</tt> event is fired again without
	 * any I/O between two (or more) <tt>sessionIdle</tt> events.
	     */
	virtual int getIdleCount(EIdleStatus status) = 0;

	/**
	 * Returns the number of the fired continuous <tt>sessionIdle</tt> events
	 * for {@link IdleStatus#READER_IDLE}.
	 * @see #getIdleCount(IdleStatus)
	 */
	virtual int getReaderIdleCount() = 0;

	/**
	 * Returns the number of the fired continuous <tt>sessionIdle</tt> events
	 * for {@link IdleStatus#WRITER_IDLE}.
	 * @see #getIdleCount(IdleStatus)
	 */
	virtual int getWriterIdleCount() = 0;

	/**
	 * Returns the number of the fired continuous <tt>sessionIdle</tt> events
	 * for {@link IdleStatus#BOTH_IDLE}.
	 * @see #getIdleCount(IdleStatus)
	 */
	virtual int getBothIdleCount() = 0;

	/**
	 * @param status The researched idle status
	 * @return the time in milliseconds when the last <tt>sessionIdle</tt> event
	 * is fired for the specified {@link IdleStatus}.
	 */
	virtual llong getLastIdleTime(EIdleStatus status) = 0;

	/**
	 * Returns the time in milliseconds when the last <tt>sessionIdle</tt> event
	 * is fired for {@link IdleStatus#READER_IDLE}.
	 * @see #getLastIdleTime(IdleStatus)
	 */
	virtual llong getLastReaderIdleTime() = 0;

	/**
	 * Returns the time in milliseconds when the last <tt>sessionIdle</tt> event
	 * is fired for {@link IdleStatus#WRITER_IDLE}.
	 * @see #getLastIdleTime(IdleStatus)
	 */
	virtual llong getLastWriterIdleTime() = 0;

	/**
	 * Returns the time in milliseconds when the last <tt>sessionIdle</tt> event
	 * is fired for {@link IdleStatus#BOTH_IDLE}.
	 * @see #getLastIdleTime(IdleStatus)
	 */
	virtual llong getLastBothIdleTime() = 0;

	/**
	 *
	 */
	virtual ELock* getLock() = 0;
};

} /* namespace eio */
} /* namespace efc */

#endif /* EIOSESSION_HH_ */
