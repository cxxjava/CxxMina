/*
 * EAbstractPollingIoProcessor.hh
 *
 *  Created on: 2016-3-9
 *      Author: cxxjava@163.com
 */

#ifndef EABSTRACTPOLLINGIOPROCESSOR_HH_
#define EABSTRACTPOLLINGIOPROCESSOR_HH_

#include "./EIoProcessor.hh"
#include "./ESessionState.hh"
#include "./ENioSession.hh"
#include "./EIoBuffer.hh"
#include "./EFileRegion.hh"
#include "./EDefaultIoFuture.hh"
#include "ELog.hh"

namespace efc {
namespace eio {

/**
 * An abstract implementation of {@link IoProcessor} which helps transport
 * developers to write an {@link IoProcessor} easily. This class is in charge of
 * active polling a set of {@link IoSession} and trigger events when some I/O
 * operation is possible.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 *
 * @param <S>
 *            the type of the {@link IoSession} this processor can handle
 */

class EAbstractPollingIoProcessor: public EIoProcessor {
public:
	virtual ~EAbstractPollingIoProcessor();

	/**
	 * {@inheritDoc}
	 */
	virtual void add(sp<EIoSession> session);

	/**
	 * {@inheritDoc}
	 */
	virtual void remove(sp<EIoSession>& session);

	/**
	 * {@inheritDoc}
	 */
	virtual void write(sp<EIoSession>& session, sp<EWriteRequest>& writeRequest);

	/**
	 * {@inheritDoc}
	 */
	virtual void flush(sp<EIoSession>& session);

	/**
	 * {@inheritDoc}
	 */
	virtual void updateTrafficControl(sp<EIoSession>& session);

	/**
	 * Updates the traffic mask for a given session
	 *
	 * @param session the session to update
	 */
	void updateTrafficMask(sp<ENioSession>& session);

protected:
	EAtomicBoolean wakeupCalled;// = new AtomicBoolean(false);

	/**
	 * Create an {@link AbstractPollingIoProcessor} with the given
	 * {@link Executor} for handling I/Os events.
	 *
	 * @param executor
	 *            the {@link Executor} for handling I/O events
	 */
	EAbstractPollingIoProcessor(EExecutor* executor);

	/**
	 * Initialize the polling of a session. Add it to the polling process.
	 *
	 * @param session the {@link IoSession} to add to the polling
	 * @throws Exception any exception thrown by the underlying system calls
	 */
	virtual void init(sp<ENioSession>& session) THROWS(EException) = 0;

	/**
	 * Destroy the underlying client socket handle
	 *
	 * @param session the {@link IoSession}
	 * @throws Exception any exception thrown by the underlying system calls
	 */
	virtual void destroy(sp<ENioSession>& session) THROWS(EException) = 0;

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isDisposing();

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isDisposed();

	/**
	 * {@inheritDoc}
	 */
	virtual void dispose();

	/**
	 * Dispose the resources used by this {@link IoProcessor} for polling the
	 * client connections. The implementing class doDispose method will be
	 * called.
	 *
	 * @throws Exception
	 *             if some low level IO error occurs
	 */
	virtual void doDispose() THROWS(EException) = 0;

	/**
	 * poll those sessions for the given timeout
	 *
	 * @param timeout
	 *            milliseconds before the call timeout if no event appear
	 * @return The number of session ready for read or for write
	 * @throws Exception
	 *             if some low level IO error occurs
	 */
	virtual int select(llong timeout) THROWS(EException) = 0;

	/**
	 * poll those sessions forever
	 *
	 * @return The number of session ready for read or for write
	 * @throws Exception
	 *             if some low level IO error occurs
	 */
	virtual int select() THROWS(EException) = 0;

	/**
	 * Say if the list of {@link IoSession} polled by this {@link IoProcessor}
	 * is empty
	 *
	 * @return <tt>true</tt> if at least a session is managed by this {@link IoProcessor}
	 */
	virtual boolean isSelectorEmpty() = 0;

	/**
	 * Interrupt the {@link #select(long)} call.
	 */
	virtual void wakeup() = 0;

	/**
	 * Get an {@link Iterator} for the list of {@link IoSession} polled by this
	 * {@link IoProcessor}
	 *
	 * @return {@link Iterator} of {@link IoSession}
	 */
	virtual sp<EIterator<sp<ENioSession> > > allSessions() = 0;

