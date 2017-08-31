/*
 * EWriteRequestQueue.hh
 *
 *  Created on: 2015-12-31
 *      Author: cxxjava@163.com
 */

#ifndef EWRITEREQUESTQUEUE_HH_
#define EWRITEREQUESTQUEUE_HH_

#include "./EIoSession.hh"

namespace efc {
namespace eio {

class EWriteRequest;
    
/**
 * Stores {@link WriteRequest}s which are queued to an {@link IoSession}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

interface EWriteRequestQueue : virtual public EObject {
    virtual ~EWriteRequestQueue() {
    }

	/**
	 * Get the first request available in the queue for a session.
	 * @param session The session
	 * @return The first available request, if any.
	 */
	virtual sp<EWriteRequest> poll(sp<EIoSession> session) = 0;

	/**
	 * Add a new WriteRequest to the session write's queue
	 * @param session The session
	 * @param writeRequest The writeRequest to add
	 */
	virtual void offer(sp<EIoSession> session, EWriteRequest* writeRequest) = 0;
	virtual void offer(sp<EIoSession> session, sp<EWriteRequest> writeRequest) = 0;

	/**
	 * Tells if the WriteRequest queue is empty or not for a session
	 * @param session The session to check
	 * @return <code>true</code> if the writeRequest is empty
	 */
	virtual boolean isEmpty(sp<EIoSession> session) = 0;

	/**
	 * Removes all the requests from this session's queue.
	 * @param session The associated session
	 */
	virtual void clear(sp<EIoSession> session) = 0;

	/**
	 * Disposes any releases associated with the specified session.
	 * This method is invoked on disconnection.
	 * @param session The associated session
	 */
	virtual void dispose(sp<EIoSession> session) = 0;

	/**
	 * @return the number of objects currently stored in the queue.
	 */
	virtual int size() = 0;
};

} /* namespace eio */
} /* namespace efc */

#endif /* EWRITEREQUESTQUEUE_HH_ */
