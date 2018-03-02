/*
 * EAbstractPollingIoConnector.hh
 *
 *  Created on: 2016-3-22
 *      Author: cxxjava@163.com
 */

#ifndef EABSTRACTPOLLINGIOCONNECTOR_HH_
#define EABSTRACTPOLLINGIOCONNECTOR_HH_

#include "./EAbstractIoConnector.hh"
#include "./EAbstractIoSession.hh"
#include "./ESocketSessionConfig.hh"
#include "./EIoProcessor.hh"
#include "./EExceptionMonitor.hh"
#include "./ERuntimeIoException.hh"
#include "./ESimpleIoProcessorPool.hh"
#include "./EDefaultConnectFuture.hh"

namespace efc {
namespace eio {

/**
 * A base class for implementing client transport using a polling strategy. The
 * underlying sockets will be checked in an active loop and woke up when an
 * socket needed to be processed. This class handle the logic behind binding,
 * connecting and disposing the client sockets. A {@link Executor} will be used
 * for running client connection, and an {@link AbstractPollingIoProcessor} will
 * be used for processing connected client I/O operations like reading, writing
 * and closing.
 *
 * All the low level methods for binding, connecting, closing need to be
 * provided by the subclassing implementation.
 *
 * @see NioSocketConnector for a example of implementation
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

template<typename H>
abstract class EAbstractPollingIoConnector: public EAbstractIoConnector {
public:
	virtual ~EAbstractPollingIoConnector() {
		if (createdProcessor) {
			delete processor;
		}
        connectorRef.set(null); //!!!
	}

	class ConnectionRequest : public EDefaultConnectFuture {
	private:
		EAbstractPollingIoConnector* owner;

		/** The handle associated with this connection request */
		H handle;

		/** The time up to this connection request will be valid */
		llong deadline;

		/** The callback to call when the session is initialized */
		EIoSessionInitializer* sessionInitializer;

	public:
		ConnectionRequest(H handle, EIoSessionInitializer* callback, EAbstractPollingIoConnector* s) : owner(s) {
			this->handle = handle;
			long timeout = owner->getConnectTimeoutMillis();

			if (timeout <= 0L) {
				this->deadline = ELLong::MAX_VALUE;
			} else {
				this->deadline = ESystem::currentTimeMillis() + timeout;
			}

			this->sessionInitializer = callback;
		}

		H getHandle() {
			return handle;
		}

		llong getDeadline() {
			return deadline;
		}

		EIoSessionInitializer* getSessionInitializer() {
			return sessionInitializer;
		}

		virtual boolean cancel() {
			if (!isDone()) {
				boolean justCancelled = EDefaultConnectFuture::cancel();

				// We haven't cancelled the request before, so add the future
				// in the cancel queue.
				if (justCancelled) {
					sp<ConnectionRequest> self = dynamic_pointer_cast<ConnectionRequest>(this->tosp());
					owner->cancelQueue.add(self);
					owner->startupWorker();
					owner->wakeup();
				}
			}

			return true;
		}
	};

protected:
	volatile boolean selectable;

	/**
	 * Constructor for {@link AbstractPollingIoConnector}. You need to provide a
	 * default session configuration, a class of {@link IoProcessor} which will
	 * be instantiated in a {@link SimpleIoProcessorPool} for better scaling in
	 * multiprocessor systems. The default pool size will be used.
	 *
	 * @see SimpleIoProcessorPool
	 *
	 * @param sessionConfig
	 *            the default configuration for the managed {@link IoSession}
	 * @param processorClass
	 *            a {@link Class} of {@link IoProcessor} for the associated
	 *            {@link IoSession} type.
	 */
	EAbstractPollingIoConnector(EIoSessionConfig* sessionConfig) :
			EAbstractIoConnector(sessionConfig, null) {
		ctor(sessionConfig, null, new ESimpleIoProcessorPool(), true);
	}

