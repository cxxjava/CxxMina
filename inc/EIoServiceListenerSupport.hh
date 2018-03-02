/*
 * EIoServiceListenerSupport.hh
 *
 *  Created on: 2014-9-15
 *      Author: cxxjava@163.com
 */

#ifndef EIOSERVICELISTENERSUPPORT_HH_
#define EIOSERVICELISTENERSUPPORT_HH_

#include "./EIoService.hh"

namespace efc {
namespace eio {

/**
 * A helper class which provides addition and removal of {@link IoServiceListener}s and firing
 * events.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */
class EIoServiceListenerSupport : public EObject {
public:
	virtual ~EIoServiceListenerSupport();

	/**
	 * Creates a new instance of the listenerSupport.
	 *
	 * @param service The associated IoService
	 */
	EIoServiceListenerSupport(EIoService* service);

	/**
	 * Adds a new listener.
	 *
	 * @param listener The added listener
	 */
	void add(sp<EIoServiceListener> listener);

	/**
	 * Removes an existing listener.
	 *
	 * @param listener The listener to remove
	 */
	void remove(EIoServiceListener* listener);

	/**
	 * @return The time (in ms) this instance has been activated
	 */
	long getActivationTime();

	/**
	 *
	 */
	EConcurrentMap<llong, EIoSession>* getManagedSessions();

	/**
	 *
	 */
	int getManagedSessionCount();

	/**
	 * @return The largest number of managed session since the creation of this
	 * listenerSupport
	 */
	int getLargestManagedSessionCount();

	/**
	 * @return The total number of sessions managed since the initilization of this
	 * ListenerSupport
	 */
	long getCumulativeManagedSessionCount();

	/**
	 * @return true if the instance is active
	 */
	boolean isActive();

	/**
	 * Calls {@link IoServiceListener#serviceActivated(IoService)}
	 * for all registered listeners.
	 */
	void fireServiceActivated();

	/**
	 * Calls {@link IoServiceListener#serviceDeactivated(IoService)}
	 * for all registered listeners.
	 */
	void fireServiceDeactivated();

	/**
	 * Calls {@link IoServiceListener#sessionCreated(IoSession)} for all registered listeners.
	 *
	 * @param session The session which has been created
	 */
	void fireSessionCreated(sp<EIoSession> session);

	/**
	 * Calls {@link IoServiceListener#sessionDestroyed(IoSession)} for all registered listeners.
	 *
	 * @param session The session which has been destroyed
	 */
	void fireSessionDestroyed(sp<EIoSession> session);

	/**
	 *
	 */
	sp<EIoSession> getSession(sp<EIoSession> session);

private:
	/** The {@link IoService} that this instance manages. */
	EIoService* service;

	/** A list of {@link IoServiceListener}s. */
	EList<sp<EIoServiceListener> >* listeners;// = new CopyOnWriteArrayList<IoServiceListener>();

	/** Tracks managed sessions. */
	EConcurrentMap<llong, EIoSession>* managedSessions;// = new ConcurrentHashMap<Long, IoSession>();
	EReentrantLock managedSessionsLock; //#added by cxxjava

	/**  Read only version of {@link #managedSessions}. */
	//Map<Long, IoSession> readOnlyManagedSessions = Collections.unmodifiableMap(managedSessions);

	EAtomicBoolean activated;// = new AtomicBoolean();

	/** Time this listenerSupport has been activated */
	volatile long activationTime;

	/** A counter used to store the maximum sessions we managed since the listenerSupport has been activated */
	volatile int largestManagedSessionCount;// = 0;

	/** A global counter to count the number of sessions managed since the start */
	EAtomicLLong cumulativeManagedSessionCount;// = 0;

	/**
	 * Close all the sessions
	 * TODO disconnectSessions.
	 *
	 */
	void disconnectSessions();
};

} /* namespace eio */
} /* namespace efc */
#endif /* EIOSERVICELISTENERSUPPORT_HH_ */
