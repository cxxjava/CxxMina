/*
 * EOrderedThreadPoolExecutor.cpp
 *
 *  Created on: 2016-9-23
 *      Author: cxxjava@163.com
 */

#include "EOrderedThreadPoolExecutor.hh"
#include "EDummySession.hh"

namespace efc {
namespace eio {

sp<ELogger> EOrderedThreadPoolExecutor::LOGGER = ELoggerManager::getLogger("EOrderedThreadPoolExecutor");
sp<EIoSession> EOrderedThreadPoolExecutor::EXIT_SIGNAL = new EDummySession();

EOrderedThreadPoolExecutor::~EOrderedThreadPoolExecutor() throw() {
	this->shutdown();
	this->awaitTermination();
	delete termination;
}

EOrderedThreadPoolExecutor::EOrderedThreadPoolExecutor() :
		EThreadPoolExecutor(DEFAULT_INITIAL_THREAD_POOL_SIZE,
				DEFAULT_MAX_THREAD_POOL, DEFAULT_KEEP_ALIVE, ETimeUnit::SECONDS,
				new ESynchronousQueue<ERunnable>(),
				EExecutors::defaultThreadFactory()),
				mainLock(ES_THREAD_MUTEX_NESTED),
				largestPoolSize(0),
				completedTaskCount(0),
				shutdown_(false) {
	this->init(DEFAULT_INITIAL_THREAD_POOL_SIZE, DEFAULT_MAX_THREAD_POOL, null);
}

EOrderedThreadPoolExecutor::EOrderedThreadPoolExecutor(int maximumPoolSize) :
		EThreadPoolExecutor(DEFAULT_INITIAL_THREAD_POOL_SIZE, maximumPoolSize,
				DEFAULT_KEEP_ALIVE, ETimeUnit::SECONDS,
				new ESynchronousQueue<ERunnable>(),
				EExecutors::defaultThreadFactory()),
				mainLock(ES_THREAD_MUTEX_NESTED),
				largestPoolSize(0),
				completedTaskCount(0),
				shutdown_(false) {
	this->init(DEFAULT_INITIAL_THREAD_POOL_SIZE, maximumPoolSize, null);
}

EOrderedThreadPoolExecutor::EOrderedThreadPoolExecutor(int corePoolSize,
		int maximumPoolSize) :
		EThreadPoolExecutor(corePoolSize, maximumPoolSize, DEFAULT_KEEP_ALIVE,
				ETimeUnit::SECONDS, new ESynchronousQueue<ERunnable>(),
				EExecutors::defaultThreadFactory()),
				mainLock(ES_THREAD_MUTEX_NESTED),
				largestPoolSize(0),
				completedTaskCount(0),
				shutdown_(false) {
	this->init(corePoolSize, maximumPoolSize, null);
}

EOrderedThreadPoolExecutor::EOrderedThreadPoolExecutor(int corePoolSize,
		int maximumPoolSize, long keepAliveTime, ETimeUnit* unit) :
		EThreadPoolExecutor(corePoolSize, maximumPoolSize, keepAliveTime, unit,
				new ESynchronousQueue<ERunnable>(),
				EExecutors::defaultThreadFactory()),
				mainLock(ES_THREAD_MUTEX_NESTED),
				largestPoolSize(0),
				completedTaskCount(0),
				shutdown_(false) {
	this->init(corePoolSize, maximumPoolSize, null);
}

EOrderedThreadPoolExecutor::EOrderedThreadPoolExecutor(int corePoolSize,
		int maximumPoolSize, long keepAliveTime, ETimeUnit* unit,
		EIoEventQueueHandler* eventQueueHandler) :
		EThreadPoolExecutor(corePoolSize, maximumPoolSize, keepAliveTime, unit,
				new ESynchronousQueue<ERunnable>(),
				EExecutors::defaultThreadFactory()),
				mainLock(ES_THREAD_MUTEX_NESTED),
				largestPoolSize(0),
				completedTaskCount(0),
				shutdown_(false) {
	this->init(corePoolSize, maximumPoolSize, eventQueueHandler);
}

EOrderedThreadPoolExecutor::EOrderedThreadPoolExecutor(int corePoolSize,
		int maximumPoolSize, long keepAliveTime, ETimeUnit* unit,
		sp<EThreadFactory> threadFactory) :
		EThreadPoolExecutor(corePoolSize, maximumPoolSize, keepAliveTime, unit,
				new ESynchronousQueue<ERunnable>(), threadFactory),
				mainLock(ES_THREAD_MUTEX_NESTED),
				largestPoolSize(0),
				completedTaskCount(0),
				shutdown_(false) {
	this->init(corePoolSize, maximumPoolSize, null);
}

EOrderedThreadPoolExecutor::EOrderedThreadPoolExecutor(int corePoolSize,
		int maximumPoolSize, long keepAliveTime, ETimeUnit* unit,
		sp<EThreadFactory> threadFactory,
		EIoEventQueueHandler* eventQueueHandler) :
		EThreadPoolExecutor(DEFAULT_INITIAL_THREAD_POOL_SIZE, 1, keepAliveTime,
				unit, new ESynchronousQueue<ERunnable>(), threadFactory),
				mainLock(ES_THREAD_MUTEX_NESTED),
				largestPoolSize(0),
				completedTaskCount(0),
				shutdown_(false) {
	this->init(corePoolSize, maximumPoolSize, eventQueueHandler);
}

void EOrderedThreadPoolExecutor::init(int corePoolSize, int maximumPoolSize,
		EIoEventQueueHandler* eventQueueHandler) {
	// We have to initialize the pool with default values (0 and 1) in order to
	// handle the exception in a better way. We can't add a try {} catch() {}
	// around the super() call.

	this->TASKS_QUEUE = new EAttributeKey(typeid(EOrderedThreadPoolExecutor).name(), "tasksQueue");
	this->termination = mainLock.newCondition();

	if (corePoolSize < DEFAULT_INITIAL_THREAD_POOL_SIZE) {
		corePoolSize = DEFAULT_INITIAL_THREAD_POOL_SIZE;
	}

	if (maximumPoolSize < corePoolSize) {
		maximumPoolSize = corePoolSize;
	}

	// Now, we can setup the pool sizes
	setCorePoolSize(corePoolSize);
	setMaximumPoolSize(maximumPoolSize);

	// The queueHandler might be null.
	if (eventQueueHandler == null) {
		this->eventQueueHandler = new DummyIoEventQueueHandler();
	} else {
		this->eventQueueHandler = eventQueueHandler;
	}
}

EIoEventQueueHandler* EOrderedThreadPoolExecutor::getQueueHandler() {
	return eventQueueHandler;
}

void EOrderedThreadPoolExecutor::setRejectedExecutionHandler(sp<ERejectedExecutionHandler> handler) {
	// Ignore the request.  It must always be AbortPolicy.
}

int EOrderedThreadPoolExecutor::getMaximumPoolSize() {
	return EThreadPoolExecutor::getMaximumPoolSize();
}

void EOrderedThreadPoolExecutor::setMaximumPoolSize(int maximumPoolSize) {
	if ((maximumPoolSize <= 0) || (maximumPoolSize < EThreadPoolExecutor::getCorePoolSize())) {
		throw EIllegalArgumentException(__FILE__, __LINE__, (EString() + "maximumPoolSize: " + maximumPoolSize).c_str());
	}

	SYNCBLOCK(&mainLock) {
		EThreadPoolExecutor::setMaximumPoolSize(maximumPoolSize);
		int difference = workers.size() - maximumPoolSize;
		while (difference > 0) {
			removeWorker();
			--difference;
		}
    }}
}

boolean EOrderedThreadPoolExecutor::awaitTermination() {
	return this->awaitTermination(EInteger::MAX_VALUE, ETimeUnit::SECONDS);
}

boolean EOrderedThreadPoolExecutor::awaitTermination(llong timeout, ETimeUnit* unit) {
	llong nanos = unit->toNanos(timeout);
	SYNCBLOCK(&mainLock) {
		while (!isTerminated()) {
			if (nanos <= 0)
				return false;
			nanos = termination->awaitNanos(nanos);
		}
    }}
	return isTerminated();

}

