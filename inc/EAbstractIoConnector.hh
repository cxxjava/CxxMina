/*
 * EAbstractIoConnector.hh
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#ifndef EABSTRACTIOCONNECTOR_HH_
#define EABSTRACTIOCONNECTOR_HH_

#include "./EAbstractIoService.hh"
#include "./EIoConnector.hh"
#include "./EIoSessionConfig.hh"
#include "./EIoSessionInitializer.hh"
#include "Efc.hh"

namespace efc {
namespace eio {

/**
 * A base implementation of {@link IoConnector}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */
abstract class EAbstractIoConnector: public EAbstractIoService,
		virtual public EIoConnector {
public:
	virtual ~EAbstractIoConnector();

	/**
	 * Returns the minimum connection timeout value for this connector
	 *
	 * @return
	 *  The minimum time that this connector can have for a connection
	 *  timeout in milliseconds.
	 */
	long getConnectTimeoutCheckInterval();

	void setConnectTimeoutCheckInterval(long minimumConnectTimeout);

	/**
	 * {@inheritDoc}
	 */
	virtual long getConnectTimeoutMillis();

	/**
	 * Sets the connect timeout value in milliseconds.
	 *
	 */
	virtual void setConnectTimeoutMillis(long connectTimeoutInMillis);

	/**
	 * {@inheritDoc}
	 */
	virtual EInetSocketAddress* getDefaultRemoteAddress();

	/**
	 * {@inheritDoc}
	 */
	virtual void setDefaultRemoteAddress(EInetSocketAddress* defaultRemoteAddress);
	virtual void setDefaultRemoteAddress(const char* hostname, int port);

	/**
	 * {@inheritDoc}
	 */
	virtual EInetSocketAddress* getDefaultLocalAddress();

	/**
	 * {@inheritDoc}
	 */
	virtual void setDefaultLocalAddress(EInetSocketAddress* localAddress);
	virtual void setDefaultLocalAddress(const char* hostname, int port);

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EConnectFuture> connect();

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EConnectFuture> connect(EIoSessionInitializer* sessionInitializer);

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EConnectFuture> connect(EInetSocketAddress* remoteAddress);
	virtual sp<EConnectFuture> connect(const char* hostname, int port);

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EConnectFuture> connect(EInetSocketAddress* remoteAddress,
				EIoSessionInitializer* sessionInitializer);
	virtual sp<EConnectFuture> connect(const char* hostname, int port,
				EIoSessionInitializer* sessionInitializer);

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EConnectFuture> connect(EInetSocketAddress* remoteAddress,
			EInetSocketAddress* localAddress);
	virtual sp<EConnectFuture> connect(const char* rhostname, int rport,
			const char* lhostname, int lport);
	/**
	 * {@inheritDoc}
	 */
	virtual sp<EConnectFuture> connect(EInetSocketAddress* remoteAddress,
				EInetSocketAddress* localAddress,
				EIoSessionInitializer* sessionInitializer);
	virtual sp<EConnectFuture> connect(const char* rhostname, int rport,
				const char* lhostname, int lport,
				EIoSessionInitializer* sessionInitializer);

	/**
	 * {@inheritDoc}
	 */
	virtual EString toString();

protected:
	/**
	 * Constructor for {@link AbstractIoConnector}. You need to provide a default
	 * session configuration and an {@link Executor} for handling I/O events. If
	 * null {@link Executor} is provided, a default one will be created using
	 * {@link Executors#newCachedThreadPool()}.
	 *
	 * {@see AbstractIoService#AbstractIoService(IoSessionConfig, Executor)}
	 *
	 * @param sessionConfig
	 *            the default configuration for the managed {@link IoSession}
	 * @param executor
	 *            the {@link Executor} used for handling execution of I/O
	 *            events. Can be <code>null</code>.
	 */
	EAbstractIoConnector(EIoSessionConfig* sessionConfig,
			EExecutorService* executor);

	/**
	 * Implement this method to perform the actual connect operation.
	 *
	 * @param localAddress <tt>null</tt> if no local address is specified
	 */
	virtual sp<EConnectFuture> connect0(EInetSocketAddress* remoteAddress,
			EInetSocketAddress* localAddress,
			EIoSessionInitializer* sessionInitializer) = 0;

	/**
	 * Adds required internal attributes and {@link IoFutureListener}s
	 * related with event notifications to the specified {@code session}
	 * and {@code future}.  Do not call this method directly;
	 * {@link #finishSessionInitialization(IoSession, IoFuture, IoSessionInitializer)}
	 * will call this method instead.
	 */
	void finishSessionInitialization0(sp<EIoSession>& session, sp<EIoFuture>& future);

private:
	/**
	 * The minimum timeout value that is supported (in milliseconds).
	 */
	long connectTimeoutCheckInterval; // = 50L;

	long connectTimeoutInMillis; // = 60 * 1000L; // 1 minute by default

	/** The remote address we are connected to */
	EInetSocketAddress* defaultRemoteAddress;

	/** The local address */
	EInetSocketAddress* defaultLocalAddress;
};

} /* namespace eio */
} /* namespace efc */

#endif /* EABSTRACTIOCONNECTOR_HH_ */