	/**
	 * Constructor for {@link AbstractPollingIoConnector}. You need to provide a
	 * default session configuration, a class of {@link IoProcessor} which will
	 * be instantiated in a {@link SimpleIoProcessorPool} for using multiple
	 * thread for better scaling in multiprocessor systems.
	 *
	 * @see SimpleIoProcessorPool
	 *
	 * @param sessionConfig
	 *            the default configuration for the managed {@link IoSession}
	 * @param processorClass
	 *            a {@link Class} of {@link IoProcessor} for the associated
	 *            {@link IoSession} type.
	 * @param processorCount
	 *            the amount of processor to instantiate for the pool
	 */
	EAbstractPollingIoConnector(EIoSessionConfig* sessionConfig, int processorCount) :
			EAbstractIoConnector(sessionConfig, null) {
		ctor(sessionConfig, null, new ESimpleIoProcessorPool(processorCount), true);
	}

	/**
	 * Constructor for {@link AbstractPollingIoConnector}. You need to provide a
	 * default session configuration, a default {@link Executor} will be created
	 * using {@link Executors#newCachedThreadPool()}.
	 *
	 * @see AbstractIoService#AbstractIoService(IoSessionConfig, Executor)
	 *
	 * @param sessionConfig
	 *            the default configuration for the managed {@link IoSession}
	 * @param processor
	 *            the {@link IoProcessor} for processing the {@link IoSession}
	 *            of this transport, triggering events to the bound
	 *            {@link IoHandler} and processing the chains of
	 *            {@link IoFilter}
	 */
	EAbstractPollingIoConnector(EIoSessionConfig* sessionConfig, EIoProcessor* processor) :
		EAbstractIoConnector(sessionConfig, null) {
		ctor(sessionConfig, null, processor, false);
	}

	/**
	 * Constructor for {@link AbstractPollingIoConnector}. You need to provide a
	 * default session configuration and an {@link Executor} for handling I/O
	 * events. If null {@link Executor} is provided, a default one will be
	 * created using {@link Executors#newCachedThreadPool()}.
	 *
	 * @see AbstractIoService#AbstractIoService(IoSessionConfig, Executor)
	 *
	 * @param sessionConfig
	 *            the default configuration for the managed {@link IoSession}
	 * @param executor
	 *            the {@link Executor} used for handling asynchronous execution
	 *            of I/O events. Can be <code>null</code>.
	 * @param processor
	 *            the {@link IoProcessor} for processing the {@link IoSession}
	 *            of this transport, triggering events to the bound
	 *            {@link IoHandler} and processing the chains of
	 *            {@link IoFilter}
	 */
	EAbstractPollingIoConnector(EIoSessionConfig* sessionConfig, EExecutorService* executor, EIoProcessor* processor) :
		EAbstractIoConnector(sessionConfig, executor) {
		ctor(sessionConfig, executor, processor, false);
	}

	/**
	 * {@inheritDoc}
	 */
	void dispose0() THROWS(EException) {
		startupWorker();
		wakeup();
	}

	virtual sp<EConnectFuture> connect0(EInetSocketAddress* remoteAddress, EInetSocketAddress* localAddress,
					EIoSessionInitializer* sessionInitializer) {
		H handle = null;
		boolean success = false;
		ON_FINALLY_NOTHROW(
			if (!success && handle != null) {
				try {
					close(handle);
				} catch (EException& e) {
					EExceptionMonitor::getInstance()->exceptionCaught(e);
				}
			}
		) {
			try {
				handle = newHandle(localAddress);
				if (connect(handle, remoteAddress)) {
					sp<EConnectFuture> future = new EDefaultConnectFuture();
					sp<EAbstractIoSession> session = dynamic_pointer_cast<EAbstractIoSession>(newSession(processor, handle));
					initSession(session, future, sessionInitializer);
					// Forward the remaining process to the IoProcessor.
					session->getProcessor()->add(session);
					success = true;
					return future;
				}

				success = true;
			} catch (EException& e) {
				return EDefaultConnectFuture::newFailedFuture(new EThrowableObject<EException>(e));
			}
        }}

		sp<ConnectionRequest> request = new ConnectionRequest(handle, sessionInitializer, this);
		connectQueue.add(request);
		startupWorker();
		wakeup();

