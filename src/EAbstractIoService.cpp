/*
 * EAbstractIoService.cpp
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#include "EAbstractIoService.hh"
#include "EExceptionMonitor.hh"
#include "EConnectFuture.hh"
#include "EDefaultIoFilterChain.hh"
#include "EDefaultIoFilterChainBuilder.hh"
#include "ENamePreservingRunnable.hh"
#include "EDefaultIoSessionDataStructureFactory.hh"
#include "EIoSessionInitializationException.hh"
#include "EIoUtil.hh"

namespace efc {
namespace eio {

sp<ELogger> EAbstractIoService::LOGGER = ELoggerManager::getLogger("EAbstractIoService");
EAtomicInteger EAbstractIoService::id;

class ServiceActivationListener : public EIoServiceListener {
public:
	void serviceActivated(EIoService* service) {
        // Update lastIoTime.
        EIoServiceStatistics* _stats = service->getStatistics();
        _stats->setLastReadTime(service->getActivationTime());
        _stats->setLastWriteTime(service->getActivationTime());
        _stats->setLastThroughputCalculationTime(service->getActivationTime());
    }

    void serviceDeactivated(EIoService* service) {
        // Empty handler
    }

    void serviceIdle(EIoService* service, EIdleStatus idleStatus) {
        // Empty handler
    }

    void sessionCreated(sp<EIoSession>& session) {
        // Empty handler
    }

    void sessionClosed(sp<EIoSession>& session) {
		// Empty handler
	}

    void sessionDestroyed(sp<EIoSession>& session) {
        // Empty handler
    }
};

EAbstractIoService::~EAbstractIoService() throw() {
	delete filterChainBuilder;
	delete sessionDataStructureFactory;
	delete stats;
	delete listeners;

	if (createdExecutor) {
		executor->shutdownNow();
		executor->awaitTermination();
		delete executor;
	}
}

EAbstractIoService::EAbstractIoService(EIoSessionConfig* sessionConfig,
		EExecutorService* executor) : handler(null), disposing(false), disposed(false) {
	if (sessionConfig == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "sessionConfig");
	}

    /*
	if (getTransportMetadata() == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "TransportMetadata");
	}

	if (!getTransportMetadata().getSessionConfigType().isAssignableFrom(sessionConfig.getClass())) {
		throw new IllegalArgumentException("sessionConfig type: " + sessionConfig.getClass() + " (expected: "
				+ getTransportMetadata().getSessionConfigType() + ")");
	}
	*/

	// Init some members.
	serviceActivationListener = new ServiceActivationListener();
	filterChainBuilder = new EDefaultIoFilterChainBuilder();
	sessionDataStructureFactory = new EDefaultIoSessionDataStructureFactory();
	stats = new EIoServiceStatistics(this);

	// Create the listeners, and add a first listener : a activation listener
	// for this service, which will give information on the service state.
	listeners = new EIoServiceListenerSupport(this);
	listeners->add(serviceActivationListener);

	// Stores the given session configuration
	this->sessionConfig = sessionConfig;

	// Make JVM load the exception monitor before some transports
	// change the thread context class loader.
	EExceptionMonitor::getInstance();

	if (executor == null) {
		this->executor = EExecutors::newCachedThreadPool();
		createdExecutor = true;
	} else {
		this->executor = executor;
		createdExecutor = false;
	}

	threadName = "EAbstractIoService-";
	threadName += id.incrementAndGet();
}

void EAbstractIoService::initSession(sp<EAbstractIoSession>& session, sp<EIoFuture> future, EIoSessionInitializer* sessionInitializer) {
	// cxxjava@163.com
	session->init();

	// Update lastIoTime if needed.
	if (stats->getLastReadTime() == 0) {
		stats->setLastReadTime(getActivationTime());
	}

	if (stats->getLastWriteTime() == 0) {
		stats->setLastWriteTime(getActivationTime());
	}

	// Every property but attributeMap should be set now.
	// Now initialize the attributeMap.  The reason why we initialize
	// the attributeMap at last is to make sure all session properties
	// such as remoteAddress are provided to IoSessionDataStructureFactory.
	try {
		session->setAttributeMap(session->getService()->getSessionDataStructureFactory()
				->getAttributeMap(session.get()));
	} catch (EIoSessionInitializationException& e) {
		throw e;
	} catch (EException& e) {
		EString msg("Failed to initialize an attributeMap.");
		msg << "\nCause:\n  " << e.getMessage();
		throw EIoSessionInitializationException(__FILE__, __LINE__, msg.c_str());
	}

	try {
		session->setWriteRequestQueue(session->getService()->getSessionDataStructureFactory()
				->getWriteRequestQueue(session.get()));
	} catch (EIoSessionInitializationException& e) {
		throw e;
	} catch (EException& e) {
		EString msg("Failed to initialize a writeRequestQueue.");
		msg << "\nCause:\n  " << e.getMessage();
		throw EIoSessionInitializationException(__FILE__, __LINE__, msg.c_str());
	}

	if ((future != null) && (dynamic_pointer_cast<EConnectFuture>(future) != null)) {
		// DefaultIoFilterChain will notify the future. (We support ConnectFuture only for now).
		session->setAttribute(EDefaultIoFilterChain::SESSION_CREATED_FUTURE, future);
	}

	if (sessionInitializer != null) {
		sessionInitializer->initializeSession(session, future);
	}

	finishSessionInitialization0(session, future);
}

