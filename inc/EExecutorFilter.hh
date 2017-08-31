/*
 * EExecutorFilter.hh
 *
 *  Created on: 2016-9-18
 *      Author: cxxjava@163.com
 */

#ifndef EEXECUTORFILTER_HH_
#define EEXECUTORFILTER_HH_

#include "./EIoFilterEvent.hh"
#include "./EIoFilterAdapter.hh"
#include "./EIoEventQueueHandler.hh"

namespace efc {
namespace eio {

/**
 * A filter that forwards I/O events to {@link Executor} to enforce a certain
 * thread model while allowing the events per session to be processed
 * simultaneously. You can apply various thread model by inserting this filter
 * to a {@link IoFilterChain}.
 *
 * <h2>Life Cycle Management</h2>
 *
 * Please note that this filter doesn't manage the life cycle of the {@link Executor}.
 * If you created this filter using {@link #ExecutorFilter(Executor)} or similar
 * constructor that accepts an {@link Executor} that you've instantiated, you have
 * full control and responsibility of managing its life cycle (e.g. calling
 * {@link ExecutorService#shutdown()}.
 * <p>
 * If you created this filter using convenience constructors like
 * {@link #ExecutorFilter(int)}, then you can shut down the executor by calling
 * {@link #destroy()} explicitly.
 *
 * <h2>Event Ordering</h2>
 *
 * All convenience constructors of this filter creates a new
 * {@link OrderedThreadPoolExecutor} instance.  Therefore, the order of event is
 * maintained like the following:
 * <ul>
 * <li>All event handler methods are called exclusively.
 *     (e.g. messageReceived and messageSent can't be invoked at the same time.)</li>
 * <li>The event order is never mixed up.
 *     (e.g. messageReceived is always invoked before sessionClosed or messageSent.)</li>
 * </ul>
 * However, if you specified other {@link Executor} instance in the constructor,
 * the order of events are not maintained at all.  This means more than one event
 * handler methods can be invoked at the same time with mixed order.  For example,
 * let's assume that messageReceived, messageSent, and sessionClosed events are
 * fired.
 * <ul>
 * <li>All event handler methods can be called simultaneously.
 *     (e.g. messageReceived and messageSent can be invoked at the same time.)</li>
 * <li>The event order can be mixed up.
 *     (e.g. sessionClosed or messageSent can be invoked before messageReceived
 *           is invoked.)</li>
 * </ul>
 * If you need to maintain the order of events per session, please specify an
 * {@link OrderedThreadPoolExecutor} instance or use the convenience constructors.
 *
 * <h2>Selective Filtering</h2>
 *
 * By default, all event types but <tt>sessionCreated</tt>, <tt>filterWrite</tt>,
 * <tt>filterClose</tt> and <tt>filterSetTrafficMask</tt> are submitted to the
 * underlying executor, which is most common setting.
 * <p>
 * If you want to submit only a certain set of event types, you can specify them
 * in the constructor.  For example, you could configure a thread pool for
 * write operation for the maximum performance:
 * <pre><code>
 * IoService service = ...;
 * DefaultIoFilterChainBuilder chain = service.getFilterChain();
 *
 * chain.addLast("codec", new ProtocolCodecFilter(...));
 * // Use one thread pool for most events.
 * chain.addLast("executor1", new ExecutorFilter());
 * // and another dedicated thread pool for 'filterWrite' events.
 * chain.addLast("executor2", new ExecutorFilter(IoEventType.WRITE));
 * </code></pre>
 *
 * <h2>Preventing {@link OutOfMemoryError}</h2>
 *
 * Please refer to {@link IoEventQueueThrottle}, which is specified as
 * a parameter of the convenience constructors.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 *
 * @see OrderedThreadPoolExecutor
 * @see UnorderedThreadPoolExecutor
 * @org.apache.xbean.XBean
 */

class EExecutorFilter: public EIoFilterAdapter {
public:
	virtual ~EExecutorFilter() throw();

	/**
	 * (Convenience constructor) Creates a new instance with a new
	 * {@link OrderedThreadPoolExecutor}, no thread in the pool, and a
	 * maximum of 16 threads in the pool. All the event will be handled
	 * by this default executor.
	 */
	EExecutorFilter();

	/**
	 * (Convenience constructor) Creates a new instance with a new
	 * {@link OrderedThreadPoolExecutor}, no thread in the pool, but
	 * a maximum of threads in the pool is given. All the event will be handled
	 * by this default executor.
	 *
	 * @param maximumPoolSize The maximum pool size
	 */
	EExecutorFilter(int maximumPoolSize);

	/**
	 * (Convenience constructor) Creates a new instance with a new
	 * {@link OrderedThreadPoolExecutor}, a number of thread to start with, a
	 * maximum of threads the pool can contain. All the event will be handled
	 * by this default executor.
	 *
	 * @param corePoolSize The initial pool size
	 * @param maximumPoolSize The maximum pool size
	 */
	EExecutorFilter(int corePoolSize, int maximumPoolSize);