boolean EOrderedThreadPoolExecutor::isShutdown() {
	return shutdown_;
}

boolean EOrderedThreadPoolExecutor::isTerminating() {
	SYNCBLOCK(&mainLock) {
		return isShutdown() && !isTerminated();
    }}
}

boolean EOrderedThreadPoolExecutor::isTerminated() {
	if (!shutdown_) {
		return false;
	}

	SYNCBLOCK(&mainLock) {
		return workers.isEmpty();
    }}
}

void EOrderedThreadPoolExecutor::shutdown() {
	if (shutdown_) {
		return;
	}

	shutdown_ = true;

	SYNCBLOCK(&mainLock) {
		for (int i = workers.size(); i > 0; i--) {
			waitingSessions.offer(EXIT_SIGNAL);
		}
    }}
}

eal<ERunnable> EOrderedThreadPoolExecutor::shutdownNow() {
	shutdown();

	eal<ERunnable> answer;
	sp<EIoSession> session;

	while ((session = waitingSessions.poll()) != null) {
		if (session == EXIT_SIGNAL) {
			waitingSessions.offer(EXIT_SIGNAL);
			EThread::yield(); // Let others take the signal.
			continue;
		}

		sp<SessionTasksQueue> sessionTasksQueue = dynamic_pointer_cast<SessionTasksQueue>(session->getAttribute(TASKS_QUEUE.get()));

		SYNCBLOCK(&sessionTasksQueue->tasksQueueLock) {
			sp<EConcurrentIterator<ERunnable> > iter = sessionTasksQueue->tasksQueue.iterator();
			while (iter->hasNext()) {
				sp<ERunnable> task = iter->next();
				getQueueHandler()->polled(this, dynamic_cast<EIoEvent*>(task.get()));
				answer.add(task);
			}

			sessionTasksQueue->tasksQueue.clear();
        }}
	}

	return answer;
}

