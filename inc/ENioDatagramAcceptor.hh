/*
 * ENioDatagramAcceptor.hh
 *
 *  Created on: 2016-8-30
 *      Author: cxxjava@163.com
 */

#ifndef ENIODATAGRAMACCEPTOR_HH_
#define ENIODATAGRAMACCEPTOR_HH_

#include "./EAbstractIoAcceptor.hh"
#include "./EDatagramAcceptor.hh"
#include "./EIoProcessor.hh"
#include "./ENioSession.hh"
#include "./EIoBuffer.hh"
#include "./EDefaultDatagramSessionConfig.hh"

namespace efc {
namespace eio {

/**
 * {@link IoAcceptor} for datagram transport (UDP/IP).
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @org.apache.xbean.XBean
 */

class ENioDatagramAcceptor: public EAbstractIoAcceptor,
		virtual public EDatagramAcceptor,
		virtual public EIoProcessor {
public:
	virtual ~ENioDatagramAcceptor();

	/**
	 * Creates a new instance.
	 */
	ENioDatagramAcceptor();

	/**
	 * Creates a new instance.
	 *
	 * @param executor The executor to use
	 */
	ENioDatagramAcceptor(EExecutorService* executor);

	/**
	 * {@inheritDoc}
	 */
	virtual EIoSessionRecycler* getSessionRecycler();

	/**
	 * {@inheritDoc}
	 */
	virtual void setSessionRecycler(EIoSessionRecycler* sessionRecycler);

	/**
	 * {@inheritDoc}
	 */
	virtual void add(sp<EIoSession> session);

	/**
	 * {@inheritDoc}
	 */
	virtual sp<ESet<EInetSocketAddress*> > bindInternal(EList<EInetSocketAddress*>& localAddresses) THROWS(EIOException);

	/**
	 * {@inheritDoc}
	 */
	virtual void flush(sp<EIoSession>& session);

	/**
	 * {@inheritDoc}
	 */
	virtual void setDefaultLocalAddress(EInetSocketAddress* localAddress);

	/**
	 * {@inheritDoc}
	 */
	virtual EInetSocketAddress* getDefaultLocalAddress();

	/**
	 * {@inheritDoc}
	 */
	virtual EInetSocketAddress* getLocalAddress();

	/**
	 * {@inheritDoc}
	 */
	virtual EDatagramSessionConfig* getSessionConfig();

	/**
	 * {@inheritDoc}
	 */
	virtual ETransportMetadata* getTransportMetadata();

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EIoSession> newSession(EInetSocketAddress *remoteAddress, EInetSocketAddress *localAddress);

	/**
	 * {@inheritDoc}
	 */
	virtual void remove(sp<EIoSession>& session);

	/**
	 * {@inheritDoc}
	 */
	virtual void unbind0(EList<EInetSocketAddress*>& localAddresses) THROWS(EIOException);

	/**
	 * {@inheritDoc}
	 */
	virtual void updateTrafficControl(sp<EIoSession>& session);

	/**
	 * {@inheritDoc}
	 */
	virtual void write(sp<EIoSession>& session, sp<EWriteRequest>& writeRequest);

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isDisposing();
	virtual boolean isDisposed();
	virtual void dispose();
	virtual void dispose(boolean awaitTermination);

protected:

	sp<EIoSession> newSession(EIoProcessor* processor, sp<EDatagramChannel> handle,
	            EInetSocketAddress* remoteAddress);

	sp<EInetSocketAddress> localAddress(sp<EDatagramChannel> handle) THROWS(EException);

	sp<EDatagramChannel> open(EInetSocketAddress* localAddress) THROWS(EException);

	void destroy() THROWS(EException);

	virtual void init() THROWS(EException);

	virtual void wakeup();

	virtual void close(sp<EDatagramChannel> handle) THROWS(EException);

	virtual void dispose0() THROWS(EException);

	virtual int select(llong timeout) THROWS(EException);
	virtual int select() THROWS(EException);

	virtual boolean isReadable(sp<EDatagramChannel> handle);
	virtual boolean isWritable(sp<EDatagramChannel> handle);

	virtual ESet<sp<ESelectionKey> >* selectedHandles();

	virtual sp<EInetSocketAddress> receive(sp<EDatagramChannel> handle, EIoBuffer* buffer) THROWS(EException);
	virtual int send(sp<ENioSession>& session, EIoBuffer* buffer, EInetSocketAddress* remoteAddress) THROWS(EException);

	virtual void setInterestedInWrite(sp<ENioSession>& session, boolean isInterested) THROWS(EException);

private:
	/**
	 * This private class is used to accept incoming connection from
	 * clients. It's an infinite loop, which can be stopped when all
	 * the registered handles have been removed (unbound).
	 */
	class Acceptor: public ERunnable {
	private:
		ENioDatagramAcceptor* nda;
	public:
		Acceptor(ENioDatagramAcceptor* a): nda(a) {
		}
		virtual void run() {
			nda->acceptorProcess();
		}
	};

private:
	friend class Acceptor;

	/**
	 * A session recycler that is used to retrieve an existing session, unless it's too old.
	 **/
	static EIoSessionRecycler* DEFAULT_RECYCLER;// = new ExpiringSessionRecycler();

	/**
	 * A timeout used for the select, as we need to get out to deal with idle
	 * sessions
	 */
	static const long SELECT_TIMEOUT = 1000L;

	/** A lock used to protect the selector to be waked up before it's created */
	ESemaphore lock;// = new Semaphore(1);

	/** A queue used to store the list of pending Binds */
	EConcurrentLiteQueue<AcceptorOperationFuture> registerQueue;// = new ConcurrentLinkedQueue<AcceptorOperationFuture>();

	EConcurrentLiteQueue<AcceptorOperationFuture> cancelQueue;// = new ConcurrentLinkedQueue<AcceptorOperationFuture>();

	EConcurrentLiteQueue<ENioSession> flushingSessions;// = new ConcurrentLinkedQueue<NioSession>();

	EMap<sp<EInetSocketAddress>, sp<EDatagramChannel> >* boundHandles;// = Collections.synchronizedMap(new HashMap<SocketAddress, DatagramChannel>());

	EIoSessionRecycler* sessionRecycler;// = DEFAULT_RECYCLER;

	ServiceOperationFuture* disposalFuture;// = new ServiceOperationFuture();

	volatile boolean selectable;

	/** The thread responsible of accepting incoming requests */
	Acceptor* acceptor;

	llong lastIdleCheckTime;

	/** The Selector used by this acceptor */
	ESelector* volatile selector;

	EDefaultDatagramSessionConfig* ddsc;

	/**
	 * Starts the inner Acceptor thread.
	 */
	void startupAcceptor() THROWS(EInterruptedException);

	void acceptorProcess();

	int registerHandles();

	int unregisterHandles();

	void processReadySessions(ESet<sp<ESelectionKey> >* handles);

	void flushSessions(llong currentTime);

	void notifyIdleSessions(llong currentTime);

	boolean scheduleFlush(sp<EIoSession>& session);

	void readHandle(sp<EDatagramChannel> handle) THROWS(EException);

	boolean flush(sp<ENioSession>& session, llong currentTime) THROWS(EException);

	sp<EIoSession> newSessionWithoutLock(EInetSocketAddress* remoteAddress, EInetSocketAddress* localAddress) THROWS(EIOException);
};

} /* namespace eio */
} /* namespace efc */
#endif /* ENIODATAGRAMACCEPTOR_HH_ */
