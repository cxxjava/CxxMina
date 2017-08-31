/*
 * EProxyFilter.hh
 *
 *  Created on: 2016-6-15
 *      Author: cxxjava@lotuseed.com
 */

#ifndef EPROXYFILTER_HH_
#define EPROXYFILTER_HH_

#include "./EIoFilterAdapter.hh"
#include "./EIoFilterChain.hh"
#include "./EIoSession.hh"
#include "ELog.hh"

namespace efc {
namespace eio {

/**
 * ProxyFilter.java - Proxy {@link IoFilter}.
 * Automatically inserted into the {@link IoFilter} chain by {@link ProxyConnector}.
 * Sends the initial handshake message to the proxy and handles any response
 * to the handshake. Once the handshake has completed and the proxied connection has been
 * established this filter becomes transparent to data flowing through the connection.
 * <p>
 * Based upon SSLFilter from mina-filter-ssl.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since MINA 2.0.0-M3
 */

class EProxyLogicHandler;

class EProxyFilter: public EIoFilterAdapter {
public:
	/**
	 * Create a new {@link ProxyFilter}.
	 */
	EProxyFilter();

	/**
	 * Called before the filter is added into the filter chain.
	 * Checks if chain already holds an {@link ProxyFilter} instance.
	 *
	 * @param chain the filter chain
	 * @param name the name assigned to this filter
	 * @param nextFilter the next filter
	 * @throws IllegalStateException if chain already contains an instance of
	 * {@link ProxyFilter}
	 */
	virtual void onPreAdd(EIoFilterChain* parent, const char* name, EIoFilter::NextFilter* nextFilter) THROWS(EException);

	/**
	 * Called when the filter is removed from the filter chain.
	 * Cleans the {@link ProxyIoSession} instance from the session.
	 *
	 * @param chain the filter chain
	 * @param name the name assigned to this filter
	 * @param nextFilter the next filter
	 */
	virtual void onPreRemove(EIoFilterChain* parent, const char* name, EIoFilter::NextFilter* nextFilter) THROWS(EException);

	/**
	 * Called when the session is created. Will create the handler able to handle
	 * the {@link ProxyIoSession#getRequest()} request stored in the session. Event
	 * is stored in an {@link IoSessionEventQueue} for later delivery to the next filter
	 * in the chain when the handshake would have succeed. This will prevent the rest of
	 * the filter chain from being affected by this filter internals.
	 *
	 * Please note that this event can occur multiple times because of some http
	 * proxies not handling keep-alive connections thus needing multiple sessions
	 * during the handshake.
	 *
	 * @param nextFilter the next filter in filter chain
	 * @param session the session object
	 */
	virtual void sessionCreated(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session) THROWS(EException);

	/**
	 * Event is stored in an {@link IoSessionEventQueue} for later delivery to the next filter
	 * in the chain when the handshake would have succeed. This will prevent the rest of
	 * the filter chain from being affected by this filter internals.
	 *
	 * @param nextFilter the next filter in filter chain
	 * @param session the session object
	 */
	virtual void sessionOpened(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session) THROWS(EException);

	/**
	 * Event is stored in an {@link IoSessionEventQueue} for later delivery to the next filter
	 * in the chain when the handshake would have succeed. This will prevent the rest of
	 * the filter chain from being affected by this filter internals.
	 *
	 * @param nextFilter the next filter in filter chain
	 * @param session the session object
	 */
	virtual void sessionClosed(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session) THROWS(EException);

	/**
	 * Event is stored in an {@link IoSessionEventQueue} for later delivery to the next filter
	 * in the chain when the handshake would have succeed. This will prevent the rest of
	 * the filter chain from being affected by this filter internals.
	 *
	 * @param nextFilter the next filter in filter chain
	 * @param session the session object
	 */
	virtual void sessionIdle(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, EIdleStatus status) THROWS(EException);

	/**
	 * Called when an exception occurs in the chain. A flag is set in the
	 * {@link ProxyIoSession} session's instance to signal that handshake
	 * failed.
	 *
	 * @param nextFilter next filter in the filter chain
	 * @param session the MINA session
	 * @param cause the original exception
	 */
	virtual void exceptionCaught(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, sp<EThrowableType>& cause) THROWS(EException);

	/**
	 * Receives data from the remote host, passes to the handler if a handshake is in progress,
	 * otherwise passes on transparently.
	 *
	 * @param nextFilter the next filter in filter chain
	 * @param session the session object
	 * @param message the object holding the received data
	 */
	virtual void messageReceived(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, sp<EObject>& message) THROWS(EException);

	/**
	 * Filter handshake related messages from reaching the messageSent callbacks of
	 * downstream filters.
	 *
	 * @param nextFilter the next filter in filter chain
	 * @param session the session object
	 * @param writeRequest the data written
	 */
	virtual void messageSent(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) THROWS(EException);

	/**
	 * Filters outgoing writes, queueing them up if necessary while a handshake
	 * is ongoing.
	 *
	 * @param nextFilter the next filter in filter chain
	 * @param session the session object
	 * @param writeRequest the data to write
	 */
	virtual void filterWrite(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) THROWS(EException);

	/**
	 * Actually write data. Queues the data up unless it relates to the handshake or the
	 * handshake is done.
	 *
	 * @param nextFilter the next filter in filter chain
	 * @param session the session object
	 * @param writeRequest the data to write
	 * @param isHandshakeData true if writeRequest is written by the proxy classes.
	 */
	void writeData(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, sp<EWriteRequest>& writeRequest, boolean isHandshakeData);

private:
	static sp<ELogger> LOGGER;// = LoggerFactory.getLogger(ProxyFilter.class);

	/**
	 * Get the {@link ProxyLogicHandler} for a given session.
	 *
	 * @param session the session object
	 * @return the handler which will handle handshaking with the proxy
	 */
	 EProxyLogicHandler* getProxyHandler(sp<EIoSession>& session);
};

} /* namespace eio */
} /* namespace efc */
#endif /* EPROXYFILTER_HH_ */