void EOrderedThreadPoolExecutor::execute(sp<ERunnable> task) {
	if (shutdown_) {
		//@see: rejectTask(task);
		getRejectedExecutionHandler()->rejectedExecution(task, this);
	}

	// Check that it's a IoEvent task
	checkTaskType(task.get());

	sp<EIoEvent> event = dynamic_pointer_cast<EIoEvent>(task);

	// Get the associated session
	sp<EIoSession> session = event->getSession();

	// Get the session's queue of events
	sp<SessionTasksQueue> sessionTasksQueue = getSessionTasksQueue(session);

	boolean offerSession;

	// propose the new event to the event queue handler. If we
	// use a throttle queue handler, the message may be rejected
	// if the maximum size has been reached.
	boolean offerEvent = eventQueueHandler->accept(this, event.get());

	if (offerEvent) {
		// Ok, the message has been accepted
		SYNCBLOCK(&sessionTasksQueue->tasksQueueLock) {
			// Inject the event into the executor taskQueue
			sessionTasksQueue->tasksQueue.offer(event);

			if (sessionTasksQueue->processingCompleted) {
				sessionTasksQueue->processingCompleted = false;
				offerSession = true;
			} else {
				offerSession = false;
			}

			EString sb("Adding event "); sb.append(GetIoEventTypeName(event->getType())).append(" to session ").append((llong)event->getSession()->getId());
			LOGGER->debug__(__FILE__, __LINE__, sb.c_str());
        }}
	} else {
		offerSession = false;
	}

	if (offerSession) {
		// As the tasksQueue was empty, the task has been executed
		// immediately, so we can move the session to the queue
		// of sessions waiting for completion.
		waitingSessions.offer(session);
	}

	addWorkerIfNecessary();

	if (offerEvent) {
		eventQueueHandler->offered(this, event.get());
	}
}

int EOrderedThreadPoolExecutor::getActiveCount() {
	SYNCBLOCK(&mainLock) {
		return workers.size() - idleWorkers.get();
    }}
}

