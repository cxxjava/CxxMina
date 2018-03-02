/*
 * EExpiringMap.hh
 *
 *  Created on: 2016-8-29
 *      Author: cxxjava@163.com
 */

#ifndef EEXPIRINGMAP_HH_
#define EEXPIRINGMAP_HH_

#include "Efc.hh"
#include "./EExpirationListener.hh"

namespace efc {
namespace eio {

/**
 * A map with expiration.  This class contains a worker thread that will
 * periodically check this class in order to determine if any objects
 * should be removed based on the provided time-to-live value.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

/** The default value, 60 seconds */
#define DEFAULT_TIME_TO_LIVE  60

/** The default value, 1 second */
#define DEFAULT_EXPIRATION_INTERVAL 1

template<typename K, typename V>
class EExpiringMap: public EConcurrentMap<K,V> {
public:
	/**
	 * A Thread that monitors an {@link ExpiringMap} and will remove
	 * elements that have passed the threshold.
	 *
	 */
	class Expirer: public ERunnable {
	public:
		virtual ~Expirer() {
			expirerThread->join();
			sp<ERunnable> self = expirerThread->getTarget();
			self.dismiss(); //!
			delete expirerThread;
		}

		Expirer(EExpiringMap<K,V>* em): expiringMap(em), running(false) {
			expirerThread = new EThread(this, "ExpiringMapExpirer");
		}

		virtual void run() {
			while (running) {
				expiringMap->processExpires(timeToLiveMillis);

				try {
					EThread::sleep(expirationIntervalMillis);
				} catch (EInterruptedException& e) {
					// Do nothing
				}
			}
		}

		/**
		 * Kick off this thread which will look for old objects and remove them.
		 *
		 */
		void startExpiring() {
			SYNCBLOCK(stateLock.writeLock()) {
				if (!running) {
					running = true;
					expirerThread->start();
				}
            }}
		}

		/**
		 * If this thread has not started, then start it.
		 * Otherwise just return;
		 */
		void startExpiringIfNotStarted() {
			SYNCBLOCK(stateLock.readLock()) {
				if (running) {
					return;
				}
            }}

			SYNCBLOCK(stateLock.writeLock()) {
				if (!running) {
					running = true;
					expirerThread->start();
				}
            }}
		}

		/**
		 * Stop the thread from monitoring the map.
		 */
		void stopExpiring() {
			SYNCBLOCK(stateLock.writeLock()) {
				if (running) {
					running = false;
					expirerThread->interrupt();
				}
            }}
		}

		/**
		 * Checks to see if the thread is running
		 *
		 * @return
		 *  If the thread is running, true.  Otherwise false.
		 */
		boolean isRunning() {
			SYNCBLOCK(stateLock.readLock()) {
				return running;
            }}
		}

		/**
		 * @return the Time-to-live value in seconds.
		 */
		int getTimeToLive() {
			SYNCBLOCK(stateLock.readLock()) {
				return (int) timeToLiveMillis / 1000;
            }}
		}

		/**
		 * Update the value for the time-to-live
		 *
		 * @param timeToLive
		 *  The time-to-live (seconds)
		 */
		void setTimeToLive(llong timeToLive) {
			SYNCBLOCK(stateLock.writeLock()) {
				this->timeToLiveMillis = timeToLive * 1000;
            }}
		}

		/**
		 * Get the interval in which an object will live in the map before
		 * it is removed.
		 *
		 * @return
		 *  The time in seconds.
		 */
		int getExpirationInterval() {
			SYNCBLOCK(stateLock.readLock()) {
				return (int) expirationIntervalMillis / 1000;
            }}
		}

		/**
		 * Set the interval in which an object will live in the map before
		 * it is removed.
		 *
		 * @param expirationInterval
		 *  The time in seconds
		 */
		void setExpirationInterval(llong expirationInterval) {
			SYNCBLOCK(stateLock.writeLock()) {
				this->expirationIntervalMillis = expirationInterval * 1000;
            }}
		}

	private:
		EExpiringMap<K,V>* expiringMap;

		EReentrantReadWriteLock stateLock;
		llong timeToLiveMillis;
		llong expirationIntervalMillis;
		boolean running;// = false;
		EThread* expirerThread;
	};

public:
	virtual ~EExpiringMap() {
		//
	}

	/**
	 * Creates a new instance of ExpiringMap using the supplied values and
	 * a {@link ConcurrentHashMap} for the internal data structure.
	 *
	 * @param timeToLive The time-to-live value (seconds)
	 * @param expirationInterval The time between checks to see if a value should be removed (seconds)
	 */
	EExpiringMap(int timeToLive=DEFAULT_TIME_TO_LIVE, int expirationInterval=DEFAULT_EXPIRATION_INTERVAL): expirer(this) {
		expirer.setTimeToLive(timeToLive);
		expirer.setExpirationInterval(expirationInterval);
	}

	void addExpirationListener(sp<EExpirationListener<V> > listener) {
        expirationListeners.add(listener);
    }

    void removeExpirationListener(EExpirationListener<V>* listener) {
        expirationListeners.remove(listener);
    }

    Expirer* getExpirer() {
        return &expirer;
    }

    int getExpirationInterval() {
        return expirer.getExpirationInterval();
    }

    int getTimeToLive() {
        return expirer.getTimeToLive();
    }

    void setExpirationInterval(int expirationInterval) {
        expirer.setExpirationInterval(expirationInterval);
    }

    void setTimeToLive(int timeToLive) {
        expirer.setTimeToLive(timeToLive);
    }

