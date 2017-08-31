/*
 * ESimpleIoProcessorPool.hh
 *
 *  Created on: 2016-1-27
 *      Author: cxxjava@163.com
 */

#ifndef ESIMPLEIOPROCESSORPOOL_HH_
#define ESIMPLEIOPROCESSORPOOL_HH_

#include "./EIoProcessor.hh"
#include "./EAttributeKey.hh"
#include "./ENioSession.hh"
#include "ELog.hh"

namespace efc {
namespace eio {

/**
 * An {@link IoProcessor} pool that distributes {@link IoSession}s into one or more
 * {@link IoProcessor}s. Most current transport implementations use this pool internally
 * to perform better in a multi-core environment, and therefore, you won't need to
 * use this pool directly unless you are running multiple {@link IoService}s in the
 * same JVM.
 * <p>
 * If you are running multiple {@link IoService}s, you could want to share the pool
 * among all services.  To do so, you can create a new {@link SimpleIoProcessorPool}
 * instance by yourself and provide the pool as a constructor parameter when you
 * create the services.
 * <p>
 * This pool uses Java reflection API to create multiple {@link IoProcessor} instances.
 * It tries to instantiate the processor in the following order:
 * <ol>
 * <li>A public constructor with one {@link ExecutorService} parameter.</li>
 * <li>A public constructor with one {@link Executor} parameter.</li>
 * <li>A public default constructor</li>
 * </ol>
 * The following is an example for the NIO socket transport:
 * <pre><code>
 * // Create a shared pool.
 * SimpleIoProcessorPool&lt;NioSession&gt; pool =
 *         new SimpleIoProcessorPool&lt;NioSession&gt;(NioProcessor.class, 16);
 *
 * // Create two services that share the same pool.
 * SocketAcceptor acceptor = new NioSocketAcceptor(pool);
 * SocketConnector connector = new NioSocketConnector(pool);
 *
 * ...
 *
 * // Release related resources.
 * connector.dispose();
 * acceptor.dispose();
 * pool.dispose();
 * </code></pre>
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 *
 * @param <S> the type of the {@link IoSession} to be managed by the specified
 *            {@link IoProcessor}.
 */

class ESimpleIoProcessorPool: public virtual EIoProcessor {
public:
	virtual ~ESimpleIoProcessorPool() throw();

	/**
	 * Creates a new instance of SimpleIoProcessorPool with a default
	 * size of NbCPUs +1.
	 *
	 * @param processorType The type of IoProcessor to use
	 */
	ESimpleIoProcessorPool();

	/**
	 * Creates a new instance of SimpleIoProcessorPool with a defined
	 * number of IoProcessors in the pool
	 *
	 * @param size The number of IoProcessor in the pool
	 */
	ESimpleIoProcessorPool(int size);

	/**
	 * Returns <tt>true</tt> if and if only {@link #dispose()} method has
	 * been called.  Please note that this method will return <tt>true</tt>
	 * even after all the related resources are released.
	 */
	virtual boolean isDisposing();

	/**
	 * Returns <tt>true</tt> if and if only all resources of this processor
	 * have been disposed.
	 */
	virtual boolean isDisposed();;

	/**
	 * Releases any resources allocated by this processor.  Please note that
	 * the resources might not be released as long as there are any sessions
	 * managed by this processor.  Most implementations will close all sessions
	 * immediately and release the related resources.
	 */
	virtual void dispose();

	/**
	 * Adds the specified {@code session} to the I/O processor so that
	 * the I/O processor starts to perform any I/O operations related
	 * with the {@code session}.
	 *
	 * @param session The added session
	 */
	virtual void add(sp<EIoSession> session);

	/**
	 * Flushes the internal write request queue of the specified
	 * {@code session}.
	 *
	 * @param session The session we want the message to be written
	 */
	virtual void flush(sp<EIoSession>& session);

	/**
	 * Writes the WriteRequest for the specified {@code session}.
	 *
	 * @param session The session we want the message to be written
	 * @param writeRequest the WriteRequest to write
	 */
	virtual void write(sp<EIoSession>& session, sp<EWriteRequest>& writeRequest);

	/**
	 * Removes and closes the specified {@code session} from the I/O
	 * processor so that the I/O processor closes the connection
	 * associated with the {@code session} and releases any other related
	 * resources.
	 */
	virtual void remove(sp<EIoSession>& session);

	/**
	 * Controls the traffic of the specified {@code session} depending of the
	 * {@link IoSession#isReadSuspended()} and {@link IoSession#isWriteSuspended()}
	 * flags
	 */
	virtual void updateTrafficControl(sp<EIoSession>& session);

private:
	/** A logger for this class */
	static sp<ELogger> LOGGER;// = LoggerFactory.getLogger(SimpleIoProcessorPool.class);

	/** The default pool size, when no size is provided. */
	static int DEFAULT_SIZE;// = Runtime.getRuntime().availableProcessors() + 1;

	/** A key used to store the processor pool in the session's Attributes */
	static sp<EAttributeKey> PROCESSOR;// = new AttributeKey(SimpleIoProcessorPool.class, "processor");

	/** The pool table */
	EA<sp<EIoProcessor> >* pool;

	/** The contained  which is passed to the IoProcessor when they are created */
	EExecutorService* executor;

	/** A lock to protect the disposal against concurrent calls */
	EReentrantLock disposalLock;// = new Object();

	/** A flg set to true if the IoProcessor in the pool are being disposed */
	volatile boolean disposing;

	/** A flag set to true if all the IoProcessor contained in the pool have been disposed */
	volatile boolean disposed;

	void ctor(int size);

	/**
	 * Find the processor associated to a session. If it hasen't be stored into
	 * the session's attributes, pick a new processor and stores it.
	 */
	sp<EIoProcessor> getProcessor(sp<EIoSession>& session);
};

} /* namespace eio */
} /* namespace efc */
#endif /* ESIMPLEIOPROCESSORPOOL_HH_ */
