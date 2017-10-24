/*
 * EAbstractIoService.hh
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#ifndef EABSTRACTIOSERVICE_HH_
#define EABSTRACTIOSERVICE_HH_

#include "./EIoService.hh"
#include "./EIoSessionConfig.hh"
#include "./EIoSessionInitializer.hh"
#include "./EIoServiceStatistics.hh"
#include "./EIoFilterChainBuilder.hh"
#include "./EIoServiceListenerSupport.hh"
#include "./EDefaultIoFuture.hh"
#include "./EAbstractIoSession.hh"
#include "./EWriteFuture.hh"
#include "ELog.hh"

namespace efc {
namespace eio {

/**
 * Base implementation of {@link IoService}s.
 *
 * An instance of IoService contains an Executor which will handle the incoming
 * events.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */
abstract class EAbstractIoService: virtual public EIoService {
public:
	virtual ~EAbstractIoService() throw();

	/**
	 * {@inheritDoc}
	 */
	virtual void addListener(sp<EIoServiceListener> listener);

	/**
	 * {@inheritDoc}
	 */
	virtual void removeListener(EIoServiceListener* listener);

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
	 * {@inheritDoc}
	 */
	virtual void dispose(boolean awaitTermination);

	/**
	 * {@inheritDoc}
	 */
	virtual EIoHandler* getHandler();

	/**
	 * {@inheritDoc}
	 */
	virtual void setHandler(EIoHandler* handler);

	/**
	 * {@inheritDoc}
	 */
	virtual EConcurrentMap<llong, EIoSession>* getManagedSessions();

	/**
	 * {@inheritDoc}
	 */
	virtual int getManagedSessionCount();

	/**
	 * {@inheritDoc}
	 */
	virtual EIoFilterChainBuilder* getFilterChainBuilder();

	/**
	 * {@inheritDoc}
	 */
	virtual EDefaultIoFilterChainBuilder* getFilterChain();

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isActive();

	/**
	 * {@inheritDoc}
	 */
	virtual long getActivationTime();

	/**
	 * {@inheritDoc}
	 */
	virtual EArrayList<sp<EWriteFuture> > broadcast(sp<EObject> message);

	/**
	 * {@inheritDoc}
	 */
	virtual EIoSessionDataStructureFactory* getSessionDataStructureFactory();

	/**
	 * {@inheritDoc}
	 */
	virtual int getScheduledWriteBytes();

	/**
	 * {@inheritDoc}
	 */
	virtual int getScheduledWriteMessages();

	/**
	 * {@inheritDoc}
	 */
	virtual EIoServiceStatistics* getStatistics();

	/**
	 *
	 */
	virtual EIoServiceListenerSupport* getListeners();

protected:
	/**
	 * A specific class used to
	 * @author elecharny
	 *
	 */
	class ServiceOperationFuture : public EDefaultIoFuture {
	public:
		ServiceOperationFuture() : EDefaultIoFuture(null) {
		}

		boolean isDone() {
			EBoolean *b = dynamic_cast<EBoolean*>(getValue());
			return b ? b->booleanValue() : false;
		}

		void setDone() {
			setValue(new EBoolean(true));
		}

		EThrowable* getException() {
			EObject* obj = getValue();
			EThrowableType* tt = dynamic_cast<EThrowableType*>(obj);
			if (tt) {
				return tt->getThrowable();
			}
			return null;
		}
        
        void tryThrowException() {
            EObject* obj = getValue();
            EThrowableType* tt = dynamic_cast<EThrowableType*>(obj);
            if (tt) {
                tt->throwException();
            }
        }

		void setException(EObject* exception) {
			if (exception == null) {
				throw EIllegalArgumentException(__FILE__, __LINE__, "exception");
			}

			setValue(exception);
		}
	};

protected:
	/**
	 * The default {@link IoSessionConfig} which will be used to configure new sessions.
	 */
	EIoSessionConfig* sessionConfig;

	/**
	 * A lock object which must be acquired when related resources are
	 * destroyed.
	 */
	EReentrantLock disposalLock;// = new Object();

	/**
	 * Constructor for {@link AbstractIoService}. You need to provide a default
	 * session configuration and an {@link Executor} for handling I/O events. If
	 * a null {@link Executor} is provided, a default one will be created using
	 * {@link Executors#newCachedThreadPool()}.
	 *
	 * @param sessionConfig
	 *            the default configuration for the managed {@link IoSession}
	 * @param executor
	 *            the {@link Executor} used for handling execution of I/O
	 *            events. Can be <code>null</code>.
	 */
	EAbstractIoService(EIoSessionConfig* sessionConfig, EExecutorService* executor);

	/**
	 *
	 */
	void initSession(sp<EAbstractIoSession>& session, sp<EIoFuture> future, EIoSessionInitializer* sessionInitializer);

	/**
	 *
	 */
	void executeWorker(sp<ERunnable> worker);

	/**
	 *
	 */
	void executeWorker(sp<ERunnable> worker, const char* suffix);

	/**
	 * Implement this method to perform additional tasks required for session
	 * initialization. Do not call this method directly;
	 * {@link #initSession(IoSession, IoFuture, IoSessionInitializer)} will call
	 * this method instead.
	 */
	virtual void finishSessionInitialization0(sp<EAbstractIoSession>& session, sp<EIoFuture>& future);

	/**
	 * Implement this method to release any acquired resources.  This method
	 * is invoked only once by {@link #dispose()}.
	 *
	 * @throws Exception If the dispose failed
	 */
	virtual void dispose0() THROWS(EException) = 0;

private:
	static sp<ELogger> LOGGER;// = LoggerFactory.getLogger(AbstractIoService.class);

	/**
	 * The unique number identifying the Service. It's incremented
	 * for each new IoService created.
	 */
	static EAtomicInteger id;// = new AtomicInteger();

	/**
	 * The thread name built from the IoService inherited
	 * instance class name and the IoService Id
	 **/
	EString threadName;

	/**
	 * The associated executor, responsible for handling execution of I/O events.
	 */
	EExecutorService* executor;

	/**
	 * A flag used to indicate that the local executor has been created
	 * inside this instance, and not passed by a caller.
	 *
	 * If the executor is locally created, then it will be an instance
	 * of the ThreadPoolExecutor class.
	 */
	boolean createdExecutor;

	/**
	 * The IoHandler in charge of managing all the I/O Events. It is
	 */
	EIoHandler* handler;

	EIoServiceListener* serviceActivationListener;

	/**
	 * Current filter chain builder.
	 */
	EIoFilterChainBuilder* filterChainBuilder;// = new DefaultIoFilterChainBuilder();

	EIoSessionDataStructureFactory* sessionDataStructureFactory;// = new DefaultIoSessionDataStructureFactory();

	/**
	 * Maintains the {@link IoServiceListener}s of this service.
	 */
	EIoServiceListenerSupport* listeners;

	volatile boolean disposing;

	volatile boolean disposed;

	/**
	 * {@inheritDoc}
	 */
	EIoServiceStatistics* stats;// = new IoServiceStatistics(this);
};

} /* namespace eio */
} /* namespace efc */

#endif /* EABSTRACTIOSERVICE_HH_ */