	/**
	 * (Convenience constructor) Creates a new instance with a new
	 * {@link OrderedThreadPoolExecutor}.
	 *
	 * @param corePoolSize The initial pool size
	 * @param maximumPoolSize The maximum pool size
	 * @param keepAliveTime Default duration for a thread
	 * @param unit Time unit used for the keepAlive value
	 */
	EExecutorFilter(int corePoolSize, int maximumPoolSize, long keepAliveTime, ETimeUnit* unit);

	/**
	 * (Convenience constructor) Creates a new instance with a new
	 * {@link OrderedThreadPoolExecutor}.
	 *
	 * @param corePoolSize The initial pool size
	 * @param maximumPoolSize The maximum pool size
	 * @param keepAliveTime Default duration for a thread
	 * @param unit Time unit used for the keepAlive value
	 * @param queueHandler The queue used to store events
	 */
	EExecutorFilter(int corePoolSize, int maximumPoolSize, long keepAliveTime, ETimeUnit* unit,
			EIoEventQueueHandler* queueHandler);

	/**
	 * (Convenience constructor) Creates a new instance with a new
	 * {@link OrderedThreadPoolExecutor}.
	 *
	 * @param corePoolSize The initial pool size
	 * @param maximumPoolSize The maximum pool size
	 * @param keepAliveTime Default duration for a thread
	 * @param unit Time unit used for the keepAlive value
	 * @param threadFactory The factory used to create threads
	 */
	EExecutorFilter(int corePoolSize, int maximumPoolSize, long keepAliveTime, ETimeUnit* unit,
			sp<EThreadFactory> threadFactory);

	/**
	 * (Convenience constructor) Creates a new instance with a new
	 * {@link OrderedThreadPoolExecutor}.
	 *
	 * @param corePoolSize The initial pool size
	 * @param maximumPoolSize The maximum pool size
	 * @param keepAliveTime Default duration for a thread
	 * @param unit Time unit used for the keepAlive value
	 * @param threadFactory The factory used to create threads
	 * @param queueHandler The queue used to store events
	 */
	EExecutorFilter(int corePoolSize, int maximumPoolSize, long keepAliveTime, ETimeUnit* unit,
			sp<EThreadFactory> threadFactory, EIoEventQueueHandler* queueHandler);

	/**
	 * (Convenience constructor) Creates a new instance with a new
	 * {@link OrderedThreadPoolExecutor}.
	 *
	 * @param eventTypes The event for which the executor will be used
	 */
	EExecutorFilter(long IoEventTypeSet);

	/**
	 * (Convenience constructor) Creates a new instance with a new
	 * {@link OrderedThreadPoolExecutor}.
	 *
	 * @param maximumPoolSize The maximum pool size
	 * @param eventTypes The event for which the executor will be used
	 */
	EExecutorFilter(int maximumPoolSize, long IoEventTypeSet);

	/**
	 * (Convenience constructor) Creates a new instance with a new
	 * {@link OrderedThreadPoolExecutor}.
	 *
	 * @param corePoolSize The initial pool size
	 * @param maximumPoolSize The maximum pool size
	 * @param eventTypes The event for which the executor will be used
	 */
	EExecutorFilter(int corePoolSize, int maximumPoolSize, long IoEventTypeSet);

	/**
	 * (Convenience constructor) Creates a new instance with a new
	 * {@link OrderedThreadPoolExecutor}.
	 *
	 * @param corePoolSize The initial pool size
	 * @param maximumPoolSize The maximum pool size
	 * @param keepAliveTime Default duration for a thread
	 * @param unit Time unit used for the keepAlive value
	 * @param eventTypes The event for which the executor will be used
	 */
	EExecutorFilter(int corePoolSize, int maximumPoolSize, long keepAliveTime, ETimeUnit* unit,
			long IoEventTypeSet);

	/**
	 * (Convenience constructor) Creates a new instance with a new
	 * {@link OrderedThreadPoolExecutor}.
	 *
	 * @param corePoolSize The initial pool size
	 * @param maximumPoolSize The maximum pool size
	 * @param keepAliveTime Default duration for a thread
	 * @param unit Time unit used for the keepAlive value
	 * @param queueHandler The queue used to store events
	 * @param eventTypes The event for which the executor will be used
	 */
	EExecutorFilter(int corePoolSize, int maximumPoolSize, long keepAliveTime, ETimeUnit* unit,
			EIoEventQueueHandler* queueHandler, long IoEventTypeSet);

