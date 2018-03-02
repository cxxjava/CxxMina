/*
 * ENioSocketAcceptor.hh
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#ifndef ENIOSOCKETACCEPTOR_HH_
#define ENIOSOCKETACCEPTOR_HH_

#include "./EAbstractPollingIoAcceptor.hh"
#include "./ESocketAcceptor.hh"
#include "./ESocketSessionConfig.hh"
#include "./ENioSession.hh"
#include "./EDefaultSocketSessionConfig.hh"

namespace efc {
namespace eio {

/**
 * {@link IoAcceptor} for socket transport (TCP/IP).  This class
 * handles incoming TCP/IP based socket connections.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */
class ENioSocketAcceptor : public EAbstractPollingIoAcceptor<sp<EServerSocketChannel> >,
		virtual public ESocketAcceptor {
public:
	virtual ~ENioSocketAcceptor();

	/**
	 * Constructor for {@link NioSocketAcceptor} using default parameters (multiple thread model).
	 */
	ENioSocketAcceptor();

	/**
	 * Constructor for {@link NioSocketAcceptor} using default parameters, and
	 * given number of {@link NioProcessor} for multithreading I/O operations.
	 *
	 * @param processorCount the number of processor to create and place in a
	 * {@link SimpleIoProcessorPool}
	 */
	ENioSocketAcceptor(int processorCount);

	/**
	 *  Constructor for {@link NioSocketAcceptor} with default configuration but a
	 *  specific {@link IoProcessor}, useful for sharing the same processor over multiple
	 *  {@link IoService} of the same type.
	 * @param processor the processor to use for managing I/O events
	 */
	ENioSocketAcceptor(EIoProcessor* processor);

	/**
	 *  Constructor for {@link NioSocketAcceptor} with a given {@link Executor} for handling
	 *  connection events and a given {@link IoProcessor} for handling I/O events, useful for
	 *  sharing the same processor and executor over multiple {@link IoService} of the same type.
	 * @param executor the executor for connection
	 * @param processor the processor for I/O operations
	 */
	ENioSocketAcceptor(EExecutorService* executor, EIoProcessor* processor);

	/**
	 * {@inheritDoc}
	 */
	virtual ETransportMetadata* getTransportMetadata();

	/**
	 * {@inheritDoc}
	 */
	virtual EInetSocketAddress* getLocalAddress();

	/**
	 * {@inheritDoc}
	 */
	virtual EInetSocketAddress* getDefaultLocalAddress();

	/**
	 * {@inheritDoc}
	 */
	virtual void setDefaultLocalAddress(EInetSocketAddress* localAddress);

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isReuseAddress();
	virtual void setReuseAddress(boolean reuseAddress);
	virtual int getBacklog();
	virtual void setBacklog(int backlog);

protected:
	/**
	 * {@inheritDoc}
	 */
	virtual void init() THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void destroy() THROWS(EException);

	/**
	 * Check if we have at least one key whose corresponding channels is
	 * ready for I/O operations.
	 *
	 * This method performs a blocking selection operation.
	 * It returns only after at least one channel is selected,
	 * this selector's wakeup method is invoked, or the current thread
	 * is interrupted, whichever comes first.
	 *
	 * @return The number of keys having their ready-operation set updated
	 * @throws IOException If an I/O error occurs
	 * @throws ClosedSelectorException If this selector is closed
	 */
	virtual int select() THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void wakeup();

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EIterator<sp<EServerSocketChannel> > > selectedHandles();

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EServerSocketChannel> open(EInetSocketAddress* localAddress) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual EInetSocketAddress* localAddress(sp<EServerSocketChannel> handle) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EIoSession> accept(EIoProcessor* processor, sp<EServerSocketChannel> handle) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void close(sp<EServerSocketChannel> handle) THROWS(EException);

private:
	ESelector* volatile selector;
	EDefaultSocketSessionConfig* dssc;
};

} /* namespace eio */
} /* namespace efc */

#endif /* ENIOSOCKETACCEPTOR_HH_ */