		return dynamic_pointer_cast<EConnectFuture>(request);
	}

	/**
	 * Initialize the polling system, will be called at construction time.
	 *
	 * @throws Exception
	 *             any exception thrown by the underlying system calls
	 */
	virtual void init() THROWS(EException) = 0;

	/**
	 * Destroy the polling system, will be called when this {@link IoConnector}
	 * implementation will be disposed.
	 *
	 * @throws Exception
	 *             any exception thrown by the underlying systems calls
	 */
	virtual void destroy() THROWS(EException) = 0;

	/**
	 * Create a new client socket handle from a local {@link SocketAddress}
	 *
	 * @param localAddress
	 *            the socket address for binding the new client socket
	 * @return a new client socket handle
	 * @throws Exception
	 *             any exception thrown by the underlying systems calls
	 */
	virtual H newHandle(EInetSocketAddress* localAddress) THROWS(EException) = 0;

	/**
	 * Connect a newly created client socket handle to a remote
	 * {@link SocketAddress}. This operation is non-blocking, so at end of the
	 * call the socket can be still in connection process.
	 *
	 * @param handle the client socket handle
	 * @param remoteAddress the remote address where to connect
	 * @return <tt>true</tt> if a connection was established, <tt>false</tt> if
	 *         this client socket is in non-blocking mode and the connection
	 *         operation is in progress
	 * @throws Exception If the connect failed
	 */
	virtual boolean connect(H handle, EInetSocketAddress* remoteAddress) THROWS(EException) = 0;

	/**
	 * Finish the connection process of a client socket after it was marked as
	 * ready to process by the {@link #select(int)} call. The socket will be
	 * connected or reported as connection failed.
	 *
	 * @param handle
	 *            the client socket handle to finish to connect
	 * @return true if the socket is connected
	 * @throws Exception
	 *             any exception thrown by the underlying systems calls
	 */
	virtual boolean finishConnect(H handle) THROWS(EException) = 0;

	/**
	 * Create a new {@link IoSession} from a connected socket client handle.
	 * Will assign the created {@link IoSession} to the given
	 * {@link IoProcessor} for managing future I/O events.
	 *
	 * @param processor
	 *            the processor in charge of this session
	 * @param handle
	 *            the newly connected client socket handle
	 * @return a new {@link IoSession}
	 * @throws Exception
	 *             any exception thrown by the underlying systems calls
	 */
	virtual sp<EIoSession> newSession(EIoProcessor* processor, H handle) THROWS(EException) = 0;

	/**
	 * Close a client socket.
	 *
	 * @param handle
	 *            the client socket
	 * @throws Exception
	 *             any exception thrown by the underlying systems calls
	 */
	virtual void close(H handle) THROWS(EException) = 0;

	/**
	 * Interrupt the {@link #select(int)} method. Used when the poll set need to
	 * be modified.
	 */
	virtual void wakeup() = 0;

	/**
	 * Check for connected sockets, interrupt when at least a connection is
	 * processed (connected or failed to connect). All the client socket
	 * descriptors processed need to be returned by {@link #selectedHandles()}
	 *
	 * @param timeout The timeout for the select() method
	 * @return The number of socket having received some data
	 * @throws Exception any exception thrown by the underlying systems calls
	 */
	virtual int select(int timeout) THROWS(EException) = 0;

	/**
	 * {@link Iterator} for the set of client sockets found connected or failed
	 * to connect during the last {@link #select(int)} call.
	 *
	 * @return the list of client socket handles to process
	 */
	virtual sp<EIterator<H> > selectedHandles() = 0;

	/**
	 * {@link Iterator} for all the client sockets polled for connection.
	 *
	 * @return the list of client sockets currently polled for connection
	 */
	virtual sp<EIterator<H> > allHandles() = 0;

	/**
	 * Register a new client socket for connection, add it to connection polling
	 *
	 * @param handle
	 *            client socket handle
	 * @param request
	 *            the associated {@link ConnectionRequest}
	 * @throws Exception
	 *             any exception thrown by the underlying systems calls
	 */
	virtual void register_(H handle, sp<ConnectionRequest> request) THROWS(EException) = 0;

	/**
	 * get the {@link ConnectionRequest} for a given client socket handle
	 *
	 * @param handle
	 *            the socket client handle
	 * @return the connection request if the socket is connecting otherwise
	 *         <code>null</code>
	 */
	virtual sp<ConnectionRequest> getConnectionRequest(H handle) = 0;

