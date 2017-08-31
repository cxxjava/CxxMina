/*
 * EProxyIoSessionInitializer.hh
 *
 *  Created on: 2016-7-5
 *      Author: cxxjava@163.com
 */

#ifndef EPROXYIOSESSIONINITIALIZER_HH_
#define EPROXYIOSESSIONINITIALIZER_HH_

#include "./EIoSessionInitializer.hh"
#include "./EProxyIoSession.hh"

namespace efc {
namespace eio {

/**
 * ProxyIoSessionInitializer.java - {@link IoSessionInitializer} wrapper class to inject the
 * {@link ProxyIoSession} object that contains all the attributes of the target connection
 * into the {@link IoSession}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since MINA 2.0.0-M3
 */

class EProxyIoSessionInitializer: virtual public EIoSessionInitializer {
public:
	EProxyIoSessionInitializer(EIoSessionInitializer* wrappedSessionInitializer,
	            sp<EProxyIoSession>& proxyIoSession);

	sp<EProxyIoSession> getProxySession();

	virtual void initializeSession(sp<EIoSession> session, sp<EIoFuture> future);

private:
	EIoSessionInitializer* wrappedSessionInitializer;
	sp<EProxyIoSession> proxyIoSession;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EPROXYIOSESSIONINITIALIZER_HH_ */
