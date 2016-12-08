/*
 * EIoServiceListenerSupport.cpp
 *
 *  Created on: 2014-9-15
 *      Author: cxxjava@163.com
 */

#include "EIoServiceListenerSupport.hh"
#include "EIoFutureListener.hh"
#include "EIoFilterChain.hh"
#include "EIoAcceptor.hh"
#include "ECloseFuture.hh"
#include "EIoConnector.hh"
#include "EExceptionMonitor.hh"
#include "EDefaultIoFilterChain.hh"

namespace efc {
namespace eio {

/**
 * A listener in charge of releasing the lock when the close has been completed
 */
class LockNotifyingListener : public EIoFutureListener, public ESynchronizeable {
public:
	virtual void operationComplete(EIoFuture* future) {
		SYNCHRONIZED(this) {
			this->notifyAll();
        }}
	}
};

EIoServiceListenerSupport::~EIoServiceListenerSupport()
{
	delete managedSessions;
	delete listeners;
}

EIoServiceListenerSupport::EIoServiceListenerSupport(EIoService* service)
{
	if (service == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "service");
	}

	this->service = service;

	listeners = new ECopyOnWriteArrayList<EIoServiceListener>();
	managedSessions = new EConcurrentHashMap<llong, EIoSession>();
	largestManagedSessionCount = 0;
}

void EIoServiceListenerSupport::add(sp<EIoServiceListener> listener) {
	if (listener != null) {
		listeners->add(listener);
	}
}

void EIoServiceListenerSupport::remove(EIoServiceListener* listener) {
	if (listener != null) {
		listeners->remove(listener);
	}
}

long EIoServiceListenerSupport::getActivationTime() {
	return activationTime;
}

EConcurrentMap<llong, EIoSession>* EIoServiceListenerSupport::getManagedSessions() {
	//@see: return readOnlyManagedSessions;
	return managedSessions;
}

int EIoServiceListenerSupport::getManagedSessionCount() {
	return managedSessions->size();
}

int EIoServiceListenerSupport::getLargestManagedSessionCount() {
	return largestManagedSessionCount;
}

long EIoServiceListenerSupport::getCumulativeManagedSessionCount() {
	return cumulativeManagedSessionCount.get();
}

boolean EIoServiceListenerSupport::isActive() {
	return activated.get();
}

void EIoServiceListenerSupport::fireServiceActivated() {
	if (!activated.compareAndSet(false, true)) {
		// The instance is already active
		return;
	}

	activationTime = ESystem::currentTimeMillis();

	// Activate all the listeners now
	/** @see:
	for (IoServiceListener listener : listeners) {
		try {
			listener.serviceActivated(service);
		} catch (Throwable e) {
			ExceptionMonitor.getInstance().exceptionCaught(e);
		}
	}
	*/
	sp<EConcurrentIterator<EIoServiceListener> > iter = listeners->iterator();
	while (iter->hasNext()) {
		sp<EIoServiceListener> listener = iter->next();
		try {
			listener->serviceActivated(service);
		} catch (EThrowable& e) {
			EExceptionMonitor::getInstance()->exceptionCaught(e);
		}
	}
}

void EIoServiceListenerSupport::fireServiceDeactivated() {
	if (!activated.compareAndSet(true, false)) {
		// The instance is already desactivated
		return;
	}

	// Desactivate all the listeners
	try {
		/** @see:
		for (IoServiceListener listener : listeners) {
			try {
				listener.serviceDeactivated(service);
			} catch (Throwable e) {
				ExceptionMonitor.getInstance().exceptionCaught(e);
			}
		}
		*/
		sp<EConcurrentIterator<EIoServiceListener> > iter = listeners->iterator();
		while (iter->hasNext()) {
			sp<EIoServiceListener> listener = iter->next();
			try {
				listener->serviceDeactivated(service);
			} catch (EThrowable& e) {
				EExceptionMonitor::getInstance()->exceptionCaught(e);
			}
		}
	} catch(...) {
		disconnectSessions();
		throw; //!
	} finally {
		disconnectSessions();
	}
}