public:
	virtual int size() {
		return delegate.size();
	}

	virtual boolean isEmpty() {
		return delegate.isEmpty();
	}

	virtual boolean containsKey(K* key) {
		return delegate.containsKey(key);
	}

	virtual boolean containsValue(V* value) {
		throw EUnsupportedOperationException(__FILE__, __LINE__);
	}

	virtual sp<V> get(K* key) {
		sp<ExpiringObject> object = delegate.get(key);

		if (object != null) {
			object->setLastAccessTime(ESystem::currentTimeMillis());

			return object->getValue();
		}

		return null;
	}

	virtual sp<V> put(sp<K> key, sp<V> value) {
		sp<ExpiringObject> answer = delegate.put(key, new ExpiringObject(key, value, ESystem::currentTimeMillis()));

		if (answer == null) {
			return null;
		}

		return answer->getValue();
	}

	virtual sp<V> remove(K* key) {
		sp<ExpiringObject> answer = delegate.remove(key);
		if (answer == null) {
			return null;
		}

		return answer->getValue();
	}

	virtual void clear() {
		delegate.clear();
	}

	virtual ESet<sp<K> >* keySet() {
		return delegate.keySet();
	}

	virtual ECollection<sp<V> >* values() {
		throw EUnsupportedOperationException(__FILE__, __LINE__);
	}

	virtual ESet<sp<EConcurrentMapEntry<K,V> > >* entrySet() {
		throw EUnsupportedOperationException(__FILE__, __LINE__);
	}

	virtual sp<V> putIfAbsent(sp<K> key, sp<V> value) {
		throw EUnsupportedOperationException(__FILE__, __LINE__);
	}

	virtual boolean remove(K* key, V* value) {
		throw EUnsupportedOperationException(__FILE__, __LINE__);
	}

	virtual boolean replace(K* key, V* oldValue, sp<V> newValue) {
		throw EUnsupportedOperationException(__FILE__, __LINE__);
	}

	virtual sp<V> replace(K* key, sp<V> value) {
		throw EUnsupportedOperationException(__FILE__, __LINE__);
	}

private:
	class ExpiringObject: public EObject {
	private:
		sp<K> key;
		sp<V> value;
		llong lastAccessTime;
		EReentrantReadWriteLock lastAccessTimeLock;

	public:
		virtual ~ExpiringObject() {
			//
		}

		ExpiringObject(sp<K>& key, sp<V>& value, llong lastAccessTime) {
			if (value == null) {
				throw EIllegalArgumentException(__FILE__, __LINE__, "An expiring object cannot be null.");
			}

			this->key = key;
			this->value = value;
			this->lastAccessTime = lastAccessTime;
		}

		llong getLastAccessTime() {
			SYNCBLOCK(lastAccessTimeLock.readLock()) {
				return lastAccessTime;
            }}
		}

		void setLastAccessTime(llong lastAccessTime) {
			SYNCBLOCK(lastAccessTimeLock.writeLock()) {
				this->lastAccessTime = lastAccessTime;
            }}
		}

		sp<K> getKey() {
			return key;
		}

		sp<V> getValue() {
			return value;
		}

		virtual boolean equals(EObject* obj) {
			return value->equals(obj);
		}

		virtual int hashCode() {
			return value->hashCode();
		}
	};

private:
	EConcurrentHashMap<K, ExpiringObject> delegate;

	ECopyOnWriteArrayList<EExpirationListener<V> > expirationListeners;

	friend class Expirer;
	Expirer expirer;

	void processExpires(llong timeToLiveMillis) {
		llong timeNow = ESystem::currentTimeMillis();

		sp<EIterator<sp<ExpiringObject> > > iter = delegate.values()->iterator();
		while (iter->hasNext()) {
			sp<ExpiringObject> o = iter->next();

			if (timeToLiveMillis <= 0) {
				continue;
			}

			llong timeIdle = timeNow - o->getLastAccessTime();

			if (timeIdle >= timeToLiveMillis) {
				delegate.remove(o->getKey().get());

				sp<EIterator<sp<EExpirationListener<V> > > > iter2 = expirationListeners.iterator();
				while (iter2->hasNext()) {
					sp<EExpirationListener<V> > listener = iter2->next();
					listener->expired(o->getValue());
				}
			}
		}
	}
};

} /* namespace eio */
} /* namespace efc */
#endif /* EEXPIRINGMAP_HH_ */