private:
	class Connector : public ERunnable {
	private:
		EAbstractPollingIoConnector<H>* owner;

	public:
		Connector(EAbstractPollingIoConnector<H>* s) : owner(s) {
		}

		virtual void run() {
			ES_ASSERT (owner->connectorRef.get() == this);

			int nHandles = 0;

			while (owner->selectable) {
				try {
					// the timeout for select shall be smaller of the connect
					// timeout or 1 second...
					int timeout = (int) ES_MIN(owner->getConnectTimeoutMillis(), 1000L);
					int selected = owner->select(timeout);

					nHandles += owner->registerNew();

					// get a chance to get out of the connector loop, if we
					// don't have any more handles
					if (nHandles == 0) {
						owner->connectorRef.set(null);

						if (owner->connectQueue.isEmpty()) {
							ES_ASSERT (owner->connectorRef.get() != this);
							break;
						}

						if (!owner->connectorRef.compareAndSet(null, this)) {
							ES_ASSERT (owner->connectorRef.get() != this);
							break;
						}

						ES_ASSERT (owner->connectorRef.get() == this);
					}

					if (selected > 0) {
						nHandles -= owner->processConnections(owner->selectedHandles());
					}

					owner->processTimedOutSessions(owner->allHandles());

					nHandles -= owner->cancelKeys();
				} catch (EClosedSelectorException& cse) {
					// If the selector has been closed, we can exit the loop
					EExceptionMonitor::getInstance()->exceptionCaught(cse);
					break;
				} catch (EException& e) {
					EExceptionMonitor::getInstance()->exceptionCaught(e);

					try {
						EThread::sleep(1000);
					} catch (EInterruptedException& e1) {
						EExceptionMonitor::getInstance()->exceptionCaught(e1);
					}
				}
			}

			if (owner->selectable && owner->isDisposing()) {
				owner->selectable = false;
				ON_FINALLY_NOTHROW(
					ON_FINALLY_NOTHROW(
						owner->disposalFuture->setDone();
					) {
						try {
							SYNCBLOCK(&owner->disposalLock) {
								if (owner->isDisposing()) {
									owner->destroy();
								}
                            }}
						} catch (EException& e) {
							EExceptionMonitor::getInstance()->exceptionCaught(e);
						}
                    }}
				) {
					if (owner->createdProcessor) {
						owner->processor->dispose();
					}
                }}
			}
		}
	};