void EAbstractIoService::addListener(sp<EIoServiceListener> listener) {
	listeners->add(listener);
}

void EAbstractIoService::removeListener(EIoServiceListener* listener) {
	listeners->remove(listener);
}

boolean EAbstractIoService::isDisposing() {
	return disposing;
}

boolean EAbstractIoService::isDisposed() {
	return disposed;
}

void EAbstractIoService::dispose() {
	dispose(false);
}

void EAbstractIoService::dispose(boolean awaitTermination) {
	if (disposed) {
		return;
	}

	SYNCBLOCK(&disposalLock) {
		if (!disposing) {
			disposing = true;

			try {
				dispose0();
			} catch (EException& e) {
				EExceptionMonitor::getInstance()->exceptionCaught(e);
			}
		}
    }}

	// cxxjava@163.com
	getFilterChain()->destroy();

	if (createdExecutor) {
		executor->shutdownNow();
		if (awaitTermination) {
			try {
				LOGGER->debug_("awaitTermination on %p called by thread=[%s]", this, EThread::currentThread()->getName());
				executor->awaitTermination(EInteger::MAX_VALUE, ETimeUnit::SECONDS);
				LOGGER->debug_("awaitTermination on %p finished", this);
			} catch (EInterruptedException& e1) {
				LOGGER->warn_("awaitTermination on [%p] was interrupted", this);
				// Restore the interrupted status
				EThread::currentThread()->interrupt();
			}
		}
	}
	disposed = true;
}

EIoHandler* EAbstractIoService::getHandler()
{
	return handler;
}

void EAbstractIoService::setHandler(EIoHandler* handler)
{
	if (handler == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "handler cannot be null");
	}

	if (isActive()) {
		throw EIllegalStateException(__FILE__, __LINE__,
				"handler cannot be set while the service is active.");
	}

	this->handler = handler;
}

EConcurrentMap<llong, EIoSession>* EAbstractIoService::getManagedSessions() {
	return listeners->getManagedSessions();
}

int EAbstractIoService::getManagedSessionCount() {
	return listeners->getManagedSessionCount();
}

EIoFilterChainBuilder* EAbstractIoService::getFilterChainBuilder() {
	return filterChainBuilder;
}

EDefaultIoFilterChainBuilder* EAbstractIoService::getFilterChain() {
	return dynamic_cast<EDefaultIoFilterChainBuilder*>(filterChainBuilder);
}

boolean EAbstractIoService::isActive() {
	return listeners->isActive();
}

long EAbstractIoService::getActivationTime() {
	return listeners->getActivationTime();
}

eal<EWriteFuture> EAbstractIoService::broadcast(sp<EObject> message) {
	/* @see:
	// Convert to Set.  We do not return a List here because only the
	// direct caller of MessageBroadcaster knows the order of write
	// operations.
	final List<WriteFuture> futures = IoUtil.broadcast(message, getManagedSessions().values());
	return new AbstractSet<WriteFuture>() {
		@Override
		public Iterator<WriteFuture> iterator() {
			return futures.iterator();
		}

		@Override
		public int size() {
			return futures.size();
		}
	};
	*/
	return EIoUtil::broadcast(message, getManagedSessions()->values());
}

EIoServiceListenerSupport* EAbstractIoService::getListeners() {
	return listeners;
}

EIoSessionDataStructureFactory* EAbstractIoService::getSessionDataStructureFactory() {
	return sessionDataStructureFactory;
}

int EAbstractIoService::getScheduledWriteBytes() {
	return stats->getScheduledWriteBytes();
}

int EAbstractIoService::getScheduledWriteMessages() {
	return stats->getScheduledWriteMessages();
}

EIoServiceStatistics* EAbstractIoService::getStatistics() {
	return stats;
}

void EAbstractIoService::executeWorker(sp<ERunnable> worker) {
	executeWorker(worker, null);
}

void EAbstractIoService::executeWorker(sp<ERunnable> worker, const char* suffix) {
	EString actualThreadName(threadName);
	if (suffix != null) {
		actualThreadName << '-' << suffix;
	}
	executor->execute(new ENamePreservingRunnable(worker, actualThreadName));
}

void EAbstractIoService::finishSessionInitialization0(sp<EAbstractIoSession>& session, sp<EIoFuture>& future) {
	// Do nothing. Extended class might add some specific code
}

} /* namespace eio */
} /* namespace efc */
