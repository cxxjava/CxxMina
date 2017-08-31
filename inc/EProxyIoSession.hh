/*
 * EProxyIoSession.hh
 *
 *  Created on: 2016-6-16
 *      Author: cxxjava@lotuseed.com
 */

#ifndef EPROXYIOSESSION_HH_
#define EPROXYIOSESSION_HH_

#include "./EIoSession.hh"
#include "./EProxyRequest.hh"
#include "./EProxyFilter.hh"
#include "./EProxyConnector.hh"
#include "./EProxyLogicHandler.hh"
#include "./EIoSessionEventQueue.hh"
#include "./EHttpAuthenticationMethods.hh"

namespace efc {
namespace eio {

/**
 * ProxyIoSession.java - Class that contains all informations for the current proxy
 * authentication session.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since MINA 2.0.0-M3
 */

class EProxyIoSession: public EObject {
public:
	DECLARE_STATIC_INITZZ;

	static sp<EString> PROXY_SESSION;

public:
	virtual ~EProxyIoSession();

	/**
	 * Constructor.
	 *
	 * @param proxyAddress the IP address of the proxy server
	 * @param request the proxy request
	 */
	EProxyIoSession(EInetSocketAddress* proxyAddress, sp<EProxyRequest>& request);

	/**
	 * @return the pending event queue.
	 */
	EIoSessionEventQueue* getEventQueue();

	/**
	 * @return the list of the prefered order for the authentication methods.
	 * This list is used by the {@link HttpSmartProxyHandler} to determine
	 * which authentication mechanism to use first between those accepted by the
	 * proxy server. This list is only used when connecting to an http proxy.
	 */
	EList<int>* getPreferedOrder();

	/**
	 * Sets the ordered list of prefered authentication mechanisms.
	 *
	 * @param preferedOrder the ordered list
	 */
	void setPreferedOrder(EList<int>* preferedOrder);

	/**
	 * @return the {@link ProxyLogicHandler} currently in use.
	 */
	EProxyLogicHandler* getHandler();

	/**
	 * Sets the {@link ProxyLogicHandler} to use.
	 *
	 * @param handler the {@link ProxyLogicHandler} instance
	 */
	void setHandler(EProxyLogicHandler* handler);

	/**
	 * @return the {@link ProxyFilter}.
	 */
	EProxyFilter* getProxyFilter();

	/**
	 * Sets the {@link ProxyFilter}.
	 * Note : Please do not call this method from your code it could result
	 * in an unexpected behaviour.
	 *
	 * @param proxyFilter the filter
	 */
	void setProxyFilter(EProxyFilter* proxyFilter);

	/**
	 * @return the proxy request.
	 */
	sp<EProxyRequest> getRequest();

	/**
	 * @return the current {@link IoSession}.
	 */
	sp<EIoSession> getSession();

	/**
	 * Sets the {@link IoSession} in use.
	 * Note : Please do not call this method from your code it could result in an
	 * unexpected behaviour.
	 *
	 * @param session the current io session
	 */
	void setSession(sp<EIoSession>& session);

	/**
	 * @return the proxy connector.
	 */
	EProxyConnector* getConnector();

	/**
	 * Sets the connector reference of this proxy session.
	 * Note : Please do not call this method from your code it could result in an
	 * unexpected behaviour.
	 *
	 * @param connector the proxy connector
	 */
	void setConnector(EProxyConnector* connector);

	/**
	 * @return the IP address of the proxy server.
	 */
	EInetSocketAddress* getProxyAddress();

	/**
	 * @return true if the current authentication process is not finished
	 * but the server has closed the connection.
	 */
	boolean isReconnectionNeeded();

	/**
	 * Sets the reconnection needed flag. If set to true, it means that an
	 * authentication process is currently running but the proxy server did not
	 * kept the connection alive. So we need to reconnect to the server to complete
	 * the process.
	 * Note : Please do not call this method from your code it could result in an
	 * unexpected behaviour.
	 *
	 * @param reconnectionNeeded the value to set the flag to
	 */
	void setReconnectionNeeded(boolean reconnectionNeeded);

	/**
	 * @return true if authentication failed.
	 */
	boolean isAuthenticationFailed();

	/**
	 * Sets the authentication failed flag.
	 *
	 * @param authenticationFailed the value to set the flag to
	 */
	void setAuthenticationFailed(boolean authenticationFailed);

private:
	/**
	 * The list contains the authentication methods to use.
	 * The order in the list is revelant : if first method is available
	 * then it will be used etc ...
	 */
	EArrayList<int> preferedOrder;

	/**
	 * The request to send to the proxy.
	 */
	sp<EProxyRequest> request;

	/**
	 * The currently selected proxy handler.
	 */
	EProxyLogicHandler* handler;

	/**
	 * Parent {@link ProxyFilter} handling the session.
	 */
	EProxyFilter* proxyFilter;

	/**
	 * The session.
	 */
	sp<EIoSession> session;

	/**
	 * The proxy connector.
	 */
	EProxyConnector* connector;

	/**
	 * Address of the proxy server.
	 */
	EInetSocketAddress* proxyAddress;// = null;

	/**
	 * A flag that indicates that the proxy closed the connection before handshake
	 * is done. So we need to reconnect to the proxy to continue the handshaking
	 * process.
	 */
	boolean reconnectionNeeded;// = false;

	/**
	 * The session event queue.
	 */
	EIoSessionEventQueue* eventQueue;// = new IoSessionEventQueue(this);

	/**
	 * Set to true when an exception has been thrown or if authentication failed.
	 */
	boolean authenticationFailed;

	/**
	 * Sets the proxy request.
	 *
	 * @param request the proxy request
	 */
	void setRequest(sp<EProxyRequest> request);

	/**
	 * Sets the IP address of the proxy server.
	 *
	 * @param proxyAddress the IP address of the proxy server
	 */
	void setProxyAddress(EInetSocketAddress* proxyAddress);
};

} /* namespace eio */
} /* namespace efc */
#endif /* EPROXYIOSESSION_HH_ */
