/*
 * EChainedIoHandler.hh
 *
 *  Created on: 2016-5-31
 *      Author: cxxjava@163.com
 */

#ifndef ECHAINEDIOHANDLER_HH_
#define ECHAINEDIOHANDLER_HH_

#include "./EIoHandlerAdapter.hh"
#include "./EIoHandlerChain.hh"

namespace efc {
namespace eio {

/**
 * An {@link IoHandler} which executes an {@link IoHandlerChain}
 * on a <tt>messageReceived</tt> event.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EChainedIoHandler: public EIoHandlerAdapter {
public:
	virtual ~EChainedIoHandler();

	/**
	 * Creates a new instance which contains an empty {@link IoHandlerChain}.
	 */
	EChainedIoHandler();

	/**
	 * @return the {@link IoHandlerCommand} this handler will use to
	 * handle <tt>messageReceived</tt> events.
	 */
	EIoHandlerChain* getChain();

	/**
	 * Handles the specified <tt>messageReceived</tt> event with the
	 * {@link IoHandlerCommand} or {@link IoHandlerChain} you specified
	 * in the constructor.
	 */
	virtual void messageReceived(sp<EIoSession>& session, sp<EObject>& message) THROWS(EException);

private:
	EIoHandlerChain* chain;
};

} /* namespace eio */
} /* namespace efc */
#endif /* ECHAINEDIOHANDLER_HH_ */
