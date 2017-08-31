/*
 * EDefaultIoFuture.cpp
 *
 *  Created on: 2016-1-11
 *      Author: cxxjava@163.com
 */

#include "../inc/EDefaultIoFuture.hh"
#include "../inc/EExceptionMonitor.hh"

namespace efc {
namespace eio {

EDefaultIoFuture::~EDefaultIoFuture() {
	delete result;
}

sp<EIoFuture> EDefaultIoFuture::tosp() {
	return shared_from_this();
}

EDefaultIoFuture::EDefaultIoFuture(sp<EIoSession> is) :
		session(is), result(null), ready(
				false), waiters(0) {

}

sp<EIoSession> EDefaultIoFuture::getSession() {
	return session.lock();
}

void EDefaultIoFuture::await() {
	SYNCHRONIZED(this) {
		while (!ready) {
			waiters++;

			{
				ON_SCOPE_EXIT(
						waiters--;

						if (!ready) {
							checkDeadLock();
						}
				);

				// Wait for a notify, or if no notify is called,
				// assume that we have a deadlock and exit the
				// loop to check for a potential deadlock.
				this->wait(DEAD_LOCK_CHECK_INTERVAL);
			}
		}
    }}
}

boolean EDefaultIoFuture::await(llong timeout, ETimeUnit* unit) {
	return await0(unit->toMillis(timeout), true);
}

boolean EDefaultIoFuture::await(llong timeoutMillis) {
	return await0(timeoutMillis, true);
}

void EDefaultIoFuture::awaitUninterruptibly() {
	try {
		await0(ELLong::MAX_VALUE, false);
	} catch (EInterruptedException& ie) {
		// Do nothing : this catch is just mandatory by contract
	}
}

boolean EDefaultIoFuture::awaitUninterruptibly(llong timeout, ETimeUnit* unit) {
	try {
		return await0(unit->toMillis(timeout), false);
	} catch (EInterruptedException& e) {
		//throw new InternalError();
		throw ERuntimeException(__FILE__, __LINE__);
	}
}

boolean EDefaultIoFuture::awaitUninterruptibly(llong timeoutMillis) {
	try {
		return await0(timeoutMillis, false);
	} catch (EInterruptedException& e) {
		//throw new InternalError();
		throw ERuntimeException(__FILE__, __LINE__);
	}
}

boolean EDefaultIoFuture::isDone() {
	SYNCHRONIZED(this) {
		return ready;
    }}
}

boolean EDefaultIoFuture::setValue(EObject* newValue) {
	SYNCHRONIZED(this) {
		// Allowed only once.
		if (ready) {
			delete newValue; //!!!
			return false;
		}

		result = newValue;
		ready = true;

		// Now, if we have waiters, notofy them that the operation has completed
		if (waiters > 0) {
			this->notifyAll();
		}
    }}

	// Last, not least, inform the listeners
	notifyListeners();

	return true;
}

void EDefaultIoFuture::addListener(sp<EIoFutureListener> listener) {
	if (listener == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "listener");
	}

	SYNCHRONIZED(this) {
		if (ready) {
			// Shortcut : if the operation has completed, no need to
			// add a new listener, we just have to notify it. The existing
			// listeners have already been notified anyway, when the
			// 'ready' flag has been set.
			notifyListener(listener);
		} else {
			if (firstListener == null) {
				firstListener = listener;
			} else {
				otherListeners.add(listener);
			}
		}
    }}
}

void EDefaultIoFuture::removeListener(EIoFutureListener* listener) {
	if (listener == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "listener");
	}

	SYNCHRONIZED(this) {
		if (!ready) {
			if (firstListener == listener) {
				if (!otherListeners.isEmpty()) {
					firstListener = otherListeners.removeAt(0);
				} else {
					firstListener = null;
				}
			} else if (!otherListeners.isEmpty()) {
				otherListeners.remove(listener);
			}
		}
    }}
}

EObject* EDefaultIoFuture::getValue() {
	SYNCHRONIZED(this) {
		return result;
    }}
}

