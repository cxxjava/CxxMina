/*
 * EIoSessionEventQueue.hh
 *
 *  Created on: 2016-6-16
 *      Author: cxxjava@lotuseed.com
 */

#ifndef EIOSESSIONEVENTQUEUE_HH_
#define EIOSESSIONEVENTQUEUE_HH_

#include "./EIoSessionEvent.hh"
#include "ELog.hh"

namespace efc {
namespace eio {

/**
 * IoSessionEventQueue.java - Queue that contains filtered session events
 * while handshake isn't done.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since MINA 2.0.0-M3
 */

class EProxyIoSession;

class EIoSessionEventQueue: EObject {
public:
	EIoSessionEventQueue(EProxyIoSession* proxyIoSession);

	/**
	 * Event is enqueued only if necessary :
	 * - socks proxies do not need the reconnection feature so events are always
	 * forwarded for these.
	 * - http proxies events will be enqueued while handshake has not been completed
	 * or until connection was closed.
	 * If connection was prematurely closed previous events are discarded and only the
	 * session closed is delivered.
	 *
	 * @param evt the event to enqueue
	 */
	void enqueueEventIfNecessary(EIoSessionEvent* evt);

	/**
	 * Send any session event which were queued while waiting for handshaking to complete.
	 *
	 * Please note this is an internal method. DO NOT USE it in your code.
	 *
	 * @throws Exception If something went wrong while flushing the pending events
	 */
	void flushPendingSessionEvents() THROWS(EException);

private:
	static sp<ELogger> logger;// = LoggerFactory.getLogger(IoSessionEventQueue.class);

	/**
	 * The proxy session object.
	 */
	EProxyIoSession* proxyIoSession;

	/**
	 * Queue of session events which occurred before the proxy handshake had completed.
	 */
	ELinkedList<EIoSessionEvent*> sessionEventsQueue;// = new LinkedList<IoSessionEvent>();
	EReentrantLock queueLock;

	/**
	 * Discard all events from the queue.
	 */
	void discardSessionQueueEvents();

	/**
	 * Enqueue an event to be delivered once handshaking is complete.
	 *
	 * @param evt the session event to enqueue
	 */
	void enqueueSessionEvent(EIoSessionEvent* evt);
};

} /* namespace eio */
} /* namespace efc */
#endif /* EIOSESSIONEVENTQUEUE_HH_ */