llong EOrderedThreadPoolExecutor::getCompletedTaskCount() {
	SYNCBLOCK(&mainLock) {
		llong answer = completedTaskCount;
		for (int i = 0; i < workers.size(); i++) {
			answer += workers.get(i)->completedTaskCount.get();
		}
		return answer;
    }}
}

int EOrderedThreadPoolExecutor::getLargestPoolSize() {
	return largestPoolSize;
}

int EOrderedThreadPoolExecutor::getPoolSize() {
	SYNCBLOCK(&mainLock) {
		return workers.size();
    }}
}

llong EOrderedThreadPoolExecutor::getTaskCount() {
	return getCompletedTaskCount();
}

int EOrderedThreadPoolExecutor::prestartAllCoreThreads() {
	int answer = 0;
	SYNCBLOCK(&mainLock) {
		for (int i = EThreadPoolExecutor::getCorePoolSize() - workers.size(); i > 0; i--) {
			addWorker();
			answer++;
		}
    }}
	return answer;
}

boolean EOrderedThreadPoolExecutor::prestartCoreThread() {
	SYNCBLOCK(&mainLock) {
		if (workers.size() < EThreadPoolExecutor::getCorePoolSize()) {
			addWorker();
			return true;
		} else {
			return false;
		}
    }}
}

sp<EBlockingQueue<ERunnable> > EOrderedThreadPoolExecutor::getQueue() {
	throw EUnsupportedOperationException(__FILE__, __LINE__);
}

void EOrderedThreadPoolExecutor::purge() {
	// Nothing to purge in this implementation.
}

boolean EOrderedThreadPoolExecutor::remove(sp<ERunnable> task) {
	checkTaskType(task.get());
	sp<EIoEvent> event = dynamic_pointer_cast<EIoEvent>(task);
	sp<EIoSession> session = event->getSession();
	sp<SessionTasksQueue> sessionTasksQueue = dynamic_pointer_cast<SessionTasksQueue>(session->getAttribute(TASKS_QUEUE.get()));

	if (sessionTasksQueue == null) {
		return false;
	}

	boolean removed;

	SYNCBLOCK(&sessionTasksQueue->tasksQueueLock) {
		removed = sessionTasksQueue->tasksQueue.remove(task.get());
    }}

	if (removed) {
		getQueueHandler()->polled(this, event.get());
	}

	return removed;
}

int EOrderedThreadPoolExecutor::getCorePoolSize() {
	return EThreadPoolExecutor::getCorePoolSize();
}

void EOrderedThreadPoolExecutor::setCorePoolSize(int corePoolSize) {
	if (corePoolSize < 0) {
		throw EIllegalArgumentException(__FILE__, __LINE__, (EString() + "corePoolSize: " + corePoolSize).c_str());
	}
	if (corePoolSize > EThreadPoolExecutor::getMaximumPoolSize()) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "corePoolSize exceeds maximumPoolSize");
	}

	SYNCBLOCK(&mainLock) {
		if (EThreadPoolExecutor::getCorePoolSize() > corePoolSize) {
			for (int i = EThreadPoolExecutor::getCorePoolSize() - corePoolSize; i > 0; i--) {
				removeWorker();
			}
		}
		EThreadPoolExecutor::setCorePoolSize(corePoolSize);
    }}
}

void EOrderedThreadPoolExecutor::addWorker() {
//	SYNCBLOCK(&mainLock) {
		if (workers.size() >= EThreadPoolExecutor::getMaximumPoolSize()) {
			return;
		}

		// Create a new worker, and add it to the thread pool
		sp<Worker> worker = new Worker(this);
		worker->init(); //!!!
		EThread* thread = worker->thread;

		// As we have added a new thread, it's considered as idle.
		idleWorkers.incrementAndGet();

		// Now, we can start it.
		thread->start();
		workers.add(worker);

		if (workers.size() > largestPoolSize) {
			largestPoolSize = workers.size();
		}
//    }}
}

void EOrderedThreadPoolExecutor::addWorkerIfNecessary() {
	if (idleWorkers.get() == 0) {
		SYNCBLOCK(&mainLock) {
			if (workers.isEmpty() || (idleWorkers.get() == 0)) {
				addWorker();
			}
        }}
	}
}