void EIoServiceListenerSupport::fireSessionCreated(sp<EIoSession> session) {
	boolean firstSession = false;

	if (instanceof<EIoConnector>(session->getService())) {
		SYNCBLOCK(&managedSessionsLock) {
			firstSession = managedSessions->isEmpty();
        }}
	}

	// If already registered, ignore.
	if (managedSessions->putIfAbsent(session->getId(), session) != null) {
		return;
	}

	// If the first connector session, fire a virtual service activation event.
	if (firstSession) {
		fireServiceActivated();
	}

	// Fire session events.
	EIoFilterChain* filterChain = session->getFilterChain();
	filterChain->fireSessionCreated();

	// Notify the related future.
	sp<EConnectFuture> future = dynamic_pointer_cast<EConnectFuture>(
						session->removeAttribute(EDefaultIoFilterChain::SESSION_CREATED_FUTURE.get()));
	if (future != null) {
		future->setSession(session);
	}

	filterChain->fireSessionOpened();

	int managedSessionCount = managedSessions->size();

	if (managedSessionCount > largestManagedSessionCount) {
		largestManagedSessionCount = managedSessionCount;
	}

	cumulativeManagedSessionCount.incrementAndGet();

	// Fire listener events.
	/** @see:
	for (IoServiceListener l : listeners) {
		try {
			l.sessionCreated(session);
		} catch (Throwable e) {
			ExceptionMonitor.getInstance().exceptionCaught(e);
		}
	}
	*/
	sp<EConcurrentIterator<EIoServiceListener> > iter = listeners->iterator();
	while (iter->hasNext()) {
		sp<EIoServiceListener> listener = iter->next();
		try {
			listener->sessionCreated(session);
		} catch (EThrowable& e) {
			EExceptionMonitor::getInstance()->exceptionCaught(e);
		}
	}
}

void EIoServiceListenerSupport::fireSessionDestroyed(sp<EIoSession> session) {
	// Try to remove the remaining empty session set after removal.
	sp<EIoSession> s = managedSessions->remove(session->getId());
	if (s == null) {
		return;
	}

	// Fire session events.
	session->getFilterChain()->fireSessionClosed();

	// Fire listener events.
	try {
		/** @see:
		for (IoServiceListener l : listeners) {
			try {
				l.sessionDestroyed(session);
			} catch (Throwable e) {
				ExceptionMonitor.getInstance().exceptionCaught(e);
			}
		}
		*/
		sp<EConcurrentIterator<EIoServiceListener> > iter = listeners->iterator();
		while (iter->hasNext()) {
			sp<EIoServiceListener> listener = iter->next();
			try {
				listener->sessionDestroyed(session);
			} catch (EThrowable& e) {
				EExceptionMonitor::getInstance()->exceptionCaught(e);
			}
		}
	} catch(...) {
		finally {
			// Fire a virtual service deactivation event for the last session of the connector.
			if (instanceof<EIoConnector>(session->getService())) {
				boolean lastSession = false;

				SYNCBLOCK(&managedSessionsLock) {
					lastSession = managedSessions->isEmpty();
                }}

				if (lastSession) {
					fireServiceDeactivated();
				}
			}
		}

		throw; //!
	} finally {
		// Fire a virtual service deactivation event for the last session of the connector.
		if (instanceof<EIoConnector>(session->getService())) {
			boolean lastSession = false;

			SYNCBLOCK(&managedSessionsLock) {
				lastSession = managedSessions->isEmpty();
            }}

			if (lastSession) {
				fireServiceDeactivated();
			}
		}
	}
}

void EIoServiceListenerSupport::disconnectSessions() {
	if (!(instanceof<EIoAcceptor>(service))) {
		// We don't disconnect sessions for anything but an Acceptor
		return;
	}

	if (!(dynamic_cast<EIoAcceptor*>(service))->isCloseOnDeactivation()) {
		return;
	}

	/** @see:
	Object lock = new Object();
	IoFutureListener<IoFuture> listener = new LockNotifyingListener(lock);

	for (IoSession s : managedSessions.values()) {
		s.close(true).addListener(listener);
	}

	try {
		synchronized (lock) {
			while (!managedSessions.isEmpty()) {
				lock.wait(500);
			}
		}
	} catch (InterruptedException ie) {
		// Ignored
	}
	*/
	sp<LockNotifyingListener> listener = new LockNotifyingListener();

	sp<EConcurrentIterator<EIoSession> > iter = managedSessions->values()->iterator();
	while (iter->hasNext()) {
		sp<EIoSession> s = iter->next();
		s->closeNow()->addListener(listener);
	}
	try {
		SYNCHRONIZED (listener.get()) {
			while (!managedSessions->isEmpty()) {
				listener->wait(500);
			}
        }}
	} catch (EInterruptedException& ie) {
		// Ignored
	}
}

sp<EIoSession> EIoServiceListenerSupport::getSession(sp<EIoSession> session) {
	return managedSessions->get(session->getId());
}

} /* namespace eio */
} /* namespace efc */
