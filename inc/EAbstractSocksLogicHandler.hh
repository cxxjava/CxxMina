/*
 * EAbstractSocksLogicHandler.hh
 *
 *  Created on: 2016-7-6
 *      Author: cxxjava@lotuseed.com
 */

#ifndef EABSTRACTSOCKSLOGICHANDLER_HH_
#define EABSTRACTSOCKSLOGICHANDLER_HH_

#include "./EAbstractProxyLogicHandler.hh"
#include "./ESocksProxyRequest.hh"

namespace efc {
namespace eio {

/**
 * AbstractSocksLogicHandler.java - Base class for SOCKS {@link AbstractProxyLogicHandler}
 * implementations.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since MINA 2.0.0-M3
 */

class EAbstractSocksLogicHandler: public EAbstractProxyLogicHandler {
public:
	/**
	 * Creates a new {@link AbstractSocksLogicHandler}.
	 *
	 * @param proxyIoSession the proxy session object
	 */
	EAbstractSocksLogicHandler(sp<EProxyIoSession>& proxyIoSession);

protected:
	/**
	 * The request sent to the proxy.
	 */
	sp<ESocksProxyRequest> request;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EABSTRACTSOCKSLOGICHANDLER_HH_ */
