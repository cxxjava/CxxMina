/*
 * EProxyConnector.hh
 *
 *  Created on: 2016-7-5
 *      Author: cxxjava@lotuseed.com
 */

#ifndef EPROXYCONNECTOR_HH_
#define EPROXYCONNECTOR_HH_

#include "./EAbstractIoConnector.hh"
#include "./ESocketConnector.hh"
#include "./EDefaultConnectFuture.hh"

namespace efc {
namespace eio {

/**
 * ProxyConnector.java - Decorator for {@link SocketConnector} to provide proxy
 * support, as suggested by MINA list discussions.
 * <p>
 * Operates by intercepting connect requests and replacing the endpoint address
 * with the proxy address, then adding a {@link ProxyFilter} as the first
 * {@link IoFilter} which performs any necessary handshaking with the proxy
 * before allowing data to flow normally. During the handshake, any outgoing
 * write requests are buffered.
 *
 * @see <a
 *      href="http://www.nabble.com/Meta-Transport%3A-an-idea-on-implementing-reconnection-and-proxy-td12969001.html">Proxy
 *      reconnection</a>
 * @see <a href="http://issues.apache.org/jira/browse/DIRMINA-415">Proxy
 *      support</a>
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since MINA 2.0.0-M3
 */

class EProxyIoSession;
class EProxyFilter;
    
class EProxyConnector: public EAbstractIoConnector {
public:
	virtual ~EProxyConnector();

	/**
	 * Creates a new proxy connector.
	 */
	EProxyConnector();

	/**
	 * Creates a new proxy connector.
	 *
	 * @param connector Connector used to establish proxy connections.
	 */
	EProxyConnector(ESocketConnector* connector);

	/**
	 * Creates a new proxy connector.
	 *
	 * @param connector The Connector used to establish proxy connections.
	 * @param config The session confiugarion to use
	 * @param executor The associated executor
	 */
	EProxyConnector(ESocketConnector* connector, EIoSessionConfig* config, EExecutorService* executor);

	/**
	 * {@inheritDoc}
	 */
	EIoSessionConfig* getSessionConfig();

	/**
	 * @return the {@link ProxyIoSession} linked with this connector.
	 */
	sp<EProxyIoSession> getProxyIoSession();

	/**
	 * Sets the proxy session object of this connector.
	 * @param proxyIoSession the configuration of this connector.
	 */
	void setProxyIoSession(sp<EProxyIoSession>& proxyIoSession);

	/**
	 * Cancels the real connection when reconnection is in use.
	 */
	void cancelConnectFuture();
    
    /**
     * Fires the connection event on the real future to notify the client.
     *
     * @param session the current session
     * @return the future holding the connected session
     */
    sp<EConnectFuture> fireConnected(sp<EIoSession>& session);

protected:
	/**
	 * Connects to the specified <code>address</code>.  If communication starts
	 * successfully, events are fired to the connector's <code>handler</code>.
	 *
	 * @param remoteAddress the remote address to connect to
	 * @param localAddress the local address
	 * @param sessionInitializer the session initializer
	 * @return {@link ConnectFuture} that will tell the result of the connection attempt
	 */
	virtual sp<EConnectFuture> connect0(EInetSocketAddress* remoteAddress, EInetSocketAddress* localAddress,
			EIoSessionInitializer* sessionInitializer);

	/**
	 * @return the {@link SocketConnector} to be used for connections
	 * to the proxy server.
	 */
	ESocketConnector* getConnector();

	/**
	 * {@inheritDoc}
	 */
	virtual void dispose0() THROWS(EException);

private:
	/**
	 * Wrapped connector to use for outgoing TCP connections.
	 */
	ESocketConnector* connector;// = null;

	/**
	 * Proxy filter instance.
	 */
	EProxyFilter* proxyFilter;// = new ProxyFilter();

	/**
	 * The {@link ProxyIoSession} in use.
	 */
	sp<EProxyIoSession> proxyIoSession;

	/**
	 * This future will notify it's listeners when really connected to the target
	 */
	sp<EDefaultConnectFuture> future;

	//
	EIoSessionConfig* sessionConfig;

	/**
	 * Sets the {@link SocketConnector} to be used for connections
	 * to the proxy server.
	 *
	 * @param connector the connector to use
	 */
	void setConnector(ESocketConnector* connector);
};

} /* namespace eio */
} /* namespace efc */
#endif /* EPROXYCONNECTOR_HH_ */