private:
	EIoProcessor* processor;

	boolean createdProcessor;

	EConcurrentLiteQueue<ConnectionRequest> connectQueue;// = new ConcurrentLinkedQueue<ConnectionRequest>();

	EConcurrentLiteQueue<ConnectionRequest> cancelQueue;// = new ConcurrentLinkedQueue<ConnectionRequest>();

	sp<ServiceOperationFuture> disposalFuture;// = new ServiceOperationFuture();

	/** The connector thread */
	EAtomicReference<Connector*> connectorRef;// = new AtomicReference<Connector>();

	/**
	 * Constructor for {@link AbstractPollingIoAcceptor}. You need to provide a
	 * default session configuration and an {@link Executor} for handling I/O
	 * events. If null {@link Executor} is provided, a default one will be
	 * created using {@link Executors#newCachedThreadPool()}.
	 *
	 * @see AbstractIoService#AbstractIoService(IoSessionConfig, Executor)
	 *
	 * @param sessionConfig
	 *            the default configuration for the managed {@link IoSession}
	 * @param executor
	 *            the {@link Executor} used for handling asynchronous execution
	 *            of I/O events. Can be <code>null</code>.
	 * @param processor
	 *            the {@link IoProcessor} for processing the {@link IoSession}
	 *            of this transport, triggering events to the bound
	 *            {@link IoHandler} and processing the chains of
	 *            {@link IoFilter}
	 * @param createdProcessor
	 *            tagging the processor as automatically created, so it will be
	 *            automatically disposed
	 */
	void ctor(EIoSessionConfig* sessionConfig, EExecutorService* executor, EIoProcessor* processor,
			boolean createdProcessor) {

		if (processor == null) {
			throw EIllegalArgumentException(__FILE__, __LINE__, "processor");
		}

		this->processor = processor;
		this->createdProcessor = createdProcessor;

		this->disposalFuture = new ServiceOperationFuture();

		/* moved to ENioSocketConnector::init()
		ON_FINALLY_NOTHROW(
			if (!selectable) {
				try {
					destroy();
				} catch (EException& e) {
					EExceptionMonitor::getInstance()->exceptionCaught(e);
				}
			}
		) {
			try {
				init();
				selectable = true;
			} catch (ERuntimeException& e) {
				throw e;
			} catch (EException& e) {
				throw ERuntimeIoException(__FILE__, __LINE__, "Failed to initialize.");
			}
		}}
		*/
	}

	void startupWorker() {
        if (!selectable) {
            connectQueue.clear();
            cancelQueue.clear();
        }

        Connector* connector = connectorRef.get();

        if (connector == null) {
            connector = new Connector(this);

            if (connectorRef.compareAndSet(null, connector)) {
                executeWorker(connector);
            } else {
            	delete connector;
            }
        }
    }

    int registerNew() {
        int nHandles = 0;
        for (;;) {
            sp<ConnectionRequest> req = connectQueue.poll();
            if (req == null) {
                break;
            }

            H handle = req->getHandle();
            try {
                register_(handle, req);
                nHandles++;
            } catch (EException& e) {
                req->setException(new EThrowableObject<EException>(e));
                try {
                    close(handle);
                } catch (EException& e2) {
                    EExceptionMonitor::getInstance()->exceptionCaught(e2);
                }
            }
        }
        return nHandles;
    }

    int cancelKeys() {
        int nHandles = 0;

        for (;;) {
            sp<ConnectionRequest> req = cancelQueue.poll();

            if (req == null) {
                break;
            }

            H handle = req->getHandle();

            ON_FINALLY_NOTHROW(
            	nHandles++;
            ) {
				try {
					close(handle);
				} catch (EException& e) {
					EExceptionMonitor::getInstance()->exceptionCaught(e);
				}
            }}
        }

        if (nHandles > 0) {
            wakeup();
        }

        return nHandles;
    }

    /**
     * Process the incoming connections, creating a new session for each valid
     * connection.
     */
    int processConnections(sp<EIterator<H> > handlers) {
        int nHandles = 0;

        // Loop on each connection request
        while (handlers->hasNext()) {
            H handle = handlers->next();
            handlers->remove();

            sp<ConnectionRequest> connectionRequest = getConnectionRequest(handle);

            if (connectionRequest == null) {
                continue;
            }

            boolean success = false;
            ON_FINALLY_NOTHROW(
				if (!success) {
					// The connection failed, we have to cancel it.
					cancelQueue.offer(connectionRequest);
				}
            ) {
				try {
					if (finishConnect(handle)) {
						sp<EAbstractIoSession> session = dynamic_pointer_cast<EAbstractIoSession>(newSession(processor, handle));
						initSession(session, connectionRequest, connectionRequest->getSessionInitializer());
						// Forward the remaining process to the IoProcessor.
						session->getProcessor()->add(session);
						nHandles++;
					}
					success = true;
				} catch (EException& e) {
					connectionRequest->setException(new EThrowableObject<EException>(e));
				}
            }}
        }
        return nHandles;
    }

    void processTimedOutSessions(sp<EIterator<H> > handles) {
    	llong currentTime = ESystem::currentTimeMillis();

        while (handles->hasNext()) {
            H handle = handles->next();
            sp<ConnectionRequest> connectionRequest = getConnectionRequest(handle);

            if ((connectionRequest != null) && (currentTime >= connectionRequest->getDeadline())) {
            	EConnectException e(__FILE__, __LINE__, "Connection timed out.");
                connectionRequest->setException(new EThrowableObject<EConnectException>(e));
                cancelQueue.offer(connectionRequest);
            }
        }
    }
};

} /* namespace eio */
} /* namespace efc */
#endif /* EABSTRACTPOLLINGIOCONNECTOR_HH_ */
