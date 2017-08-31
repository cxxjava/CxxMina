/*
 * EIoFutureListener.hh
 *
 *  Created on: 2013-8-23
 *      Author: cxxjava@163.com
 */

#ifndef EIOFUTURELISTENER_HH_
#define EIOFUTURELISTENER_HH_

#include "./EIdleStatus.hh"
#include "./EIoSession.hh"
#include "./EIoFuture.hh"
#include "Efc.hh"

namespace efc {
namespace eio {

/**
 * Listens to events related to an {@link IoService}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */
abstract class EIoFutureListener : virtual public EEventListener {
public:
	DECLARE_STATIC_INITZZ;

public:
	/**
	 * An {@link IoFutureListener} that closes the {@link IoSession} which is
	 * associated with the specified {@link IoFuture}.
	 */
	static EIoFutureListener* CLOSE;// = new CloseListener();

	/**
	 * Invoked when the operation associated with the {@link IoFuture}
	 * has been completed even if you add the listener after the completion.
	 *
	 * @param future  The source {@link IoFuture} which called this
	 *                callback.
	 */
	virtual void operationComplete(EIoFuture* future) = 0;
};

class CloseListener : virtual public EIoFutureListener {
public:
	void operationComplete(EIoFuture* future) {
		future->getSession()->closeNow();
	}
};

} /* namespace eio */
} /* namespace efc */

#endif /* EIOFUTURELISTENER_HH_ */
