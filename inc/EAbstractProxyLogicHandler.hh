/*
 * EAbstractProxyLogicHandler.hh
 *
 *  Created on: 2016-7-6
 *      Author: cxxjava@lotuseed.com
 */

#ifndef EABSTRACTPROXYLOGICHANDLER_HH_
#define EABSTRACTPROXYLOGICHANDLER_HH_

#include "./EProxyLogicHandler.hh"
#include "./EProxyIoSession.hh"
#include "ELog.hh"

namespace efc {
namespace eio {

/**
 * AbstractProxyLogicHandler.java - Helper class to handle proxy handshaking logic. Derived classes
 * implement proxy type specific logic.
 * <p>
 * Based upon SSLHandler from mina-filter-ssl.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since MINA 2.0.0-M3
 */

class EAbstractProxyLogicHandler: virtual public EProxyLogicHandler {
private:
	/**
	 * Event wrapper class for enqueued events.
	 */
	class Event : public EObject {
	public:
		EIoFilter::NextFilter* nextFilter;
		sp<EObject> data;

		Event(EIoFilter::NextFilter* nextFilter, sp<EObject>& data) {
			this->nextFilter = nextFilter;
			this->data = data;
		}
	};

public:
	virtual ~EAbstractProxyLogicHandler();

	/**
	 * Creates a new {@link AbstractProxyLogicHandler}.
	 *
	 * @param proxyIoSession {@link ProxyIoSession} in use.
	 */
	EAbstractProxyLogicHandler(sp<EProxyIoSession>& proxyIoSession);

	/**
	 * @return the {@link ProxyIoSession} object.
	 */
	virtual sp<EProxyIoSession> getProxyIoSession();

	/**
	 * @return <tt>true</tt> if handshaking is complete and
	 * data can be sent through the proxy.
	 */
	virtual boolean isHandshakeComplete();

	/**
	 * Enqueue a message to be written once handshaking is complete.
	 */
	virtual synchronized void enqueueWriteRequest(EIoFilter::NextFilter* nextFilter, sp<EWriteRequest>& writeRequest);

protected:
	/**
	 * @return the proxy filter {@link ProxyFilter}.
	 */
	EProxyFilter* getProxyFilter();

	/**
	 * @return the session.
	 */
	sp<EIoSession> getSession();

	/**
	 * Writes data to the proxy server.
	 *
	 * @param nextFilter the next filter
	 * @param data Data buffer to be written.
	 * @return A Future for the write operation
	 */
	sp<EWriteFuture> writeData(EIoFilter::NextFilter* nextFilter, sp<EIoBuffer>& data);

	/**
	 * Signals that the handshake has finished.
	 */
	void setHandshakeComplete();

	/**
	 * Send any write requests which were queued whilst waiting for handshaking to complete.
	 *
	 * @throws Exception If we can't flush the pending write requests
	 */
	synchronized void flushPendingWriteRequests() THROWS(EException);

	/**
	 * Closes the session.
	 *
	 * @param message the error message
	 * @param t the exception which caused the session closing
	 */
	virtual void closeSession(const char* message, EThrowable& t);

	/**
	 * Closes the session.
	 *
	 * @param message the error message
	 */
	virtual void closeSession(const char* message);

private:
	static sp<ELogger> LOGGER;// = LoggerFactory.getLogger(AbstractProxyLogicHandler.class);

    /**
     * Object that contains all the proxy authentication session informations.
     */
    sp<EProxyIoSession> proxyIoSession;

    /**
     * Queue of write events which occurred before the proxy handshake had completed.
     */
    ELinkedList<Event*>* writeRequestQueue;// = null;

    /**
     * Has the handshake been completed.
     */
    boolean handshakeComplete;// = false;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EABSTRACTPROXYLOGICHANDLER_HH_ */
