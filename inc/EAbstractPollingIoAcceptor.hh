/*
 * EAbstractPollingIoAcceptor.hh
 *
 *  Created on: 2016-1-8
 *      Author: cxxjava@163.com
 */

#ifndef EABSTRACTPOLLINGIOACCEPTOR_HH_
#define EABSTRACTPOLLINGIOACCEPTOR_HH_

#include "./EAbstractIoAcceptor.hh"
#include "./EAbstractIoSession.hh"
#include "./ESocketSessionConfig.hh"
#include "./EIoProcessor.hh"
#include "./EExceptionMonitor.hh"
#include "./ERuntimeIoException.hh"
#include "./ESimpleIoProcessorPool.hh"

namespace efc {
namespace eio {

/**
 * A base class for implementing transport using a polling strategy. The
 * underlying sockets will be checked in an active loop and woke up when an
 * socket needed to be processed. This class handle the logic behind binding,
 * accepting and disposing the server sockets. An {@link Executor} will be used
 * for running client accepting and an {@link AbstractPollingIoProcessor} will
 * be used for processing client I/O operations like reading, writing and
 * closing.
 *
 * All the low level methods for binding, accepting, closing need to be provided
 * by the subclassing implementation.
 *
 * @see NioSocketAcceptor for a example of implementation
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

template<typename H>
abstract class EAbstractPollingIoAcceptor: public EAbstractIoAcceptor {
public:
	virtual ~EAbstractPollingIoAcceptor() {
		if (this->createdProcessor) {
			delete processor;
		}
		delete boundHandles;
		delete disposalFuture;
        acceptorRef.set(null); //!!!
	}

	/**
	 * {@inheritDoc}
	 */
	sp<EIoSession> newSession(EInetSocketAddress* remoteAddress, EInetSocketAddress* localAddress) {
		throw EUnsupportedOperationException(__FILE__, __LINE__);
	}

	/**
	 * @return the backLog
	 */
	virtual int getBacklog() {
		return backlog;
	}

	/**
	 * Sets the Backlog parameter
	 *
	 * @param backlog
	 *            the backlog variable
	 */
	virtual void setBacklog(int backlog) {
		SYNCBLOCK(&bindLock) {
			if (isActive()) {
				throw EIllegalStateException(__FILE__, __LINE__, "backlog can't be set while the acceptor is bound.");
			}

			this->backlog = backlog;
        }}
	}

	/**
	 * @return the flag that sets the reuseAddress information
	 */
	virtual boolean isReuseAddress() {
		return reuseAddress;
	}

	/**
	 * Set the Reuse Address flag
	 *
	 * @param reuseAddress
	 *            The flag to set
	 */
	virtual void setReuseAddress(boolean reuseAddress) {
		SYNCBLOCK(&bindLock) {
			if (isActive()) {
				throw EIllegalStateException(__FILE__, __LINE__, "backlog can't be set while the acceptor is bound.");
			}

			this->reuseAddress = reuseAddress;
        }}
	}

	/**
	 * {@inheritDoc}
	 */
	virtual ESocketSessionConfig* getSessionConfig() {
		return dynamic_cast<ESocketSessionConfig*>(sessionConfig);
	}

