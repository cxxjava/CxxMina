/*
 * EOrderedThreadPoolExecutor.hh
 *
 *  Created on: 2016-9-23
 *      Author: cxxjava@163.com
 */

#ifndef EORDEREDTHREADPOOLEXECUTOR_HH_
#define EORDEREDTHREADPOOLEXECUTOR_HH_

#include "./EIoEventQueueHandler.hh"
#include "./EAttributeKey.hh"
#include "Efc.hh"
#include "ELog.hh"

namespace efc {
namespace eio {

/**
 * A {@link ThreadPoolExecutor} that maintains the order of {@link IoEvent}s.
 * <p>
 * If you don't need to maintain the order of events per session, please use
 * {@link UnorderedThreadPoolExecutor}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @org.apache.xbean.XBean
 */

class EOrderedThreadPoolExecutor: public EThreadPoolExecutor {
public:
	DECLARE_STATIC_INITZZ;

public:
	virtual ~EOrderedThreadPoolExecutor() throw();

	/**
	 * Creates a default ThreadPool, with default values :
	 * - minimum pool size is 0
	 * - maximum pool size is 16
	 * - keepAlive set to 30 seconds
	 * - A default ThreadFactory
	 * - All events are accepted
	 */
	EOrderedThreadPoolExecutor();

	/**
	 * Creates a default ThreadPool, with default values :
	 * - minimum pool size is 0
	 * - keepAlive set to 30 seconds
	 * - A default ThreadFactory
	 * - All events are accepted
	 *
	 * @param maximumPoolSize The maximum pool size
	 */
	EOrderedThreadPoolExecutor(int maximumPoolSize);

	/**
	 * Creates a default ThreadPool, with default values :
	 * - keepAlive set to 30 seconds
	 * - A default ThreadFactory
	 * - All events are accepted
	 *
	 * @param corePoolSize The initial pool sizePoolSize
	 * @param maximumPoolSize The maximum pool size
	 */
	EOrderedThreadPoolExecutor(int corePoolSize, int maximumPoolSize);

	/**
	 * Creates a default ThreadPool, with default values :
	 * - A default ThreadFactory
	 * - All events are accepted
	 *
	 * @param corePoolSize The initial pool sizePoolSize
	 * @param maximumPoolSize The maximum pool size
	 * @param keepAliveTime Default duration for a thread
	 * @param unit Time unit used for the keepAlive value
	 */
	EOrderedThreadPoolExecutor(int corePoolSize, int maximumPoolSize, long keepAliveTime, ETimeUnit* unit);

	/**
	 * Creates a default ThreadPool, with default values :
	 * - A default ThreadFactory
	 *
	 * @param corePoolSize The initial pool sizePoolSize
	 * @param maximumPoolSize The maximum pool size
	 * @param keepAliveTime Default duration for a thread
	 * @param unit Time unit used for the keepAlive value
	 * @param eventQueueHandler The queue used to store events
	 */
	EOrderedThreadPoolExecutor(int corePoolSize, int maximumPoolSize, long keepAliveTime, ETimeUnit* unit,
			EIoEventQueueHandler* eventQueueHandler);

	/**
	 * Creates a default ThreadPool, with default values :
	 * - A default ThreadFactory
	 *
	 * @param corePoolSize The initial pool sizePoolSize
	 * @param maximumPoolSize The maximum pool size
	 * @param keepAliveTime Default duration for a thread
	 * @param unit Time unit used for the keepAlive value
	 * @param threadFactory The factory used to create threads
	 */
	EOrderedThreadPoolExecutor(int corePoolSize, int maximumPoolSize, long keepAliveTime, ETimeUnit* unit,
			sp<EThreadFactory> threadFactory);

	/**
	 * Creates a new instance of a OrderedThreadPoolExecutor.
	 *
	 * @param corePoolSize The initial pool sizePoolSize
	 * @param maximumPoolSize The maximum pool size
	 * @param keepAliveTime Default duration for a thread
	 * @param unit Time unit used for the keepAlive value
	 * @param threadFactory The factory used to create threads
	 * @param eventQueueHandler The queue used to store events
	 */
	EOrderedThreadPoolExecutor(int corePoolSize, int maximumPoolSize, long keepAliveTime, ETimeUnit* unit,
			sp<EThreadFactory> threadFactory, EIoEventQueueHandler* eventQueueHandler);

	/**
	 * @return The associated queue handler.
	 */
	EIoEventQueueHandler* getQueueHandler();

