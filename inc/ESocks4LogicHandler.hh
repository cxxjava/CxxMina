/*
 * ESocks4LogicHandler.hh
 *
 *  Created on: 2016-7-6
 *      Author: cxxjava@163.com
 */

#ifndef ESOCKS4LOGICHANDLER_HH_
#define ESOCKS4LOGICHANDLER_HH_

#include "./EAbstractSocksLogicHandler.hh"
#include "ELog.hh"

namespace efc {
namespace eio {

/**
 * Socks4LogicHandler.java - SOCKS4/SOCKS4a authentication mechanisms logic handler.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since MINA 2.0.0-M3
 */

class ESocks4LogicHandler: public EAbstractSocksLogicHandler {
public:
	/**
	 * @see AbstractSocksLogicHandler#AbstractSocksLogicHandler(ProxyIoSession)
	 *
	 * @param proxyIoSession The original session
	 */
	ESocks4LogicHandler(sp<EProxyIoSession>& proxyIoSession);

	/**
	 * Perform the handshake.
	 *
	 * @param nextFilter the next filter
	 */
	virtual void doHandshake(EIoFilter::NextFilter* nextFilter);

	/**
	 * Handle incoming data during the handshake process. Should consume only the
	 * handshake data from the buffer, leaving any extra data in place.
	 *
	 * @param nextFilter the next filter
	 * @param buf the server response data buffer
	 */
	virtual void messageReceived(EIoFilter::NextFilter* nextFilter, sp<EIoBuffer>& buf);

protected:
	/**
	 * Encode a SOCKS4/SOCKS4a request and writes it to the next filter
	 * so it can be sent to the proxy server.
	 *
	 * @param nextFilter the next filter
	 * @param request the request to send.
	 */
	virtual void writeRequest(EIoFilter::NextFilter* nextFilter, sp<ESocksProxyRequest>& request);

	/**
	 * Handle a SOCKS4/SOCKS4a response from the proxy server. Test
	 * the response buffer reply code and call {@link #setHandshakeComplete()}
	 * if access is granted.
	 *
	 * @param buf the buffer holding the server response data.
	 * @throws Exception if server response is malformed or if request is rejected
	 * by the proxy server.
	 */
	virtual void handleResponse(sp<EIoBuffer>& buf) THROWS(EException);

private:
	static sp<ELogger> logger;// = LoggerFactory.getLogger(Socks4LogicHandler.class);
};

} /* namespace eio */
} /* namespace efc */
#endif /* ESOCKS4LOGICHANDLER_HH_ */
