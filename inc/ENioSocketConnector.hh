/*
 * ENioSocketConnector.hh
 *
 *  Created on: 2016-3-22
 *      Author: cxxjava@163.com
 */

#ifndef ENIOSOCKETCONNECTOR_HH_
#define ENIOSOCKETCONNECTOR_HH_

#include "./EAbstractPollingIoConnector.hh"
#include "./ESocketConnector.hh"
#include "./ESocketSessionConfig.hh"
#include "./EDefaultSocketSessionConfig.hh"

namespace efc {
namespace eio {

/**
 * {@link IoConnector} for socket transport (TCP/IP).
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class ENioSocketConnector: public EAbstractPollingIoConnector<sp<ESocketChannel> >,
	virtual public ESocketConnector {
public:
	virtual ~ENioSocketConnector();

	/**
	 * Constructor for {@link NioSocketConnector} with default configuration (multiple thread model).
	 */
	ENioSocketConnector();

	/**
	 * Constructor for {@link NioSocketConnector} with default configuration, and
	 * given number of {@link NioProcessor} for multithreading I/O operations
	 * @param processorCount the number of processor to create and place in a
	 * {@link SimpleIoProcessorPool}
	 */
	ENioSocketConnector(int processorCount);

	/**
	 *  Constructor for {@link NioSocketConnector} with default configuration but a
	 *  specific {@link IoProcessor}, useful for sharing the same processor over multiple
	 *  {@link IoService} of the same type.
	 * @param processor the processor to use for managing I/O events
	 */
	ENioSocketConnector(EIoProcessor* processor);

	/**
	 *  Constructor for {@link NioSocketConnector} with a given {@link Executor} for handling
	 *  connection events and a given {@link IoProcessor} for handling I/O events, useful for sharing
	 *  the same processor and executor over multiple {@link IoService} of the same type.
	 * @param executor the executor for connection
	 * @param processor the processor for I/O operations
	 */
	ENioSocketConnector(EExecutorService* executor, EIoProcessor* processor);

	/**
	 * {@inheritDoc}
	 */
	virtual ETransportMetadata* getTransportMetadata();

	/**
	 * {@inheritDoc}
	 */
	virtual EInetSocketAddress* getDefaultRemoteAddress();

	/**
	 * {@inheritDoc}
	 */
	virtual void setDefaultRemoteAddress(EInetSocketAddress* defaultRemoteAddress);

	/**
	 * {@inheritDoc}
	 */
	virtual void init() THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void destroy() THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual sp<ESocketChannel> newHandle(EInetSocketAddress* localAddress) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual boolean connect(sp<ESocketChannel> handle, EInetSocketAddress* remoteAddress) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual boolean finishConnect(sp<ESocketChannel> handle) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EIoSession> newSession(EIoProcessor* processor, sp<ESocketChannel> handle) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void close(sp<ESocketChannel> handle) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void wakeup();

	/**
	 * {@inheritDoc}
	 */
	virtual int select(int timeout) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EIterator<sp<ESocketChannel> > > selectedHandles();

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EIterator<sp<ESocketChannel> > > allHandles();

	/**
	 * {@inheritDoc}
	 */
	virtual void register_(sp<ESocketChannel> handle, sp<ConnectionRequest> request) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual sp<ConnectionRequest> getConnectionRequest(sp<ESocketChannel> handle);

	/**
	 * {@inheritDoc}
	 */
	virtual ESocketSessionConfig* getSessionConfig();

private:
	ESelector* volatile selector;
	EDefaultSocketSessionConfig* dssc;
};

} /* namespace eio */
} /* namespace efc */
#endif /* ENIOSOCKETCONNECTOR_HH_ */