protected:
	boolean reuseAddress;// = false;

	/** A flag set when the acceptor has been created and initialized */
	volatile boolean selectable;

    /**
     * Define the number of socket that can wait to be accepted. Default
     * to 50 (as in the SocketServer default).
     */
    int backlog;// = 50;

    /**
     * Constructor for {@link AbstractPollingIoAcceptor}. You need to provide a default
     * session configuration, a class of {@link IoProcessor} which will be instantiated in a
     * {@link SimpleIoProcessorPool} for better scaling in multiprocessor systems. The default
     * pool size will be used.
     *
     * @see SimpleIoProcessorPool
     *
     * @param sessionConfig
     *            the default configuration for the managed {@link IoSession}
     * @param processorClass a {@link Class} of {@link IoProcessor} for the associated {@link IoSession}
     *            type.
     */
    EAbstractPollingIoAcceptor(EIoSessionConfig* sessionConfig) :
    	EAbstractIoAcceptor(sessionConfig, null), lock(1) {
        ctor(sessionConfig, null, new ESimpleIoProcessorPool(), true);
    }

    /**
     * Constructor for {@link AbstractPollingIoAcceptor}. You need to provide a default
     * session configuration, a class of {@link IoProcessor} which will be instantiated in a
     * {@link SimpleIoProcessorPool} for using multiple thread for better scaling in multiprocessor
     * systems.
     *
     * @see SimpleIoProcessorPool
     *
     * @param sessionConfig
     *            the default configuration for the managed {@link IoSession}
     * @param processorClass a {@link Class} of {@link IoProcessor} for the associated {@link IoSession}
     *            type.
     * @param processorCount the amount of processor to instantiate for the pool
     */
    EAbstractPollingIoAcceptor(EIoSessionConfig* sessionConfig, int processorCount) :
    	EAbstractIoAcceptor(sessionConfig, null), lock(1) {
    	ctor(sessionConfig, null, new ESimpleIoProcessorPool(processorCount), true);
    }

    /**
     * Constructor for {@link AbstractPollingIoAcceptor}. You need to provide a default
     * session configuration, a default {@link Executor} will be created using
     * {@link Executors#newCachedThreadPool()}.
     *
     * @see AbstractIoService
     *
     * @param sessionConfig
     *            the default configuration for the managed {@link IoSession}
     * @param processor the {@link IoProcessor} for processing the {@link IoSession} of this transport, triggering
     *            events to the bound {@link IoHandler} and processing the chains of {@link IoFilter}
     */
	EAbstractPollingIoAcceptor(EIoSessionConfig* sessionConfig,
			EIoProcessor* processor) :
			EAbstractIoAcceptor(sessionConfig, null), lock(1) {
    	ctor(sessionConfig, null, processor, false);
    }

    /**
     * Constructor for {@link AbstractPollingIoAcceptor}. You need to provide a
     * default session configuration and an {@link Executor} for handling I/O
     * events. If a null {@link Executor} is provided, a default one will be
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
     *            the {@link IoProcessor} for processing the {@link IoSession}
     *            of this transport, triggering events to the bound
     *            {@link IoHandler} and processing the chains of
     *            {@link IoFilter}
     */
	EAbstractPollingIoAcceptor(EIoSessionConfig* sessionConfig,
			EExecutorService* executor, EIoProcessor* processor) :
			EAbstractIoAcceptor(sessionConfig, executor), lock(1) {
    	ctor(sessionConfig, executor, processor, false);
    }

	/**
	 * {@inheritDoc}
	 */
	void dispose0() THROWS(EException) {
		unbind();

		startupAcceptor();
		wakeup();
	}

	/**
	 * {@inheritDoc}
	 */
	sp<ESet<EInetSocketAddress*> > bindInternal(EList<EInetSocketAddress*>& localAddresses) THROWS(EException) {
		// Create a bind request as a Future operation. When the selector
		// have handled the registration, it will signal this future.
		sp<AcceptorOperationFuture> request(new AcceptorOperationFuture(localAddresses));

		// adds the Registration request to the queue for the Workers
		// to handle
		registerQueue.add(request);

		// creates the Acceptor instance and has the local
		// executor kick it off.
		startupAcceptor();

		// As we just started the acceptor, we have to unblock the select()
		// in order to process the bind request we just have added to the
		// registerQueue.
		SEMAPHORE_SYNCBLOCK(&lock) {
			wakeup();
        }}

		// Now, we wait until this request is completed.
		request->awaitUninterruptibly();
    
		/** @see:
		if (request.getException() != null) {
            throw request.getException();
        }
		 */
        request->tryThrowException();

		// Update the local addresses.
		// setLocalAddresses() shouldn't be called from the worker thread
		// because of deadlock.
		sp<ESet<EInetSocketAddress*> > newLocalAddresses = new EHashSet<EInetSocketAddress*>();

		sp<EIterator<H> > iter = boundHandles->values()->iterator();
		while (iter->hasNext()) {
			H handle = iter->next();
			newLocalAddresses->add(new EInetSocketAddress(*localAddress(handle)));
		}

		return newLocalAddresses;
	}

	/**
	 * {@inheritDoc}
	 */
	void unbind0(EList<EInetSocketAddress*>& localAddresses) THROWS(EException) {
		sp<AcceptorOperationFuture> future(new AcceptorOperationFuture(localAddresses));

		cancelQueue.add(future);
		startupAcceptor();
		wakeup();

		future->awaitUninterruptibly();
        /** @see:
		if (future->getException() != null) {
			throw future->getException();
		}
        */
        future->tryThrowException();
	}

	/**
	 * Initialize the polling system, will be called at construction time.
	 * @throws Exception any exception thrown by the underlying system calls
	 */
	virtual void init() THROWS(EException) = 0;

	/**
	 * Destroy the polling system, will be called when this {@link IoAcceptor}
	 * implementation will be disposed.
	 * @throws Exception any exception thrown by the underlying systems calls
	 */
	virtual void destroy() THROWS(EException) = 0;

	/**
	 * Check for acceptable connections, interrupt when at least a server is ready for accepting.
	 * All the ready server socket descriptors need to be returned by {@link #selectedHandles()}
	 * @return The number of sockets having got incoming client
	 * @throws Exception any exception thrown by the underlying systems calls
	 */
	virtual int select() THROWS(EException) = 0;

	/**
	 * Interrupt the {@link #select()} method. Used when the poll set need to be modified.
	 */
	virtual void wakeup() = 0;

	/**
	 * {@link Iterator} for the set of server sockets found with acceptable incoming connections
	 *  during the last {@link #select()} call.
	 * @return the list of server handles ready
	 */
	virtual sp<EIterator<H> > selectedHandles() = 0;

	/**
	 * Open a server socket for a given local address.
	 * @param localAddress the associated local address
	 * @return the opened server socket
	 * @throws Exception any exception thrown by the underlying systems calls
	 */
	virtual H open(EInetSocketAddress* localAddress) THROWS(EException) = 0;

	/**
	 * Get the local address associated with a given server socket
	 * @param handle the server socket
	 * @return the local {@link SocketAddress} associated with this handle
	 * @throws Exception any exception thrown by the underlying systems calls
	 */
	virtual EInetSocketAddress* localAddress(H handle) THROWS(EException) = 0;

	/**
	 * Accept a client connection for a server socket and return a new {@link IoSession}
	 * associated with the given {@link IoProcessor}
	 * @param processor the {@link IoProcessor} to associate with the {@link IoSession}
	 * @param handle the server handle
	 * @return the created {@link IoSession}
	 * @throws Exception any exception thrown by the underlying systems calls
	 */
	virtual sp<EIoSession> accept(EIoProcessor* processor, H handle) THROWS(EException) = 0;

	/**
	 * Close a server socket.
	 * @param handle the server socket
	 * @throws Exception any exception thrown by the underlying systems calls
	 */
	virtual void close(H handle) THROWS(EException) = 0;

