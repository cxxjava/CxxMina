/*
 * EIoHandlerAdapter.hh
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#ifndef EIOHANDLERADAPTER_HH_
#define EIOHANDLERADAPTER_HH_

#include "./EIoHandler.hh"
#include "ELog.hh"

namespace efc {
namespace eio {

/**
 * An adapter class for {@link IoHandler}.  You can extend this
 * class and selectively override required event handler methods only.  All
 * methods do nothing by default.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */
class EIoHandlerAdapter : virtual public EIoHandler {
public:
	virtual void sessionCreated(sp<EIoSession>& session) THROWS(EException);

	virtual void sessionOpened(sp<EIoSession>& session) THROWS(EException);

	virtual void sessionClosed(sp<EIoSession>& session) THROWS(EException);

	virtual void sessionIdle(sp<EIoSession>& session, EIdleStatus status) THROWS(EException);

	virtual void exceptionCaught(sp<EIoSession>& session, sp<EThrowableType>& cause) THROWS(EException);

	virtual void messageReceived(sp<EIoSession>& session, sp<EObject>& message) THROWS(EException);

	virtual void messageSent(sp<EIoSession>& session, sp<EObject>& message) THROWS(EException);

	virtual void inputClosed(sp<EIoSession>& session) THROWS(EException);

private:
	static sp<ELogger> LOGGER;// = LoggerFactory.getLogger(IoHandlerAdapter.class);
};

} /* namespace eio */
} /* namespace efc */

#endif /* EIOHANDLERADAPTER_HH_ */