	/**
	 * Get an {@link Iterator} for the list of {@link IoSession} found selected
	 * by the last call of {@link #select(long)}
	 *
	 * @return {@link Iterator} of {@link IoSession} read for I/Os operation
	 */
	virtual sp<EIterator<sp<ENioSession> > > selectedSessions() = 0;

	/**
	 * Get the state of a session (One of OPENING, OPEN, CLOSING)
	 *
	 * @param session the {@link IoSession} to inspect
	 * @return the state of the session
	 */
	virtual ESessionState getState(sp<ENioSession>& session) = 0;

	/**
	 * Tells if the session ready for writing
	 *
	 * @param session the queried session
	 * @return <tt>true</tt> is ready, <tt>false</tt> if not ready
	 */
	virtual boolean isWritable(sp<ENioSession>& session) = 0;

	/**
	 * Tells if the session ready for reading
	 *
	 * @param session the queried session
	 * @return <tt>true</tt> is ready, <tt>false</tt> if not ready
	 */
	virtual boolean isReadable(sp<ENioSession>& session) = 0;

	/**
	 * Set the session to be informed when a write event should be processed
	 *
	 * @param session the session for which we want to be interested in write events
	 * @param isInterested <tt>true</tt> for registering, <tt>false</tt> for removing
	 * @throws Exception If there was a problem while registering the session
	 */
	virtual void setInterestedInWrite(sp<ENioSession>& session, boolean isInterested) THROWS(EException) = 0;

	/**
	 * Set the session to be informed when a read event should be processed
	 *
	 * @param session the session for which we want to be interested in read events
	 * @param isInterested <tt>true</tt> for registering, <tt>false</tt> for removing
	 * @throws Exception If there was a problem while registering the session
	 */
	virtual void setInterestedInRead(sp<ENioSession>& session, boolean isInterested) THROWS(EException) = 0;

	/**
	 * Tells if this session is registered for reading
	 *
	 * @param session the queried session
	 * @return <tt>true</tt> is registered for reading
	 */
	virtual boolean isInterestedInRead(sp<ENioSession>& session) = 0;

	/**
	 * Tells if this session is registered for writing
	 *
	 * @param session the queried session
	 * @return <tt>true</tt> is registered for writing
	 */
	virtual boolean isInterestedInWrite(sp<ENioSession>& session) = 0;

	/**
	 * Reads a sequence of bytes from a {@link IoSession} into the given
	 * {@link IoBuffer}. Is called when the session was found ready for reading.
	 *
	 * @param session the session to read
	 * @param buf the buffer to fill
	 * @return the number of bytes read
	 * @throws Exception any exception thrown by the underlying system calls
	 */
	virtual int read(sp<ENioSession>& session, EIoBuffer* buf) THROWS(EException) = 0;

	/**
	 * Write a sequence of bytes to a {@link IoSession}, means to be called when
	 * a session was found ready for writing.
	 *
	 * @param session the session to write
	 * @param buf the buffer to write
	 * @param length the number of bytes to write can be superior to the number of
	 *            bytes remaining in the buffer
	 * @return the number of byte written
	 * @throws Exception any exception thrown by the underlying system calls
	 */
	virtual int write(sp<ENioSession>& session, EIoBuffer* buf, int length) THROWS(EException) = 0;

	/**
	 * Write a part of a file to a {@link IoSession}, if the underlying API
	 * isn't supporting system calls like sendfile(), you can throw a
	 * {@link UnsupportedOperationException} so the file will be send using
	 * usual {@link #write(AbstractIoSession, IoBuffer, int)} call.
	 *
	 * @param session the session to write
	 * @param region the file region to write
	 * @param length the length of the portion to send
	 * @return the number of written bytes
	 * @throws Exception any exception thrown by the underlying system calls
	 */
	virtual int transferFile(sp<ENioSession>& session, EFileRegion* region, int length) THROWS(EException) = 0;

	/**
	 * In the case we are using the java select() method, this method is used to
	 * trash the buggy selector and create a new one, registring all the sockets
	 * on it.
	 *
	 * @throws IOException If we got an exception
	 */
	virtual void registerNewSelector() THROWS(EException) = 0;

