/*
 * EAbstractIoSession.hh
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#ifndef EABSTRACTIOSESSION_HH_
#define EABSTRACTIOSESSION_HH_

#include "./EIoSession.hh"
#include "./EIoProcessor.hh"
#include "./EIdleStatus.hh"
#include "./ECloseFuture.hh"
#include "./EAttributeKey.hh"
#include "./EWriteRequestQueue.hh"
#include "./EIoSessionAttributeMap.hh"

namespace efc {
namespace eio {

/**
 * Base implementation of {@link IoSession}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */
abstract class EAbstractIoSession: virtual public EIoSession,
		public enable_shared_from_this<EAbstractIoSession> {
public:
	DECLARE_STATIC_INITZZ;

    /**
	 * An internal write request object that triggers session close.
	 *
	 * @see #writeRequestQueue
	 */
	static sp<EWriteRequest> CLOSE_REQUEST;// = new DefaultWriteRequest(new Object());

	/**
	 * An internal write request object that triggers message sent events.
	 *
	 * @see #writeRequestQueue
	 */
	static sp<EWriteRequest> MESSAGE_SENT_REQUEST;// = new DefaultWriteRequest(DefaultWriteRequest.EMPTY_MESSAGE);

public:
	virtual ~EAbstractIoSession();

	sp<EIoSession> tosp();

	/**
	 * Initialize close future with sp<>(this).
	 *
	 * moved from ctor(): cxxjava@lotuseed.com
	 */
	virtual void init();

	/**
	 * {@inheritDoc}
	 *
	 * We use an AtomicLong to guarantee that the session ID are unique.
	 */
	virtual long getId();

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isConnected();

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isActive();

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isClosing();

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isSecured();

	/**
	 * {@inheritDoc}
	 */
	virtual sp<ECloseFuture> getCloseFuture();

	/**
	 * Tells if the session is scheduled for flushed
	 *
	 * @return true if the session is scheduled for flush
	 */
	boolean isScheduledForFlush();

	/**
	 * Schedule the session for flushed
	 */
	void scheduledForFlush();

	/**
	 * Change the session's status : it's not anymore scheduled for flush
	 */
	void unscheduledForFlush();

	/**
	 * Set the scheduledForFLush flag. As we may have concurrent access to this
	 * flag, we compare and set it in one call.
	 *
	 * @param schedule
	 *            the new value to set if not already set.
	 * @return true if the session flag has been set, and if it wasn't set
	 *         already.
	 */
	boolean setScheduledForFlush(boolean schedule);

	/**
	 * {@inheritDoc}
	 */
	virtual sp<ECloseFuture> close(boolean rightNow);

	/**
	 * {@inheritDoc}
	 */
	virtual sp<ECloseFuture> close();

	/**
	 * {@inheritDoc}
	 */
	virtual sp<ECloseFuture> closeOnFlush();

	/**
	 * {@inheritDoc}
	 */
	virtual sp<ECloseFuture> closeNow();

	/**
	 * {@inheritDoc}
	 */
	virtual EIoHandler* getHandler();

	/**
	 * {@inheritDoc}
	 */
	virtual EIoSessionConfig* getConfig();

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EReadFuture> read();

	/**
	 * Associates a message to a ReadFuture
	 *
	 * @param message the message to associate to the ReadFuture
	 *
	 */
	void offerReadFuture(EObject* message);

	/**
	 * Associates a failure to a ReadFuture
	 *
	 * @param exception the exception to associate to the ReadFuture
	 */
	void offerFailedReadFuture(EObject* exception);

	/**
	 * Inform the ReadFuture that the session has been closed
	 */
	void offerClosedReadFuture();

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EWriteFuture> write(sp<EObject> message);

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EWriteFuture> write(sp<EObject> message, EInetSocketAddress* remoteAddress);

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EObject> getAttachment();

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EObject> setAttachment(sp<EObject> attachment);

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EObject> getAttribute(EObject* key);

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EObject> getAttribute(EObject* key, EObject* defaultValue);

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EObject> setAttribute(sp<EObject> key, sp<EObject> value);

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EObject> setAttribute(sp<EObject> key);

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EObject> setAttributeIfAbsent(sp<EObject> key, sp<EObject> value);

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EObject> setAttributeIfAbsent(sp<EObject> key);

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EObject> removeAttribute(EObject* key);

	/**
	 * {@inheritDoc}
	 */
	virtual boolean removeAttribute(EObject* key, EObject* value);

	/**
	 * {@inheritDoc}
	 */
	virtual boolean replaceAttribute(EObject* key, EObject* oldValue, sp<EObject> newValue);

	/**
	 * {@inheritDoc}
	 */
	virtual boolean containsAttribute(EObject* key);

	/**
	 * {@inheritDoc}
	 */
	virtual ESet<sp<EObject> >* getAttributeKeys();

	/**
	 * @return The map of attributes associated with the session
	 */
	EIoSessionAttributeMap* getAttributeMap();

	/**
	 * Set the map of attributes associated with the session
	 *
	 * @param attributes The Map of attributes
	 */
	void setAttributeMap(EIoSessionAttributeMap* attributes);

	/**
	 * Create a new close aware write queue, based on the given write queue.
	 *
	 * @param writeRequestQueue The write request queue
	 */
	void setWriteRequestQueue(EWriteRequestQueue* writeRequestQueue);

	/**
	 * {@inheritDoc}
	 */
	virtual void suspendRead();

	/**
	 * {@inheritDoc}
	 */
	virtual void suspendWrite();

	/**
	 * {@inheritDoc}
	 */
	virtual void resumeRead();

	/**
	 * {@inheritDoc}
	 */
	virtual void resumeWrite();

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isReadSuspended();

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isWriteSuspended();

	/**
	 * {@inheritDoc}
	 */
	virtual long getReadBytes();

	/**
	 * {@inheritDoc}
	 */
	virtual long getWrittenBytes();

	/**
	 * {@inheritDoc}
	 */
	virtual long getReadMessages();

	/**
	 * {@inheritDoc}
	 */
	virtual long getWrittenMessages();

	/**
	 * {@inheritDoc}
	 */
	virtual double getReadBytesThroughput();

	/**
	 * {@inheritDoc}
	 */
	virtual double getWrittenBytesThroughput();

	/**
	 * {@inheritDoc}
	 */
	virtual double getReadMessagesThroughput();

	/**
	 * {@inheritDoc}
	 */
	virtual double getWrittenMessagesThroughput();

	/**
	 * {@inheritDoc}
	 */
	virtual void updateThroughput(llong currentTime, boolean force);

	/**
	 * {@inheritDoc}
	 */
	virtual long getScheduledWriteBytes();

	/**
	 * {@inheritDoc}
	 */
	virtual int getScheduledWriteMessages();

	/**
	 * Increase the number of read bytes
	 *
	 * @param increment The number of read bytes
	 * @param currentTime The current time
	 */
	void increaseReadBytes(long increment, llong currentTime);

	/**
	 * Increase the number of read messages
	 *
	 * @param currentTime The current time
	 */
	void increaseReadMessages(llong currentTime);

	/**
	 * Increase the number of written bytes
	 *
	 * @param increment The number of written bytes
	 * @param currentTime The current time
	 */
	void increaseWrittenBytes(int increment, llong currentTime);

	/**
	 * Increase the number of written messages
	 *
	 * @param request The written message
	 * @param currentTime The current tile
	 */
	void increaseWrittenMessages(sp<EWriteRequest>& request, llong currentTime);

	/**
	 * Increase the number of scheduled write bytes for the session
	 *
	 * @param increment The number of newly added bytes to write
	 */
	void increaseScheduledWriteBytes(int increment);

	/**
	 * Increase the number of scheduled message to write
	 */
	void increaseScheduledWriteMessages();

	/**
	 * Decrease the counters of written messages and written bytes when a message has been written
	 *
	 * @param request The written message
	 */
	void decreaseScheduledBytesAndMessages(sp<EWriteRequest>& request);

	/**
	 * {@inheritDoc}
	 */
	virtual EWriteRequestQueue* getWriteRequestQueue();

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EWriteRequest> getCurrentWriteRequest();

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EObject> getCurrentWriteMessage();

	/**
	 * {@inheritDoc}
	 */
	virtual void setCurrentWriteRequest(sp<EWriteRequest> currentWriteRequest);

	/**
	 * Increase the ReadBuffer size (it will double)
	 */
	void increaseReadBufferSize();

	/**
	 * Decrease the ReadBuffer size (it will be divided by a factor 2)
	 */
	void decreaseReadBufferSize();

	/**
	 * {@inheritDoc}
	 */
	virtual llong getCreationTime();

	/**
	 * {@inheritDoc}
	 */
	virtual llong getLastIoTime();

	/**
	 * {@inheritDoc}
	 */
	virtual llong getLastReadTime();

	/**
	 * {@inheritDoc}
	 */
	virtual llong getLastWriteTime();

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isIdle(EIdleStatus status);

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isBothIdle();

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isReaderIdle();

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isWriterIdle();

	/**
	 * {@inheritDoc}
	 */
	virtual int getIdleCount(EIdleStatus status);

	/**
	 * {@inheritDoc}
	 */
	virtual llong getLastIdleTime(EIdleStatus status);

	/**
	 * Increase the count of the various Idle counter
	 *
	 * @param status The current status
	 * @param currentTime The current time
	 */
	void increaseIdleCount(EIdleStatus status, llong currentTime);

	/**
	 * {@inheritDoc}
	 */
	virtual int getBothIdleCount();

	/**
	 * {@inheritDoc}
	 */
	virtual llong getLastBothIdleTime();

	/**
	 * {@inheritDoc}
	 */
	virtual llong getLastReaderIdleTime();

	/**
	 * {@inheritDoc}
	 */
	virtual llong getLastWriterIdleTime();

	/**
	 * {@inheritDoc}
	 */
	virtual int getReaderIdleCount();

	/**
	 * {@inheritDoc}
	 */
	virtual int getWriterIdleCount();

	/**
	 * {@inheritDoc}
	 */
	virtual EInetSocketAddress* getServiceAddress();

	/**
	 * {@inheritDoc}
	 */
	virtual EIoService* getService();

	/**
	 * {@inheritDoc}
	 */
	virtual ELock* getLock();

	/**
	 * {@inheritDoc}
	 */
	virtual EString toString();

	/**
	 * @return The associated IoProcessor for this session
	 */
	virtual EIoProcessor* getProcessor() = 0;

	/**
	 * Fires a {@link IoEventType#SESSION_IDLE} event to any applicable sessions
	 * in the specified collection.
	 *
	 * @param sessions The sessions that are notified
	 * @param currentTime the current time (i.e. {@link System#currentTimeMillis()})
	 */
	static void notifyIdleness(sp<EIterator<sp<EAbstractIoSession> > >& sessions, llong currentTime);

	/**
	 * Fires a {@link IoEventType#SESSION_IDLE} event if applicable for the
	 * specified {@code session}.
	 *
	 * @param session The session that is notified
	 * @param currentTime the current time (i.e. {@link System#currentTimeMillis()})
	 */
	static void notifyIdleSession(sp<EAbstractIoSession>& session, llong currentTime);

protected:
	/** The session config */
	EIoSessionConfig* config;

	/**
	 * Create a Session for a service
	 *
	 * @param service the Service for this session
	 */
	EAbstractIoSession(EIoService* service);

	/**
	 * Set the number of scheduled write bytes
	 *
	 * @param byteCount The number of scheduled bytes for write
	 */
	virtual void setScheduledWriteBytes(int byteCount);

	/**
	 * Set the number of scheduled write messages
	 *
	 * @param messages The number of scheduled messages for write
	 */
	virtual void setScheduledWriteMessages(int messages);

private:
	friend class CloseAwareWriteQueue;
	friend class CloseFutureListener;

	template<typename E>
	class ConcurrentLinkedQueue: public EConcurrentLiteQueue<E>,
			public ESynchronizeable {
	};

	/** The associated handler */
	EIoHandler* handler;

	/** The service which will manage this session */
	EIoService* service;

	/** The lock for close */
	EReentrantLock lock;

	EIoSessionAttributeMap* attributes;
	EWriteRequestQueue* writeRequestQueue;
	sp<EWriteRequest> currentWriteRequest;

	/** The Session creation's time */
	llong creationTime;

	/** The session ID */
	long sessionId;

	/**
	 * A future that will be set 'closed' when the connection is closed.
	 */
	sp<ECloseFuture> closeFuture;// = new DefaultCloseFuture(this);
	volatile boolean closing;

	// traffic control
	boolean readSuspended;// = false;

	boolean writeSuspended;// = false;

	// Status variables
	EAtomicBoolean scheduledForFlush_;// = new AtomicBoolean();
	EAtomicInteger scheduledWriteBytes;// = new AtomicInteger();
	EAtomicInteger scheduledWriteMessages;// = new AtomicInteger();
    EAtomicInteger idleCountForBoth;// = new AtomicInteger();
    EAtomicInteger idleCountForRead;// = new AtomicInteger();
    EAtomicInteger idleCountForWrite;// = new AtomicInteger();
	long readBytes;
    long writtenBytes;
    long readMessages;
    long writtenMessages;
    llong lastReadTime;
    llong lastWriteTime;
    llong lastThroughputCalculationTime;
    long lastReadBytes;
    long lastWrittenBytes;
    long lastReadMessages;
    long lastWrittenMessages;
    double readBytesThroughput;
    double writtenBytesThroughput;
    double readMessagesThroughput;
    double writtenMessagesThroughput;
    long lastIdleTimeForBoth;
    long lastIdleTimeForRead;
    long lastIdleTimeForWrite;
    boolean deferDecreaseReadBuffer;// = true;

    /** An id generator guaranteed to generate unique IDs for the session */
    static EAtomicLLong idGenerator;// = new AtomicLong(0);

	static sp<EAttributeKey> READY_READ_FUTURES_KEY;// = new AttributeKey(AbstractIoSession.class, "readyReadFutures");

	static sp<EAttributeKey> WAITING_READ_FUTURES_KEY;// = new AttributeKey(AbstractIoSession.class, "waitingReadFutures");

	static sp<EIoFutureListener> SCHEDULED_COUNTER_RESETTER;

	/**
	 * @return a readFuture get from the waiting ReadFuture
	 */
	sp<EReadFuture> newReadFuture();

	/**
	 * @return a queue of ReadFuture
	 */
	sp<ConcurrentLinkedQueue<EReadFuture> > getReadyReadFutures();

	/**
	 * @return the queue of waiting ReadFuture
	 */
	sp<ConcurrentLinkedQueue<EReadFuture> > getWaitingReadFutures();

	/**
	 * Decrease the number of scheduled message written
	 */
	void decreaseScheduledWriteMessages();

	/**
	 * Get the Id as a String
	 */
	EString getIdAsString();

	/**
	 * TGet the Service name
	 */
	EString getServiceName();

	static void notifyIdleSession0(sp<EAbstractIoSession>& session, llong currentTime, llong idleTime, EIdleStatus status,
	            llong lastIoTime);
	static void notifyWriteTimeout(sp<EAbstractIoSession>& session, llong currentTime);
};

} /* namespace eio */
} /* namespace efc */

#endif /* EABSTRACTIOSESSION_HH_ */
