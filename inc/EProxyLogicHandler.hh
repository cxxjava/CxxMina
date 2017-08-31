/*
 * EProxyLogicHandler.hh
 *
 *  Created on: 2016-6-15
 *      Author: cxxjava@lotuseed.com
 */

#ifndef EPROXYLOGICHANDLER_HH_
#define EPROXYLOGICHANDLER_HH_

#include "./EIoBuffer.hh"
#include "./EIoFilter.hh"
#include "./EProxyAuthException.hh"

namespace efc {
namespace eio {

/**
 * ProxyLogicHandler.java - Interface implemented by classes containing proxy type specific logic.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since MINA 2.0.0-M3
 */

class EProxyIoSession;

interface EProxyLogicHandler: public ESynchronizeable {
	virtual ~EProxyLogicHandler(){}

	/**
	 * Tests if handshake process is complete.
	 *
	 * @return <tt>true</tt> if handshaking is complete and
	 * data can be sent through the proxy, false otherwise.
	 */
	virtual boolean isHandshakeComplete() = 0;

	/**
	 * Handle incoming data during the handshake process. Should consume only the
	 * handshake data from the buffer, leaving any extra data in place.
	 *
	 * @param nextFilter the next filter in the filter chain
	 * @param buf the buffer holding the received data
	 * @throws ProxyAuthException if authentication fails
	 */
	virtual void messageReceived(EIoFilter::NextFilter* nextFilter, sp<EIoBuffer>& buf) THROWS(EProxyAuthException) = 0;

	/**
	 * Called at each step of the handshake procedure.
	 *
	 * @param nextFilter the next filter in filter chain
	 * @throws ProxyAuthException if authentication fails
	 */
	virtual void doHandshake(EIoFilter::NextFilter* nextFilter) THROWS(EProxyAuthException) = 0;

	/**
	 * @return the {@link ProxyIoSession}.
	 */
	virtual sp<EProxyIoSession> getProxyIoSession() = 0;

	/**
	 * Enqueue a message to be written once handshaking is complete.
	 *
	 * @param nextFilter the next filter in filter chain
	 * @param writeRequest the data to be written
	 */
	virtual void enqueueWriteRequest(EIoFilter::NextFilter* nextFilter, sp<EWriteRequest>& writeRequest) = 0;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EPROXYLOGICHANDLER_HH_ */