private:
	/**
	 * This class is called by the startupAcceptor() method and is
	 * placed into a NamePreservingRunnable class.
	 * It's a thread accepting incoming connections from clients.
	 * The loop is stopped when all the bound handlers are unbound.
	 */
	class Acceptor : public ERunnable {
	private:
		EAbstractPollingIoAcceptor<H>* owner;

	public:
		Acceptor(EAbstractPollingIoAcceptor<H>* apia) : owner(apia) {
		}

		virtual void run() {
			ES_ASSERT (owner->acceptorRef.get() == this);

			int nHandles = 0;

			// Release the lock
			owner->lock.release();

			while (owner->selectable) {
				try {
					// Process the bound sockets to this acceptor.
					// this actually sets the selector to OP_ACCEPT,
					// and binds to the port on which this class will
					// listen on. We do that before the select because
					// the registerQueue containing the new handler is
					// already updated at this point.
					nHandles += owner->registerHandles();

					// Detect if we have some keys ready to be processed
					// The select() will be woke up if some new connection
					// have occurred, or if the selector has been explicitly
					// woke up
					int selected = owner->select();

					// Now, if the number of registred handles is 0, we can
					// quit the loop: we don't have any socket listening
					// for incoming connection.
					if (nHandles == 0) {
						owner->acceptorRef.set(null);

						if (owner->registerQueue.isEmpty() && owner->cancelQueue.isEmpty()) {
							ES_ASSERT (owner->acceptorRef.get() != this);
							break;
						}

						if (!owner->acceptorRef.compareAndSet(null, this)) {
							ES_ASSERT (owner->acceptorRef.get() != this);
							break;
						}

						ES_ASSERT (owner->acceptorRef.get() == this);
					}

					if (selected > 0) {
						// We have some connection request, let's process
						// them here.
						processHandles(owner->selectedHandles());
					}

					// check to see if any cancellation request has been made.
					nHandles -= owner->unregisterHandles();
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

			// Cleanup all the processors, and shutdown the acceptor.
			if (owner->selectable && owner->isDisposing()) {
				owner->selectable = false;
				{
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
		}

	private:
		/**
		 * This method will process new sessions for the Worker class.  All
		 * keys that have had their status updates as per the Selector.selectedKeys()
		 * method will be processed here.  Only keys that are ready to accept
		 * connections are handled here.
		 * <p/>
		 * Session objects are created by making new instances of SocketSessionImpl
		 * and passing the session object to the SocketIoProcessor class.
		 */
		void processHandles(sp<EIterator<H> > handles) THROWS(EException) {
			while (handles->hasNext()) {
				H handle = handles->next();
				handles->remove();

				// Associates a new created connection to a processor,
				// and get back a session
				sp<EAbstractIoSession> session = dynamic_pointer_cast<EAbstractIoSession>(owner->accept(owner->processor, handle));
				if (session == null) {
					continue;
				}
				owner->initSession(session, null, null);

				// add the session to the SocketIoProcessor
				session->getProcessor()->add(session);
			}
		}
	};

private:
	/** A lock used to protect the selector to be waked up before it's created */
	ESemaphore lock;// = new Semaphore(1);

	EIoProcessor* processor;

	boolean createdProcessor;

	EConcurrentLiteQueue<AcceptorOperationFuture> registerQueue;// = new ConcurrentLinkedQueue<AcceptorOperationFuture>();

	EConcurrentLiteQueue<AcceptorOperationFuture> cancelQueue;// = new ConcurrentLinkedQueue<AcceptorOperationFuture>();

	EMap<sp<EInetSocketAddress>, H>* boundHandles;// = Collections.synchronizedMap(new HashMap<SocketAddress, H>());

	ServiceOperationFuture* disposalFuture;// = new ServiceOperationFuture();

	/** The thread responsible of accepting incoming requests */
	EAtomicReference<Acceptor*> acceptorRef;// = new AtomicReference<Acceptor>();

	/**
	 * Constructor for {@link AbstractPollingIoAcceptor}. You need to provide a
	 * default session configuration and an {@link Executor} for handling I/O
	 * events. If a null {@link Executor} is provided, a default one will be
	 * created using {@link Executors#newCachedThreadPool()}.
	 *
	 * @see AbstractIoService(IoSessionConfig, Executor)
	 *
	 * @param sessionConfig
	 *            the default configuration for the managed {@link IoSession}
	 * @param executor
	 *            the {@link Executor} used for handling asynchronous execution
	 *            of I/O events. Can be <code>null</code>.
	 * @param processor
	 *            the {@link IoProcessor} for processing the {@link IoSession}
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

		this->boundHandles = ECollections::synchronizedMap(new EHashMap<sp<EInetSocketAddress>, H>());
		this->disposalFuture = new ServiceOperationFuture();

		/* moved to ENioSocketAcceptor::init()
		{
			ON_SCOPE_EXIT(
				if (!selectable) {
					try {
						destroy();
					} catch (EException& e) {
						EExceptionMonitor::getInstance()->exceptionCaught(e);
					}
				}
			);

			try {
				// Initialize the selector
				init();

				// The selector is now ready, we can switch the
				// flag to true so that incoming connection can be accepted
				selectable = true;
			} catch (ERuntimeException& e) {
				throw e;
			} catch (EException& e) {
				throw ERuntimeIoException(__FILE__, __LINE__, "Failed to initialize.");
			}
		}
		*/
	}

	/**
	 * This method is called by the doBind() and doUnbind()
	 * methods.  If the acceptor is null, the acceptor object will
	 * be created and kicked off by the executor.  If the acceptor
	 * object is null, probably already created and this class
	 * is now working, then nothing will happen and the method
	 * will just return.
	 */
	void startupAcceptor() THROWS(EInterruptedException) {
		// If the acceptor is not ready, clear the queues
		// TODO : they should already be clean : do we have to do that ?
		if (!selectable) {
			registerQueue.clear();
			cancelQueue.clear();
		}

		// start the acceptor if not already started
		Acceptor* acceptor = acceptorRef.get();

		if (acceptor == null) {
			lock.acquire();
			acceptor = new Acceptor(this);

			if (acceptorRef.compareAndSet(null, acceptor)) {
				executeWorker(acceptor);
			} else {
				delete acceptor;
				lock.release();
			}
		}
	}

	/**
	 * Sets up the socket communications.  Sets items such as:
	 * <p/>
	 * Blocking
	 * Reuse address
	 * Receive buffer size
	 * Bind to listen port
	 * Registers OP_ACCEPT for selector
	 */
	int registerHandles() {
		for (;;) {
			// The register queue contains the list of services to manage
			// in this acceptor.
			sp<AcceptorOperationFuture> future = registerQueue.poll();

			if (future == null) {
				return 0;
			}

			// We create a temporary map to store the bound handles,
			// as we may have to remove them all if there is an exception
			// during the sockets opening.
			sp<EHashMap<sp<EInetSocketAddress>, H> > newHandles = new EHashMap<sp<EInetSocketAddress>, H>();
			EList<EInetSocketAddress*>* localAddresses = future->getLocalAddresses();

			{
				ON_SCOPE_EXIT(
					// Roll back if failed to bind all addresses.
					if (future->getException() != null) {
						sp<EIterator<H> > iter = newHandles->values()->iterator();
						while (iter->hasNext()) {
							H handle = iter->next();
							try {
								close(handle);
							} catch (EException& e) {
								EExceptionMonitor::getInstance()->exceptionCaught(e);
							}
						}

						// Wake up the selector to be sure we will process the newly bound handle
						// and not block forever in the select()
						wakeup();
					}
				);

				try {
					// Process all the addresses
					/*
					for (SocketAddress a : localAddresses) {
						H handle = open(a);
						newHandles.put(localAddress(handle), handle);
					}
					*/
					sp<EIterator<EInetSocketAddress*> > iter = localAddresses->iterator();
					while (iter->hasNext()) {
						EInetSocketAddress* a = iter->next();
						H handle = open(a);
						newHandles->put(new EInetSocketAddress(*localAddress(handle)), handle);
					}

					// Everything went ok, we can now update the map storing
					// all the bound sockets.
					{
						//@see: boundHandles->putAll(newHandles);
						auto set = newHandles->entrySet();
						auto iter = set->iterator();
						while(iter->hasNext()) {
							auto me = iter->next();
							boundHandles->put(me->getKey(), me->getValue(), null);
						}
					}

					// and notify.
					future->setDone();
					return newHandles->size();
				} catch (EException& e) {
					// We store the exception in the future
					future->setException(new EThrowableObject<EException>(e));
				}
			}
		}
	}

	/**
	 * This method just checks to see if anything has been placed into the
	 * cancellation queue.  The only thing that should be in the cancelQueue
	 * is CancellationRequest objects and the only place this happens is in
	 * the doUnbind() method.
	 */
	int unregisterHandles() {
		int cancelledHandles = 0;
		for (;;) {
			sp<AcceptorOperationFuture> future = cancelQueue.poll();
			if (future == null) {
				break;
			}

			// close the channels
			//for (SocketAddress a : future.getLocalAddresses()) {
			sp<EIterator<EInetSocketAddress*> > iter = future->getLocalAddresses()->iterator();
			while (iter->hasNext()) {
				EInetSocketAddress* a = iter->next();
				H handle = boundHandles->remove(a);

				if (handle == null) {
					continue;
				}

				{
					ON_SCOPE_EXIT(
						cancelledHandles++;
					);

					try {
						close(handle);
						wakeup(); // wake up again to trigger thread death
					} catch (EException& e) {
						EExceptionMonitor::getInstance()->exceptionCaught(e);
					}
				}
			}

			future->setDone();
		}

		return cancelledHandles;
	}
};

} /* namespace eio */
} /* namespace efc */
#endif /* EABSTRACTPOLLINGIOACCEPTOR_HH_ */