void EOrderedThreadPoolExecutor::removeWorker() {
	SYNCBLOCK(&mainLock) {
		if (workers.size() <= EThreadPoolExecutor::getCorePoolSize()) {
			return;
		}
		waitingSessions.offer(EXIT_SIGNAL);
    }}
}

void EOrderedThreadPoolExecutor::checkTaskType(ERunnable* task) {
	if (!instanceof<EIoEvent>(task)) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "task must be an IoEvent or its subclass.");
	}
}

sp<EOrderedThreadPoolExecutor::SessionTasksQueue> EOrderedThreadPoolExecutor::getSessionTasksQueue(sp<EIoSession>& session) {
	sp<SessionTasksQueue> queue = dynamic_pointer_cast<SessionTasksQueue>(session->getAttribute(TASKS_QUEUE.get()));

	if (queue == null) {
		queue = new SessionTasksQueue();
		sp<SessionTasksQueue> oldQueue = dynamic_pointer_cast<SessionTasksQueue>(session->setAttributeIfAbsent(TASKS_QUEUE, queue));

		if (oldQueue != null) {
			queue = oldQueue;
		}
	}

	return queue;
}

void EOrderedThreadPoolExecutor::workerProcess(sp<Worker> worker) {
	ON_FINALLY_NOTHROW(
		SYNCBLOCK(&mainLock) {
			workers.remove(worker.get());
			this->completedTaskCount += worker->completedTaskCount.get();
//			workers.notifyAll();
        }}
	) {
		for (;;) {
			sp<EIoSession> session = fetchSession(worker);

			idleWorkers.decrementAndGet();

			if (session == null) {
				SYNCBLOCK(&mainLock) {
					if (workers.size() > getCorePoolSize()) {
						// Remove now to prevent duplicate exit.
						workers.remove(worker.get());
						break;
					}
                }}
			}

			if (session == EXIT_SIGNAL) {
				break;
			}

			ON_FINALLY_NOTHROW(
				idleWorkers.incrementAndGet();
			) {
				if (session != null) {
					runTasks(worker, getSessionTasksQueue(session));
				}
            }}
		}
    }}
}

sp<EIoSession> EOrderedThreadPoolExecutor::fetchSession(sp<Worker>& worker) {
	sp<EIoSession> session;// = null;
	llong currentTime = ESystem::currentTimeMillis();
	llong deadline = currentTime + getKeepAliveTime(ETimeUnit::MILLISECONDS);
	for (;;) {
		try {
			llong waitTime = deadline - currentTime;
			if (waitTime <= 0) {
				break;
			}

			ON_SCOPE_EXIT(
				if (session == null) {
					currentTime = ESystem::currentTimeMillis();
				}
			);
			{
				session = waitingSessions.poll(waitTime, ETimeUnit::MILLISECONDS);
				break;
			}
		} catch (EInterruptedException& e) {
			// Ignore.
			continue;
		}
	}
	return session;
}

void EOrderedThreadPoolExecutor::runTasks(sp<Worker>& worker, sp<SessionTasksQueue> sessionTasksQueue) {
	for (;;) {
		sp<ERunnable> task;

		SYNCBLOCK(&sessionTasksQueue->tasksQueueLock) {
			task = sessionTasksQueue->tasksQueue.poll();

			if (task == null) {
				sessionTasksQueue->processingCompleted = true;
				break;
			}
        }}

		eventQueueHandler->polled(this, dynamic_cast<EIoEvent*>(task.get()));

		runTask(worker, task);
	}
}

void EOrderedThreadPoolExecutor::runTask(sp<Worker>& worker, sp<ERunnable>& task) {
	beforeExecute(EThread::currentThread(), task);
	boolean ran = false;
	try {
		task->run();
		ran = true;
		afterExecute(task, null);
		worker->completedTaskCount.incrementAndGet();
	} catch (ERuntimeException& e) {
		if (!ran) {
			afterExecute(task, &e);
		}
		throw e;
	}
}

} /* namespace eio */
} /* namespace efc */