boolean EDefaultIoFuture::await0(llong timeoutMillis, boolean interruptable) {
	llong endTime = ESystem::currentTimeMillis() + timeoutMillis;

	if (endTime < 0) {
		endTime = ELLong::MAX_VALUE;
	}

	SYNCHRONIZED(this) {
		// We can quit if the ready flag is set to true, or if
		// the timeout is set to 0 or below : we don't wait in this case.
		if (ready||(timeoutMillis <= 0)) {
			return ready;
		}

		// The operation is not completed : we have to wait
		waiters++;

		{
			ON_SCOPE_EXIT(
				// We get here for 3 possible reasons :
				// 1) We have been notified (the operation has completed a way or another)
				// 2) We have reached the timeout
				// 3) The thread has been interrupted
				// In any case, we decrement the number of waiters, and we get out.
				waiters--;

				if (!ready) {
					checkDeadLock();
				}
			);

			for (;;) {
				try {
					llong timeOut = EMath::min(timeoutMillis, DEAD_LOCK_CHECK_INTERVAL);

					// Wait for the requested period of time,
					// but every DEAD_LOCK_CHECK_INTERVAL seconds, we will
					// check that we aren't blocked.
					this->wait(timeOut);
				} catch (EInterruptedException& e) {
					if (interruptable) {
						throw e;
					}
				}

				if (ready || (endTime < ESystem::currentTimeMillis())) {
					return ready;
				} else {
					// Take a chance, detect a potential deadlock
					checkDeadLock();
				}
			}
		}
    }}
}

void EDefaultIoFuture::checkDeadLock() {
//	// Only read / write / connect / write future can cause dead lock.
//	if (!(dynamic_cast<ECloseFuture*>(this)
//			|| dynamic_cast<EWriteFuture*>(this)
//			|| dynamic_cast<EReadFuture*>(this)
//			|| dynamic_cast<EConnectFuture*>(this)
//		)) {
//		return;
//	}

//TODO...

//	// Get the current thread stackTrace.
//	// Using Thread.currentThread().getStackTrace() is the best solution,
//	// even if slightly less efficient than doing a new Exception().getStackTrace(),
//	// as internally, it does exactly the same thing. The advantage of using
//	// this solution is that we may benefit some improvement with some
//	// future versions of Java.
//	StackTraceElement[] stackTrace = Thread.currentThread().getStackTrace();
//
//	// Simple and quick check.
//	for (StackTraceElement stackElement : stackTrace) {
//		if (AbstractPollingIoProcessor.class.getName().equals(stackElement.getClassName())) {
//			IllegalStateException e = new IllegalStateException("t");
//			e.getStackTrace();
//			throw new IllegalStateException("DEAD LOCK: " + IoFuture.class.getSimpleName()
//					+ ".await() was invoked from an I/O processor thread.  " + "Please use "
//					+ IoFutureListener.class.getSimpleName() + " or configure a proper thread model alternatively.");
//		}
//	}
//
//	// And then more precisely.
//	for (StackTraceElement s : stackTrace) {
//		try {
//			Class<?> cls = DefaultIoFuture.class.getClassLoader().loadClass(s.getClassName());
//			if (IoProcessor.class.isAssignableFrom(cls)) {
//				throw new IllegalStateException("DEAD LOCK: " + IoFuture.class.getSimpleName()
//						+ ".await() was invoked from an I/O processor thread.  " + "Please use "
//						+ IoFutureListener.class.getSimpleName()
//						+ " or configure a proper thread model alternatively.");
//			}
//		} catch (Exception cnfe) {
//			// Ignore
//		}
//	}
}

void EDefaultIoFuture::notifyListeners() {
	// There won't be any visibility problem or concurrent modification
	// because 'ready' flag will be checked against both addListener and
	// removeListener calls.
	if (firstListener != null) {
		notifyListener(firstListener);
		firstListener = null;

		if (!otherListeners.isEmpty()) {
			sp<EIterator<sp<EIoFutureListener> > > iter = otherListeners.iterator();
			while (iter->hasNext()) {
				sp<EIoFutureListener> listener = iter->next();
				notifyListener(listener);
			}

			otherListeners.clear();
		}
	}
}

void EDefaultIoFuture::notifyListener(sp<EIoFutureListener> listener) {
	try {
		listener->operationComplete(this);
	} catch (EException& e) {
		EExceptionMonitor::getInstance()->exceptionCaught(e);
	}
}

} /* namespace eio */
} /* namespace efc */