	/**
	 * (Convenience constructor) Creates a new instance with a new
	 * {@link OrderedThreadPoolExecutor}.
	 *
	 * @param corePoolSize The initial pool size
	 * @param maximumPoolSize The maximum pool size
	 * @param keepAliveTime Default duration for a thread
	 * @param unit Time unit used for the keepAlive value
	 * @param threadFactory The factory used to create threads
	 * @param eventTypes The event for which the executor will be used
	 */
	EExecutorFilter(int corePoolSize, int maximumPoolSize, long keepAliveTime, ETimeUnit* unit,
			sp<EThreadFactory> threadFactory, long IoEventTypeSet);

	/**
	 * (Convenience constructor) Creates a new instance with a new
	 * {@link OrderedThreadPoolExecutor}.
	 *
	 * @param corePoolSize The initial pool size
	 * @param maximumPoolSize The maximum pool size
	 * @param keepAliveTime Default duration for a thread
	 * @param unit Time unit used for the keepAlive value
	 * @param threadFactory The factory used to create threads
	 * @param queueHandler The queue used to store events
	 * @param eventTypes The event for which the executor will be used
	 */
	EExecutorFilter(int corePoolSize, int maximumPoolSize, long keepAliveTime, ETimeUnit* unit,
			sp<EThreadFactory> threadFactory, EIoEventQueueHandler* queueHandler, long IoEventTypeSet);

	/**
	 * Creates a new instance with the specified {@link Executor}.
	 *
	 * @param executor the user's managed Executor to use in this filter
	 */
	EExecutorFilter(EExecutorService* executor);

	/**
	 * Creates a new instance with the specified {@link Executor}.
	 *
	 * @param executor the user's managed Executor to use in this filter
	 * @param eventTypes The event for which the executor will be used
	 */
	EExecutorFilter(EExecutorService* executor, long IoEventTypeSet);

	/**
	 * @return the underlying {@link Executor} instance this filter uses.
	 */
	EExecutor* getExecutor();

	/**
	 * Shuts down the underlying executor if this filter hase been created via
	 * a convenience constructor.
	 */
	virtual void destroy() THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void onPreAdd(EIoFilterChain* parent, const char* name, EIoFilter::NextFilter* nextFilter) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void sessionOpened(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void sessionClosed(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void sessionIdle(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, EIdleStatus status) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void exceptionCaught(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, sp<EThrowableType>& cause) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void messageReceived(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, sp<EObject>& message) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void messageSent(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void filterClose(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void filterWrite(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) THROWS(EException);

protected:
	/**
	 * Fires the specified event through the underlying executor.
	 *
	 * @param event The filtered event
	 */
	virtual void fireEvent(sp<EIoFilterEvent> event);

private:
	/** The list of handled events */
	long eventTypeSet;

	/** The associated executor */
	EExecutorService* executor;

	/** A flag set if the executor can be managed */
	boolean manageableExecutor;

	/** The default pool size */
	static const int DEFAULT_MAX_POOL_SIZE = 16;

	/** The number of thread to create at startup */
	static const int BASE_THREAD_NUMBER = 0;

	/** The default KeepAlive time, in seconds */
	static const long DEFAULT_KEEPALIVE_TIME = 30;

	/**
	 * A set of flags used to tell if the Executor has been created
	 * in the constructor or passed as an argument. In the second case,
	 * the executor state can be managed.
	 **/
	static const boolean MANAGEABLE_EXECUTOR = true;

	static const boolean NOT_MANAGEABLE_EXECUTOR = false;

	/** A list of default EventTypes to be handled by the executor */
	static const long DEFAULT_EVENT_SET = EIoEventType::EXCEPTION_CAUGHT |
			EIoEventType::MESSAGE_RECEIVED | EIoEventType::MESSAGE_SENT | EIoEventType::SESSION_CLOSED |
			EIoEventType::SESSION_IDLE | EIoEventType::SESSION_OPENED;

	/**
	 * Creates a new instance of ExecutorFilter. This private constructor is called by all
	 * the public constructor.
	 *
	 * @param executor The underlying {@link Executor} in charge of managing the Thread pool.
	 * @param manageableExecutor Tells if the Executor's Life Cycle can be managed or not
	 * @param eventTypes The lit of event which are handled by the executor
	 */
	inline void init(EExecutorService* executor, boolean manageableExecutor, long IoEventTypeSet);

	/**
	 * Create an OrderedThreadPool executor.
	 *
	 * @param corePoolSize The initial pool sizePoolSize
	 * @param maximumPoolSize The maximum pool size
	 * @param keepAliveTime Default duration for a thread
	 * @param unit Time unit used for the keepAlive value
	 * @param threadFactory The factory used to create threads
	 * @param queueHandler The queue used to store events
	 * @return An instance of the created Executor
	 */
	inline EExecutorService* createDefaultExecutor(int corePoolSize, int maximumPoolSize, long keepAliveTime, ETimeUnit* unit,
			sp<EThreadFactory> threadFactory, EIoEventQueueHandler* queueHandler);
};

} /* namespace eio */
} /* namespace efc */
#endif /* EEXECUTORFILTER_HH_ */
