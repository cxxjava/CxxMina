/*
 * EIoSessionConfig.hh
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#ifndef EIOSESSIONCONFIG_HH_
#define EIOSESSIONCONFIG_HH_

#include "./EIdleStatus.hh"
#include "Efc.hh"

namespace efc {
namespace eio {

interface EIoSessionConfig {
	virtual ~EIoSessionConfig() {
	}

	/**
	 * Returns the size of the read buffer that I/O processor allocates
	 * per each read.  It's unusual to adjust this property because
	 * it's often adjusted automatically by the I/O processor.
	 */
	virtual int getReadBufferSize() = 0;

	/**
	 * Sets the size of the read buffer that I/O processor allocates
	 * per each read.  It's unusual to adjust this property because
	 * it's often adjusted automatically by the I/O processor.
	 *
	 * @param readBufferSize The size of the read buffer
	 */
	virtual void setReadBufferSize(int readBufferSize) = 0;

	/**
	 * Returns the minimum size of the read buffer that I/O processor
	 * allocates per each read.  I/O processor will not decrease the
	 * read buffer size to the smaller value than this property value.
	 */
	virtual int getMinReadBufferSize() = 0;

	/**
	 * Sets the minimum size of the read buffer that I/O processor
	 * allocates per each read.  I/O processor will not decrease the
	 * read buffer size to the smaller value than this property value.
	 *
	 * @param minReadBufferSize The minimum size of the read buffer
	 */
	virtual void setMinReadBufferSize(int minReadBufferSize) = 0;

	/**
	 * Returns the maximum size of the read buffer that I/O processor
	 * allocates per each read.  I/O processor will not increase the
	 * read buffer size to the greater value than this property value.
	 */
	virtual int getMaxReadBufferSize() = 0;

	/**
	 * Sets the maximum size of the read buffer that I/O processor
	 * allocates per each read.  I/O processor will not increase the
	 * read buffer size to the greater value than this property value.
	 *
	 * @param maxReadBufferSize The maximum size of the read buffer
	 */
	virtual void setMaxReadBufferSize(int maxReadBufferSize) = 0;

	/**
	 * Returns the interval (seconds) between each throughput calculation.
	 * The default value is <tt>3</tt> seconds.
	 */
	virtual int getThroughputCalculationInterval() = 0;

	/**
	 * Returns the interval (milliseconds) between each throughput calculation.
	 * The default value is <tt>3</tt> seconds.
	 */
	virtual llong getThroughputCalculationIntervalInMillis() = 0;

	/**
	 * Sets the interval (seconds) between each throughput calculation.  The
	 * default value is <tt>3</tt> seconds.
	 *
	 * @param throughputCalculationInterval The interval
	 */
	virtual void setThroughputCalculationInterval(int throughputCalculationInterval) = 0;

	/**
	 * Returns idle time for the specified type of idleness in seconds.
	 *
	 * @param status The status for which we want the idle time (One of READER_IDLE,
     * WRITER_IDLE or BOTH_IDLE)
	 */
	virtual int getIdleTime(EIdleStatus status) = 0;

	/**
	 * Returns idle time for the specified type of idleness in milliseconds.
	 *
	 * @param status The status for which we want the idle time (One of READER_IDLE,
     * WRITER_IDLE or BOTH_IDLE)
	 */
	virtual llong getIdleTimeInMillis(EIdleStatus status) = 0;

	/**
	 * Sets idle time for the specified type of idleness in seconds.
	 *
	 * @param status The status for which we want to set the idle time (One of READER_IDLE,
     * WRITER_IDLE or BOTH_IDLE)
     * @param idleTime The time in second to set
	 */
	virtual void setIdleTime(EIdleStatus status, int idleTime) = 0;

	/**
	 * Returns idle time for {@link IdleStatus#READER_IDLE} in seconds.
	 */
	virtual int getReaderIdleTime() = 0;

	/**
	 * Returns idle time for {@link IdleStatus#READER_IDLE} in milliseconds.
	 */
	virtual llong getReaderIdleTimeInMillis() = 0;

	/**
	 * Sets idle time for {@link IdleStatus#READER_IDLE} in seconds.
	 *
	 * @param idleTime The time to set
	 */
	virtual void setReaderIdleTime(int idleTime) = 0;

	/**
	 * Returns idle time for {@link IdleStatus#WRITER_IDLE} in seconds.
	 */
	virtual int getWriterIdleTime() = 0;

	/**
	 * Returns idle time for {@link IdleStatus#WRITER_IDLE} in milliseconds.
	 */
	virtual llong getWriterIdleTimeInMillis() = 0;

	/**
	 * Sets idle time for {@link IdleStatus#WRITER_IDLE} in seconds.
	 *
	 * @param idleTime The time to set
	 */
	virtual void setWriterIdleTime(int idleTime) = 0;

	/**
	 * Returns idle time for {@link IdleStatus#BOTH_IDLE} in seconds.
	 */
	virtual int getBothIdleTime() = 0;

	/**
	 * Returns idle time for {@link IdleStatus#BOTH_IDLE} in milliseconds.
	 */
	virtual llong getBothIdleTimeInMillis() = 0;

	/**
	 * Sets idle time for {@link IdleStatus#WRITER_IDLE} in seconds.
	 *
	 * @param idleTime The time to set
	 */
	virtual void setBothIdleTime(int idleTime) = 0;

	/**
	 * Returns write timeout in seconds.
	 */
	virtual int getWriteTimeout() = 0;

	/**
	 * Returns write timeout in milliseconds.
	 */
	virtual llong getWriteTimeoutInMillis() = 0;

	/**
	 * Sets write timeout in seconds.
	 *
	 * @param writeTimeout The timeout to set
	 */
	virtual void setWriteTimeout(int writeTimeout) = 0;

	/**
	 * Returns <tt>true</tt> if and only if {@link IoSession#read()} operation
	 * is enabled.  If enabled, all received messages are stored in an internal
	 * {@link BlockingQueue} so you can read received messages in more
	 * convenient way for client applications.  Enabling this option is not
	 * useful to server applications and can cause unintended memory leak, and
	 * therefore it's disabled by default.
	 */
	virtual boolean isUseReadOperation() = 0;

	/**
	 * Enables or disabled {@link IoSession#read()} operation.  If enabled, all
	 * received messages are stored in an internal {@link BlockingQueue} so you
	 * can read received messages in more convenient way for client
	 * applications.  Enabling this option is not useful to server applications
	 * and can cause unintended memory leak, and therefore it's disabled by
	 * default.
	 *
	 * @param useReadOperation <tt>true</tt> if the read operation is enabled, <tt>false</tt> otherwise
	 */
	virtual void setUseReadOperation(boolean useReadOperation) = 0;

	/**
	 * Sets all configuration properties retrieved from the specified
	 * <tt>config</tt>.
	 *
	 * @param config The configuration to use
	 */
	virtual void setAll(EIoSessionConfig* config) = 0;
};

} /* namespace eio */
} /* namespace efc */


#endif /* EIOSESSIONCONFIG_HH_ */