	/**
	 * {@inheritDoc}
	 */
	virtual void setRejectedExecutionHandler(sp<ERejectedExecutionHandler> handler);
	virtual int getMaximumPoolSize();
	virtual void setMaximumPoolSize(int maximumPoolSize);
	virtual boolean awaitTermination() THROWS(EInterruptedException);
	virtual boolean awaitTermination(llong timeout, ETimeUnit* unit) THROWS(EInterruptedException);
	virtual boolean isShutdown();
	virtual boolean isTerminating();
	virtual boolean isTerminated();
	virtual void shutdown();
	virtual EArrayList<sp<ERunnable> > shutdownNow();
	virtual void execute(sp<ERunnable> command);
	virtual int getActiveCount();
	virtual llong getCompletedTaskCount();
	virtual int getLargestPoolSize();
	virtual int getPoolSize();
	virtual llong getTaskCount();
	virtual int prestartAllCoreThreads();
	virtual boolean prestartCoreThread();
	virtual sp<EBlockingQueue<ERunnable> > getQueue();
	virtual void purge();
	virtual boolean remove(sp<ERunnable> task);
	virtual int getCorePoolSize();
	virtual void setCorePoolSize(int corePoolSize);

private:
	/** A logger for this class (commented as it breaks MDCFlter tests) */
	static sp<ELogger> LOGGER;// = LoggerFactory.getLogger(OrderedThreadPoolExecutor.class);

	/** A default value for the initial pool size */
	static const int DEFAULT_INITIAL_THREAD_POOL_SIZE = 0;

	/** A default value for the maximum pool size */
	static const int DEFAULT_MAX_THREAD_POOL = 16;

	/** A default value for the KeepAlive delay */
	static const int DEFAULT_KEEP_ALIVE = 30;

	static sp<EIoSession> EXIT_SIGNAL;// = new DummySession();

	/**
	 * A class used to store the ordered list of events to be processed by the
	 * session, and the current task state.
	 */
	class SessionTasksQueue: public EObject {
	public:
		SessionTasksQueue(): processingCompleted(true){}

		/**  A queue of ordered event waiting to be processed */
		EConcurrentLiteQueue<ERunnable> tasksQueue;// = new ConcurrentLinkedQueue<Runnable>();
		EReentrantLock tasksQueueLock;

		/** The current task state */
		boolean processingCompleted;// = true;
	};

private:
	class Worker: public ERunnable, public enable_shared_from_this<Worker> {
	public:
		EOrderedThreadPoolExecutor* executor;
		EThread* thread;
		EAtomicLLong completedTaskCount;// = new AtomicLong(0);

		Worker(EOrderedThreadPoolExecutor* otpl): executor(otpl) {
		}

		void init() {
			executor->aliveThreadCount++;
			sp<Worker> w = shared_from_this();
			this->thread = executor->getThreadFactory()->newThread(w);
			EThread::setDaemon(this->thread, true);
			this->thread->injectExitCallback(worker_thread_exit_callback, new sp<Worker>(w));
		}

		virtual void run() {
			executor->workerProcess(shared_from_this());
		}

		static void worker_thread_exit_callback(void* arg) {
			sp<Worker>* pw = (sp<Worker>*)arg;
			ES_ASSERT(pw);
			sp<Worker> w = *pw;

			SYNCBLOCK(&w->executor->mainLock) {
				w->executor->aliveThreadCount--;

				if (w->executor->isTerminated() && w->executor->aliveThreadCount == 0) {
					w->executor->termination->signalAll();
				}
			}}

			delete pw;
		}
	};

	/** A key stored into the session's attribute for the event tasks being queued */
	sp<EAttributeKey> TASKS_QUEUE;// = new AttributeKey(getClass(), "tasksQueue");

	/** A queue used to store the available sessions */
	ELinkedBlockingQueue<EIoSession> waitingSessions;// = new LinkedBlockingQueue<IoSession>();

	ESimpleLock mainLock;
	ECondition* termination; // = mainLock.newCondition();

	EArrayList<sp<Worker> > workers;// = new HashSet<Worker>();

	volatile int largestPoolSize;

	EAtomicInteger idleWorkers;// = new AtomicInteger();
	EAtomicCounter aliveThreadCount;

	llong completedTaskCount;

	volatile boolean shutdown_;

	EIoEventQueueHandler* eventQueueHandler;

	/**
	 * initialize for constructor.
	 */
	void init(int corePoolSize, int maximumPoolSize, EIoEventQueueHandler* eventQueueHandler);

	/**
	 * Add a new thread to execute a task, if needed and possible.
	 * It depends on the current pool size. If it's full, we do nothing.
	 */
	void addWorker();

	/**
	 * Add a new Worker only if there are no idle worker.
	 */
	void addWorkerIfNecessary();

	void removeWorker();

	inline void checkTaskType(ERunnable* task);

	/**
	 * Get the session's tasks queue.
	 */
	sp<SessionTasksQueue> getSessionTasksQueue(sp<EIoSession>& session);

	/**
	 * Worker run.
	 */
	void workerProcess(sp<Worker> worker);
	sp<EIoSession> fetchSession(sp<Worker>& worker);
	void runTasks(sp<Worker>& worker, sp<SessionTasksQueue> sessionTasksQueue);
	void runTask(sp<Worker>& worker, sp<ERunnable>& task);
};

} /* namespace eio */
} /* namespace efc */
#endif /* EORDEREDTHREADPOOLEXECUTOR_HH_ */
