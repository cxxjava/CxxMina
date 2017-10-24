/*
 * EIoEventQueueHandler.hh
 *
 *  Created on: 2016-9-22
 *      Author: cxxjava@163.com
 */

#ifndef EIOEVENTQUEUEHANDLER_HH_
#define EIOEVENTQUEUEHANDLER_HH_

#include "./EIoEvent.hh"

namespace efc {
namespace eio {

/**
 * Listens and filters all event queue operations occurring in
 * {@link OrderedThreadPoolExecutor} and {@link UnorderedThreadPoolExecutor}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

interface EIoEventQueueHandler: virtual public  EEventListener {
	virtual ~EIoEventQueueHandler() {
	}

	/**
	 * @return <tt>true</tt> if and only if the specified <tt>event</tt> is
	 * allowed to be offered to the event queue.  The <tt>event</tt> is dropped
	 * if <tt>false</tt> is returned.
	 *
	 * @param source The source of event
	 * @param event The received event
	 */
	virtual boolean accept(EObject* source, EIoEvent* event) = 0;

	/**
	 * Invoked after the specified <tt>event</tt> has been offered to the
	 * event queue.
	 *
	 * @param source The source of event
	 * @param event The received event
	 */
	virtual void offered(EObject* source, EIoEvent* event) = 0;

	/**
	 * Invoked after the specified <tt>event</tt> has been polled from the
	 * event queue.
	 *
	 * @param source The source of event
	 * @param event The received event
	 */
	virtual void polled(EObject* source, EIoEvent* event) = 0;
};

/**
 * A dummy handler which always accepts event doing nothing particular.
 */
class DummyIoEventQueueHandler: virtual public EIoEventQueueHandler {
public:
	virtual boolean accept(EObject* source, EIoEvent* event) {
		return true;
	}

	virtual void offered(EObject* source, EIoEvent* event) {
		// NOOP
	}

	virtual void polled(EObject* source, EIoEvent* event) {
		// NOOP
	}
};

} /* namespace eio */
} /* namespace efc */
#endif /* EIOEVENTQUEUEHANDLER_HH_ */
