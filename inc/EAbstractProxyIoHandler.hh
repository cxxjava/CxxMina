/*
 * EAbstractProxyIoHandler.hh
 *
 *  Created on: 2016-7-6
 *      Author: cxxjava@163.com
 */

#ifndef EABSTRACTPROXYIOHANDLER_HH_
#define EABSTRACTPROXYIOHANDLER_HH_

#include "./EIoHandlerAdapter.hh"
#include "ELog.hh"

namespace efc {
namespace eio {

/**
 * AbstractProxyIoHandler.java - {@link IoHandler} that intercepts events until handshake is complete.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since MINA 2.0.0-M3
 */

class EAbstractProxyIoHandler: public EIoHandlerAdapter {
public:
	/**
	 * Method called only when handshake has completed.
	 *
	 * @param session the io session
	 * @throws Exception If the proxy session can't be opened
	 */
	virtual void proxySessionOpened(sp<EIoSession>& session) THROWS(EException) = 0;

	/**
	 * Hooked session opened event.
	 *
	 * @param session the io session
	 */
	virtual void sessionOpened(sp<EIoSession>& session) THROWS(EException);

private:
	static sp<ELogger> logger;// = LoggerFactory.getLogger(AbstractProxyIoHandler.class);
};

} /* namespace eio */
} /* namespace efc */
#endif /* EABSTRACTPROXYIOHANDLER_HH_ */
