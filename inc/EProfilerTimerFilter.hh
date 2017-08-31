/*
 * EProfilerTimerFilter.hh
 *
 *  Created on: 2016-5-17
 *      Author: cxxjava@163.com
 */

#ifndef EPROFILERTIMERFILTER_HH_
#define EPROFILERTIMERFILTER_HH_

#include "./EIoFilterAdapter.hh"
#include "./EIoEventType.hh"

namespace efc {
namespace eio {

/**
 * This class will measure the time it takes for a
 * method in the {@link IoFilterAdapter} class to execute.  The basic
 * premise of the logic in this class is to get the current time
 * at the beginning of the method, call method on nextFilter, and
 * then get the current time again.  An example of how to use
 * the filter is:
 *
 * <pre>
 * ProfilerTimerFilter profiler = new ProfilerTimerFilter(
 *         TimeUnit.MILLISECOND, IoEventType.MESSAGE_RECEIVED);
 * chain.addFirst("Profiler", profiler);
 * </pre>
 *
 * The profiled {@link IoEventType} are :
 * <ul>
 * <li>IoEventType.MESSAGE_RECEIVED</li>
 * <li>IoEventType.MESSAGE_SENT</li>
 * <li>IoEventType.SESSION_CREATED</li>
 * <li>IoEventType.SESSION_OPENED</li>
 * <li>IoEventType.SESSION_IDLE</li>
 * <li>IoEventType.SESSION_CLOSED</li>
 * </ul>
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @org.apache.xbean.XBean
 */

class EProfilerTimerFilter: public EIoFilterAdapter {
public:
	virtual ~EProfilerTimerFilter();

	/**
	 * Creates a new instance of ProfilerFilter.  This is the
	 * default constructor and will print out timings for
	 * messageReceived and messageSent and the time increment
	 * will be in milliseconds.
	 */
	EProfilerTimerFilter();

	/**
	 * Creates a new instance of ProfilerFilter.  This is the
	 * default constructor and will print out timings for
	 * messageReceived and messageSent.
	 *
	 * @param timeUnit the time increment to set
	 */
	EProfilerTimerFilter(ETimeUnit* timeUnit);

	/**
	 * Creates a new instance of ProfilerFilter.  An example
	 * of this call would be:
	 *
	 * <pre>
	 * new ProfilerTimerFilter(
	 *         TimeUnit.MILLISECONDS,
	 *         IoEventType.MESSAGE_RECEIVED | IoEventType.MESSAGE_SENT);
	 * </pre>
	 *
	 * Note : you can add as many {@link IoEventType} as you want. The method accepts
	 * a variable number of arguments.
	 *
	 * @param timeUnit Used to determine the level of precision you need in your timing.
	 * @param eventTypes A list of {@link IoEventType} representation of the methods to profile
	 */
	EProfilerTimerFilter(ETimeUnit* timeUnit, int eventTypes);

	/**
	 * Sets the {@link TimeUnit} being used.
	 *
	 * @param timeUnit the new {@link TimeUnit} to be used.
	 */
	void setTimeUnit(ETimeUnit* timeUnit);

	/**
	 * Set the {@link IoEventType} to be profiled
	 *
	 * @param type The {@link IoEventType} to profile
	 */
	void profile(EIoEventType type);

	/**
	 * Stop profiling an {@link IoEventType}
	 *
	 * @param type The {@link IoEventType} to stop profiling
	 */
	void stopProfile(EIoEventType type);

	/**
	 * Return the set of {@link IoEventType} which are profiled.
	 *
	 * @return a Set containing all the profiled {@link IoEventType}
	 */
	int getEventsToProfile();

	/**
	 * Set the profilers for a list of {@link IoEventType}
	 *
	 * @param eventTypes the list of {@link IoEventType} to profile
	 */
	void setEventsToProfile(int eventTypes);

