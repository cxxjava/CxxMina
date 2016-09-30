/*
 * EAbstractIoSessionConfig.hh
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#ifndef EABSTRACTIOSESSIONCONFIG_HH_
#define EABSTRACTIOSESSIONCONFIG_HH_

#include "EIoSessionConfig.hh"
#include "EIdleStatus.hh"
#include "Efc.hh"

namespace efc {
namespace eio {

/**
 * A base implementation of {@link IoSessionConfig}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */
abstract class EAbstractIoSessionConfig : virtual public EIoSessionConfig
{
private:
    int minReadBufferSize;// = 64;

    int readBufferSize;// = 2048;

    int maxReadBufferSize;// = 65536;

    int idleTimeForRead;

    int idleTimeForWrite;

    int idleTimeForBoth;

    int writeTimeout;// = 60;

    boolean useReadOperation;

    int throughputCalculationInterval;// = 3;

protected:
	EAbstractIoSessionConfig();

	/**
     * Implement this method to set all transport-specific configuration
     * properties retrieved from the specified <tt>config</tt>.
     *
     * @param config the {@link IoSessionConfig} to set
     */
    virtual void doSetAll(EIoSessionConfig* config) = 0;
    
public:
    /**
     * {@inheritDoc}
     */
    virtual void setAll(EIoSessionConfig* config);

    /**
     * {@inheritDoc}
     */
    virtual int getReadBufferSize();

    /**
     * {@inheritDoc}
     */
    virtual void setReadBufferSize(int readBufferSize);

    /**
     * {@inheritDoc}
     */
    virtual int getMinReadBufferSize();

    /**
     * {@inheritDoc}
     */
    virtual void setMinReadBufferSize(int minReadBufferSize);

    /**
     * {@inheritDoc}
     */
    virtual int getMaxReadBufferSize();

    /**
     * {@inheritDoc}
     */
    virtual void setMaxReadBufferSize(int maxReadBufferSize);

    /**
     * {@inheritDoc}
     */
    virtual int getIdleTime(EIdleStatus status);

    /**
     * {@inheritDoc}
     */
    virtual llong getIdleTimeInMillis(EIdleStatus status);

    /**
     * {@inheritDoc}
     */
    virtual void setIdleTime(EIdleStatus status, int idleTime);

    /**
     * {@inheritDoc}
     */
    virtual int getBothIdleTime();

    /**
     * {@inheritDoc}
     */
    virtual llong getBothIdleTimeInMillis();

    /**
     * {@inheritDoc}
     */
    virtual int getReaderIdleTime();

    /**
     * {@inheritDoc}
     */
    virtual llong getReaderIdleTimeInMillis();

    /**
     * {@inheritDoc}
     */
    virtual int getWriterIdleTime();

    /**
     * {@inheritDoc}
     */
    virtual llong getWriterIdleTimeInMillis();

    /**
     * {@inheritDoc}
     */
    virtual void setBothIdleTime(int idleTime);

    /**
     * {@inheritDoc}
     */
    virtual void setReaderIdleTime(int idleTime);

    /**
     * {@inheritDoc}
     */
    virtual void setWriterIdleTime(int idleTime);

    /**
     * {@inheritDoc}
     */
    virtual int getWriteTimeout();

    /**
     * {@inheritDoc}
     */
    virtual llong getWriteTimeoutInMillis();

    /**
     * {@inheritDoc}
     */
    virtual void setWriteTimeout(int writeTimeout);

    /**
     * {@inheritDoc}
     */
    virtual boolean isUseReadOperation();

    /**
     * {@inheritDoc}
     */
    virtual void setUseReadOperation(boolean useReadOperation);

    /**
     * {@inheritDoc}
     */
    virtual int getThroughputCalculationInterval();

    /**
     * {@inheritDoc}
     */
    virtual void setThroughputCalculationInterval(int throughputCalculationInterval);

    /**
     * {@inheritDoc}
     */
    virtual llong getThroughputCalculationIntervalInMillis();
};


} /* namespace eio */
} /* namespace efc */

#endif /* EABSTRACTIOSESSIONCONFIG_HH_ */