	/**
	 * Check that the select() has not exited immediately just because of a
	 * broken connection. In this case, this is a standard case, and we just
	 * have to loop.
	 *
	 * @return <tt>true</tt> if a connection has been brutally closed.
	 * @throws IOException If we got an exception
	 */
	virtual boolean isBrokenConnection() THROWS(EException) = 0;

private:
	/**
	 * The main loop. This is the place in charge to poll the Selector, and to
	 * process the active sessions. It's done in - handle the newly created
	 * sessions -
	 */
	class Processor : public ERunnable {
	public:
		Processor(EAbstractPollingIoProcessor* p);
		virtual void run();
	private:
		EAbstractPollingIoProcessor* owner;
	};

private:
	friend class Processor;

	/** A logger for this class */
	static sp<ELogger> LOG;// = LoggerFactory.getLogger(IoProcessor.class);

	/**
	 * A timeout used for the select, as we need to get out to deal with idle
	 * sessions
	 */
	static const long SELECT_TIMEOUT = 1000L;

	/** This IoProcessor instance name */
	EString threadName;

	/** The executor to use when we need to start the inner Processor */
	EExecutor* executor;

	/** A Session queue containing the newly created sessions */
	EConcurrentLiteQueue<ENioSession> newSessions;// = new ConcurrentLinkedQueue<S>();

	/** A queue used to store the sessions to be removed */
	EConcurrentLiteQueue<ENioSession> removingSessions;// = new ConcurrentLinkedQueue<S>();

	/** A queue used to store the sessions to be flushed */
	EConcurrentLiteQueue<ENioSession> flushingSessions;// = new ConcurrentLinkedQueue<S>();

	/**
	 * A queue used to store the sessions which have a trafficControl to be
	 * updated
	 */
	EConcurrentLiteQueue<ENioSession> trafficControllingSessions;// = new ConcurrentLinkedQueue<S>();

	/** The processor thread : it handles the incoming messages */
	EAtomicReference<Processor*> processorRef;// = new AtomicReference<Processor>();

	llong lastIdleCheckTime;

	EReentrantLock disposalLock;// = new Object();

	volatile boolean disposing;

	volatile boolean disposed;

	EDefaultIoFuture* disposalFuture;// = new DefaultIoFuture(null);

	/**
	 * Compute the thread ID for this class instance. As we may have different
	 * classes, we store the last ID number into a Map associating the class
	 * name to the last assigned ID.
	 *
	 * @return a name for the current thread, based on the class name and an
	 *         incremental value, starting at 1.
	 */
	EString nextThreadName();

	/**
	 * Starts the inner Processor, asking the executor to pick a thread in its
	 * pool. The Runnable will be renamed
	 */
	void startupProcessor();

	/**
	 * Loops over the new sessions blocking queue and returns the number of
	 * sessions which are effectively created
	 *
	 * @return The number of new sessions
	 */
	int handleNewSessions();

	/**
	 * Process a new session : - initialize it - create its chain - fire the
	 * CREATED listeners if any
	 *
	 * @param session The session to create
	 * @return <tt>true</tt> if the session has been registered
	 */
	boolean addNow(sp<ENioSession>& session);

	/**
	 * Update the trafficControl for all the session.
	 */
	void updateTrafficMask();

	/**
	 * Write all the pending messages
	 */
	void flush(llong currentTime);

	/**
	 * Deal with session ready for the read or write operations, or both.
	 */
	void process(sp<ENioSession> session);

	/**
	 *
	 */
	void read(sp<ENioSession>& session);

	void scheduleRemove(sp<ENioSession>& session);
	void process() THROWS(EException);
	int removeSessions();
	boolean removeNow(sp<ENioSession>& session);
	void clearWriteRequestQueue(sp<ENioSession>& session);
	void notifyIdleSessions(llong currentTime) THROWS(EException);
	boolean flushNow(sp<ENioSession>& session, llong currentTime);
	void scheduleFlush(sp<ENioSession>& session);
	void fireMessageSent(sp<ENioSession>& session, sp<EWriteRequest>& req);
	int writeBuffer(sp<ENioSession>& session, sp<EWriteRequest>& req, boolean hasFragmentation, int maxLength, llong currentTime) THROWS(EException);
	int writeFile(sp<ENioSession>& session, sp<EWriteRequest>& req, boolean hasFragmentation, int maxLength, llong currentTime) THROWS(EException);
};

} /* namespace eio */
} /* namespace efc */
#endif /* EABSTRACTPOLLINGIOPROCESSOR_HH_ */