	/**
	 * Get the average time for the specified method represented by the {@link IoEventType}
	 *
	 * @param type
	 *  The {@link IoEventType} that the user wants to get the average method call time
	 * @return
	 *  The average time it took to execute the method represented by the {@link IoEventType}
	 */
	double getAverageTime(EIoEventType type);

	/**
	 * Gets the total number of times the method has been called that is represented by the
	 * {@link IoEventType}
	 *
	 * @param type
	 *  The {@link IoEventType} that the user wants to get the total number of method calls
	 * @return
	 *  The total number of method calls for the method represented by the {@link IoEventType}
	 */
	llong getTotalCalls(EIoEventType type);

	/**
	 * The total time this method has been executing
	 *
	 * @param type
	 *  The {@link IoEventType} that the user wants to get the total time this method has
	 *  been executing
	 * @return
	 *  The total time for the method represented by the {@link IoEventType}
	 */
	llong getTotalTime(EIoEventType type);

	/**
	 * The minimum time the method represented by {@link IoEventType} has executed
	 *
	 * @param type
	 *  The {@link IoEventType} that the user wants to get the minimum time this method has
	 *  executed
	 * @return
	 *  The minimum time this method has executed represented by the {@link IoEventType}
	 */
	llong getMinimumTime(EIoEventType type);

	/**
	 * The maximum time the method represented by {@link IoEventType} has executed
	 *
	 * @param type
	 *  The {@link IoEventType} that the user wants to get the maximum time this method has
	 *  executed
	 * @return
	 *  The maximum time this method has executed represented by the {@link IoEventType}
	 */
	llong getMaximumTime(EIoEventType type);

	/**
	 * Profile a MessageReceived event. This method will gather the following
	 * informations :
	 * - the method duration
	 * - the shortest execution time
	 * - the slowest execution time
	 * - the average execution time
	 * - the global number of calls
	 *
	 * @param nextFilter The filter to call next
	 * @param session The associated session
	 * @param message the received message
	 */
	virtual void messageReceived(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, sp<EObject>& message) THROWS(EException);

	/**
	 * Profile a MessageSent event. This method will gather the following
	 * informations :
	 * - the method duration
	 * - the shortest execution time
	 * - the slowest execution time
	 * - the average execution time
	 * - the global number of calls
	 *
	 * @param nextFilter The filter to call next
	 * @param session The associated session
	 * @param writeRequest the sent message
	 */
	virtual void messageSent(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) THROWS(EException);

	/**
	 * Profile a SessionCreated event. This method will gather the following
	 * informations :
	 * - the method duration
	 * - the shortest execution time
	 * - the slowest execution time
	 * - the average execution time
	 * - the global number of calls
	 *
	 * @param nextFilter The filter to call next
	 * @param session The associated session
	 */
	virtual void sessionCreated(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session) THROWS(EException);

	/**
	 * Profile a SessionOpened event. This method will gather the following
	 * informations :
	 * - the method duration
	 * - the shortest execution time
	 * - the slowest execution time
	 * - the average execution time
	 * - the global number of calls
	 *
	 * @param nextFilter The filter to call next
	 * @param session The associated session
	 */
	virtual void sessionOpened(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session) THROWS(EException);

	/**
	 * Profile a SessionIdle event. This method will gather the following
	 * informations :
	 * - the method duration
	 * - the shortest execution time
	 * - the slowest execution time
	 * - the average execution time
	 * - the global number of calls
	 *
	 * @param nextFilter The filter to call next
	 * @param session The associated session
	 * @param status The session's status
	 */
	virtual void sessionIdle(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, EIdleStatus status) THROWS(EException);

	/**
	 * Profile a SessionClosed event. This method will gather the following
	 * informations :
	 * - the method duration
	 * - the shortest execution time
	 * - the slowest execution time
	 * - the average execution time
	 * - the global number of calls
	 *
	 * @param nextFilter The filter to call next
	 * @param session The associated session
	 */
	virtual void sessionClosed(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session) THROWS(EException);

private:
	/**
	 * Class that will track the time each method takes and be able to provide information
	 * for each method.
	 *
	 */
	class TimerWorker {
	private:
		/** The sum of all operation durations */
		EAtomicLLong total;

