/*
 * EIoServiceStatistics.hh
 *
 *  Created on: 2014-9-15
 *      Author: cxxjava@163.com
 */

#ifndef EIOSERVICESTATISTICS_HH_
#define EIOSERVICESTATISTICS_HH_

#include "Efc.hh"

namespace efc {
namespace eio {

class EAbstractIoService;

/**
 * Provides usage statistics for an {@link AbstractIoService} instance.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since 2.0.0-M3
 */
class EIoServiceStatistics : public EObject {
public:
	EIoServiceStatistics(EAbstractIoService* service);

	/**
	 * Returns the maximum number of sessions which were being managed at the
	 * same time.
	 */
	int getLargestManagedSessionCount();

	/**
	 * Returns the cumulative number of sessions which were managed (or are
	 * being managed) by this service, which means 'currently managed session
	 * count + closed session count'.
	 */
	long getCumulativeManagedSessionCount();

	/**
	 * Returns the time in millis when I/O occurred lastly.
	 */
	llong getLastIoTime();

	/**
	 * Returns the time in millis when read operation occurred lastly.
	 */
	llong getLastReadTime();

	/**
	 * Returns the time in millis when write operation occurred lastly.
	 */
	llong getLastWriteTime();

	/**
	 * Returns the number of bytes read by this service
	 *
	 * @return
	 *     The number of bytes this service has read
	 */
	long getReadBytes();

	/**
	 * Returns the number of bytes written out by this service
	 *
	 * @return
	 *     The number of bytes this service has written
	 */
	long getWrittenBytes();

	/**
	 * Returns the number of messages this services has read
	 *
	 * @return
	 *     The number of messages this services has read
	 */
	long getReadMessages();

	/**
	 * Returns the number of messages this service has written
	 *
	 * @return
	 *     The number of messages this service has written
	 */
	long getWrittenMessages();

	/**
	 * Returns the number of read bytes per second.
	 */
	double getReadBytesThroughput();

	/**
	 * Returns the number of written bytes per second.
	 */
	double getWrittenBytesThroughput();

	/**
	 * Returns the number of read messages per second.
	 */
	double getReadMessagesThroughput();

	/**
	 * Returns the number of written messages per second.
	 */
	double getWrittenMessagesThroughput();

	/**
	 * Returns the maximum of the {@link #getReadBytesThroughput() readBytesThroughput}.
	 */
	double getLargestReadBytesThroughput();

	/**
	 * Returns the maximum of the {@link #getWrittenBytesThroughput() writtenBytesThroughput}.
	 */
	double getLargestWrittenBytesThroughput();

	/**
	 * Returns the maximum of the {@link #getReadMessagesThroughput() readMessagesThroughput}.
	 */
	double getLargestReadMessagesThroughput();

	/**
	 * Returns the maximum of the {@link #getWrittenMessagesThroughput() writtenMessagesThroughput}.
	 */
	double getLargestWrittenMessagesThroughput();

	/**
	 * Returns the interval (seconds) between each throughput calculation.
	 * The default value is <tt>3</tt> seconds.
	 */
	int getThroughputCalculationInterval();

	/**
	 * Returns the interval (milliseconds) between each throughput calculation.
	 * The default value is <tt>3</tt> seconds.
	 */
	long getThroughputCalculationIntervalInMillis();

	/**
	 * Sets the interval (seconds) between each throughput calculation.  The
	 * default value is <tt>3</tt> seconds.
	 *
	 * @param throughputCalculationInterval The interval between two calculation
	 */
	void setThroughputCalculationInterval(int throughputCalculationInterval);

	/**
	 * Updates the throughput counters.
	 *
	 * @param currentTime The current time
	 */
	void updateThroughput(llong currentTime);

	/**
	 * Increases the count of read bytes by <code>increment</code> and sets
	 * the last read time to <code>currentTime</code>.
	 */
	void increaseReadBytes(long increment, llong currentTime);

	/**
	 * Increases the count of read messages by 1 and sets the last read time to
	 * <code>currentTime</code>.
	 */
	void increaseReadMessages(llong currentTime);

	/**
	 * Increases the count of written bytes by <code>increment</code> and sets
	 * the last write time to <code>currentTime</code>.
	 */
	void increaseWrittenBytes(int increment, llong currentTime);

	/**
	 * Increases the count of written messages by 1 and sets the last write time to
	 * <code>currentTime</code>.
	 */
	void increaseWrittenMessages(llong currentTime);

	/**
	 * Returns the count of bytes scheduled for write.
	 */
	int getScheduledWriteBytes();

	/**
	 * Increments by <code>increment</code> the count of bytes scheduled for write.
	 *
	 * @param increment The number of added bytes fro write
	 */
	void increaseScheduledWriteBytes(int increment);

	/**
	 * Returns the count of messages scheduled for write.
	 */
	int getScheduledWriteMessages();

	/**
	 * Increments by 1 the count of messages scheduled for write.
	 */
	void increaseScheduledWriteMessages();

	/**
	 * Decrements by 1 the count of messages scheduled for write.
	 */
	void decreaseScheduledWriteMessages();

//protected:
	/**
	 * Sets last time at which a read occurred on the service.
	 */
	void setLastReadTime(llong lastReadTime);

	/**
	 * Sets last time at which a write occurred on the service.
	 */
	void setLastWriteTime(llong lastWriteTime);

	/**
	 * Sets the time at which throughtput counters where updated.
	 *
	 * @param lastThroughputCalculationTime The time at which throughput counters where updated.
	 */
	void setLastThroughputCalculationTime(llong lastThroughputCalculationTime);

private:
	EAbstractIoService* service;

	/** The number of bytes read per second */
	double readBytesThroughput;

	/** The number of bytes written per second */
	double writtenBytesThroughput;

	/** The number of messages read per second */
	double readMessagesThroughput;

	/** The number of messages written per second */
	double writtenMessagesThroughput;

	/** The biggest number of bytes read per second */
	double largestReadBytesThroughput;

	/** The biggest number of bytes written per second */
	double largestWrittenBytesThroughput;

	/** The biggest number of messages read per second */
	double largestReadMessagesThroughput;

	/** The biggest number of messages written per second */
	double largestWrittenMessagesThroughput;

	/** The number of read bytes since the service has been started */
	long readBytes;

	/** The number of written bytes since the service has been started */
	long writtenBytes;

	/** The number of read messages since the service has been started */
	long readMessages;

	/** The number of written messages since the service has been started */
	long writtenMessages;

	/** The time the last read operation occurred */
	llong lastReadTime;

	/** The time the last write operation occurred */
	llong lastWriteTime;

	long lastReadBytes;

	long lastWrittenBytes;

	long lastReadMessages;

	long lastWrittenMessages;

	llong lastThroughputCalculationTime;

	int scheduledWriteBytes;

	int scheduledWriteMessages;

	/** The time (in second) between the computation of the service's statistics */
	EAtomicInteger throughputCalculationInterval;// = new AtomicInteger(3);

	ESpinLock throughputCalculationLock;// = new ReentrantLock();

    /**
     * Resets the throughput counters of the service if none session
     * is currently managed.
     */
    void resetThroughput();
};

} /* namespace eio */
} /* namespace efc */
#endif /* EIOSERVICESTATISTICS_HH_ */
