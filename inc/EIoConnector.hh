/*
 * EIoConnector.hh
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#ifndef EIOCONNECTOR_HH_
#define EIOCONNECTOR_HH_

#include "./EIoService.hh"
#include "./EConnectFuture.hh"
#include "./EIoSessionInitializer.hh"

namespace efc {
namespace eio {

/**
 * Connects to endpoint, communicates with the server, and fires events to
 * {@link IoHandler}s.
 * <p>
 * Please refer to
 * <a href="../../../../../xref-examples/org/apache/mina/examples/netcat/Main.html">NetCat</a>
 * example.
 * <p>
 * You should connect to the desired socket address to start communication,
 * and then events for incoming connections will be sent to the specified
 * default {@link IoHandler}.
 * <p>
 * Threads connect to endpoint start automatically when
 * {@link #connect(SocketAddress)} is invoked, and stop when all
 * connection attempts are finished.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

interface EIoConnector : virtual public EIoService {
	virtual ~EIoConnector() {
	}

	/**
	 * Returns the connect timeout in milliseconds.  The default value is 1 minute.
	 */
	virtual long getConnectTimeoutMillis() = 0;

	/**
	 * Sets the connect timeout in milliseconds.  The default value is 1 minute.
	 */
	virtual void setConnectTimeoutMillis(long connectTimeoutInMillis) = 0;

	/**
	 * Returns the default remote address to connect to when no argument
	 * is specified in {@link #connect()} method.
	 */
	virtual EInetSocketAddress* getDefaultRemoteAddress() = 0;

	/**
	 * Sets the default remote address to connect to when no argument is
	 * specified in {@link #connect()} method.
	 */
	virtual void setDefaultRemoteAddress(EInetSocketAddress* defaultRemoteAddress) = 0;
	virtual void setDefaultRemoteAddress(const char* hostname, int port) = 0;

	/**
	 * Connects to the {@link #setDefaultRemoteAddress(SocketAddress) default remote address}.
	 *
	 * @throws IllegalStateException if no default remoted address is set.
	 */
	virtual sp<EConnectFuture> connect() = 0;

	/**
	 * Connects to the {@link #setDefaultRemoteAddress(SocketAddress) default
	 * remote address} and invokes the <code>ioSessionInitializer</code> when
	 * the IoSession is created but before {@link IoHandler#sessionCreated(IoSession)}
	 * is invoked.  There is <em>no</em> guarantee that the <code>ioSessionInitializer</code>
	 * will be invoked before this method returns.
	 *
	 * @param sessionInitializer  the callback to invoke when the {@link IoSession} object is created
	 *
	 * @throws IllegalStateException if no default remote address is set.
	 */
	virtual sp<EConnectFuture> connect(EIoSessionInitializer* sessionInitializer) = 0;

	/**
	 * Connects to the specified remote address.
	 *
	 * @return the {@link ConnectFuture} instance which is completed when the
	 *         connection attempt initiated by this call succeeds or fails.
	 */
	virtual sp<EConnectFuture> connect(EInetSocketAddress* remoteAddress) = 0;
	virtual sp<EConnectFuture> connect(const char* hostname, int port) = 0;

	/**
	 * Connects to the specified remote address and invokes
	 * the <code>ioSessionInitializer</code> when the IoSession is created but before
	 * {@link IoHandler#sessionCreated(IoSession)} is invoked.  There is <em>no</em>
	 * guarantee that the <code>ioSessionInitializer</code> will be invoked before
	 * this method returns.
	 *
	 * @param remoteAddress  the remote address to connect to
	 * @param sessionInitializer  the callback to invoke when the {@link IoSession} object is created
	 *
	 * @return the {@link ConnectFuture} instance which is completed when the
	 *         connection attempt initiated by this call succeeds or fails.
	 */
	virtual sp<EConnectFuture> connect(EInetSocketAddress* remoteAddress, EIoSessionInitializer* sessionInitializer) = 0;
	virtual sp<EConnectFuture> connect(const char* hostname, int port, EIoSessionInitializer* sessionInitializer) = 0;

	/**
	 * Connects to the specified remote address binding to the specified local address.
	 *
	 * @return the {@link ConnectFuture} instance which is completed when the
	 *         connection attempt initiated by this call succeeds or fails.
	 */
	virtual sp<EConnectFuture> connect(EInetSocketAddress* remoteAddress, EInetSocketAddress* localAddress) = 0;
	virtual sp<EConnectFuture> connect(const char* rhostname, int rport, const char* lhostname, int lport) = 0;

	/**
	 * Connects to the specified remote address binding to the specified local
	 * address and and invokes the <code>ioSessionInitializer</code> when the
	 * IoSession is created but before {@link IoHandler#sessionCreated(IoSession)}
	 * is invoked.  There is <em>no</em> guarantee that the <code>ioSessionInitializer</code>
	 * will be invoked before this method returns.
	 *
	 * @param remoteAddress  the remote address to connect to
	 * @param localAddress  the local interface to bind to
	 * @param sessionInitializer  the callback to invoke when the {@link IoSession} object is created
	 *
	 * @return the {@link ConnectFuture} instance which is completed when the
	 *         connection attempt initiated by this call succeeds or fails.
	 */
	virtual sp<EConnectFuture> connect(EInetSocketAddress* remoteAddress, EInetSocketAddress* localAddress,
	            EIoSessionInitializer* sessionInitializer) = 0;
	virtual sp<EConnectFuture> connect(const char* rhostname, int rport, const char* lhostname, int lport,
	            EIoSessionInitializer* sessionInitializer) = 0;
};

}
} // namespace efc::eio

#endif /* EIOCONNECTOR_HH_ */