		/** The number of calls */
		EAtomicLLong callsNumber;

		/** The fastest operation */
		EAtomicLLong minimum;

		/** The slowest operation */
		EAtomicLLong maximum;

		/** A lock for synchinized blocks */
		EReentrantLock lock;// = new Object();

	public:
		/**
		 * Creates a new instance of TimerWorker.
		 *
		 */
		TimerWorker() {
			/*
			total = new AtomicLong();
			callsNumber = new AtomicLong();
			minimum = new AtomicLong();
			maximum = new AtomicLong();
			*/
		}

		/**
		 * Add a new operation duration to this class.  Total is updated
		 * and calls is incremented
		 *
		 * @param duration
		 *  The new operation duration
		 */
		void addNewDuration(llong duration) {
			callsNumber.incrementAndGet();
			total.addAndGet(duration);

			SYNCBLOCK(&lock) {
				// this is not entirely thread-safe, must lock
				if (duration < minimum.llongValue()) {
					minimum.set(duration);
				}

				// this is not entirely thread-safe, must lock
				if (duration > maximum.llongValue()) {
					maximum.set(duration);
				}
            }}
		}

		/**
		 * Gets the average reading for this event
		 *
		 * @return the average reading for this event
		 */
		double getAverage() {
			SYNCBLOCK(&lock) {
				llong cn = callsNumber.llongValue();
				if (cn == 0) {
					return 0.0;
				}
				else {
					// There are two operations, we need to synchronize the block
					return total.llongValue() / cn;
				}
            }}
        }

		/**
		 * @return The total number of profiled operation
		 */
		llong getCallsNumber() {
			return callsNumber.llongValue();
		}

		/**
		 * @return the total time
		 */
		llong getTotal() {
			return total.llongValue();
		}

		/**
		 * @return the lowest execution time
		 */
		llong getMinimum() {
			return minimum.llongValue();
		}

		/**
		 * @return the longest execution time
		 */
		llong getMaximum() {
			return maximum.llongValue();
		}
	};

private:
	/** TRhe selected time unit */
	ETimeUnit* volatile timeUnit;

	/** A TimerWorker for the MessageReceived events */
	TimerWorker* messageReceivedTimerWorker;

	/** A flag to tell the filter that the MessageReceived must be profiled */
	boolean profileMessageReceived;// = false;

	/** A TimerWorker for the MessageSent events */
	TimerWorker* messageSentTimerWorker;

	/** A flag to tell the filter that the MessageSent must be profiled */
	boolean profileMessageSent;// = false;

	/** A TimerWorker for the SessionCreated events */
	TimerWorker* sessionCreatedTimerWorker;

	/** A flag to tell the filter that the SessionCreated must be profiled */
	boolean profileSessionCreated;// = false;

	/** A TimerWorker for the SessionOpened events */
	TimerWorker* sessionOpenedTimerWorker;

	/** A flag to tell the filter that the SessionOpened must be profiled */
	boolean profileSessionOpened;// = false;

	/** A TimerWorker for the SessionIdle events */
	TimerWorker* sessionIdleTimerWorker;

	/** A flag to tell the filter that the SessionIdle must be profiled */
	boolean profileSessionIdle;// = false;

	/** A TimerWorker for the SessionClosed events */
	TimerWorker* sessionClosedTimerWorker;

	/** A flag to tell the filter that the SessionClosed must be profiled */
	boolean profileSessionClosed;// = false;

	/**
	 * Create the profilers for a list of {@link IoEventType}.
	 *
	 * @param eventTypes the list of {@link IoEventType} to profile
	 */
	void setProfilers(int eventTypes);

	/**
	 * @return the current time, expressed using the fixed TimeUnit.
	 */
	llong timeNow();
};

} /* namespace eio */
} /* namespace efc */
#endif /* EPROFILERTIMERFILTER_HH_ */
