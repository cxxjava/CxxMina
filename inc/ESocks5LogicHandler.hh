/*
 * ESocks5LogicHandler.hh
 *
 *  Created on: 2016-7-6
 *      Author: cxxjava@163.com
 */

#ifndef ESOCKS5LOGICHANDLER_HH_
#define ESOCKS5LOGICHANDLER_HH_

#include "./EAbstractSocksLogicHandler.hh"
#include "./ESocksProxyRequest.hh"
#include "ELog.hh"

namespace efc {
namespace eio {

/**
 * Socks5LogicHandler.java - SOCKS5 authentication mechanisms logic handler.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since MINA 2.0.0-M3
 */

class ESocks5LogicHandler: public EAbstractSocksLogicHandler {
public:
	virtual ~ESocks5LogicHandler();

	/**
	 * @see AbstractSocksLogicHandler#AbstractSocksLogicHandler(ProxyIoSession)
	 *
	 * @param proxyIoSession The original session
	 */
	ESocks5LogicHandler(sp<EProxyIoSession>& proxyIoSession);

	/**
	 * Performs the handshake process.
	 *
	 * @param nextFilter the next filter
	 */
	virtual synchronized void doHandshake(EIoFilter::NextFilter* nextFilter);

	/**
	 * Handles incoming data during the handshake process. Should consume only the
	 * handshake data from the buffer, leaving any extra data in place.
	 *
	 * @param nextFilter the next filter
	 * @param buf the buffered data received
	 */
	virtual synchronized void messageReceived(EIoFilter::NextFilter* nextFilter, sp<EIoBuffer>& buf);

protected:
	/**
	 * Handle a SOCKS v5 response from the proxy server.
	 *
	 * @param nextFilter the next filter
	 * @param buf the buffered data received
	 * @param step the current step in the authentication process
	 * @throws Exception If something went wrong
	 */
	void handleResponse(EIoFilter::NextFilter* nextFilter, sp<EIoBuffer>& buf, int step) THROWS(EException);

	/**
	 * Closes the session. If any {@link GSSContext} is present in the session
	 * then it is closed.
	 *
	 * @param message the error message
	 */
	virtual void closeSession(const char* message);

private:
	static sp<ELogger> LOGGER;// = LoggerFactory.getLogger(Socks5LogicHandler.class);

	/**
	 * The selected authentication method attribute key.
	 */
	static sp<EString> SELECTED_AUTH_METHOD;// = Socks5LogicHandler.class.getName() + ".SelectedAuthMethod";

	/**
	 * The current step in the handshake attribute key.
	 */
	static sp<EString> HANDSHAKE_STEP;// = Socks5LogicHandler.class.getName() + ".HandshakeStep";

	/**
	 * The Java GSS-API context attribute key.
	 */
	static sp<EString> GSS_CONTEXT;// = Socks5LogicHandler.class.getName() + ".GSSContext";

	/**
	 * Last GSS token received attribute key.
	 */
	static sp<EString> GSS_TOKEN;// = Socks5LogicHandler.class.getName() + ".GSSToken";

	/**
	 * Encodes the initial greeting packet.
	 *
	 * @param request the socks proxy request data
	 * @return the encoded buffer
	 */
	sp<EIoBuffer> encodeInitialGreetingPacket(sp<ESocksProxyRequest>& request);

	/**
	 * Encodes the proxy authorization request packet.
	 *
	 * @param request the socks proxy request data
	 * @return the encoded buffer
	 * @throws UnsupportedEncodingException if request's hostname charset
	 * can't be converted to ASCII.
	 */
	sp<EIoBuffer> encodeProxyRequestPacket(sp<ESocksProxyRequest>& request) THROWS(EUnsupportedEncodingException);

	/**
	 * Encodes the authentication packet for supported authentication methods.
	 *
	 * @param request the socks proxy request data
	 * @return the encoded buffer, if null then authentication step is over
	 * and handshake process can jump immediately to the next step without waiting
	 * for a server reply.
	 * @throws UnsupportedEncodingException if some string charset convertion fails
	 * @throws GSSException when something fails while using GSSAPI
	 */
	sp<EIoBuffer> encodeAuthenticationPacket(sp<ESocksProxyRequest>& request) THROWS2(EUnsupportedEncodingException,
			EGSSException);

	/**
	 * Encodes the authentication packet for supported authentication methods.
	 *
	 * @param request the socks proxy request data
	 * @return the encoded buffer
	 * @throws GSSException when something fails while using GSSAPI
	 */
	sp<EIoBuffer> encodeGSSAPIAuthenticationPacket(sp<ESocksProxyRequest>& request) THROWS(EGSSException);

	/**
	 * Encodes a SOCKS5 request and writes it to the next filter
	 * so it can be sent to the proxy server.
	 *
	 * @param nextFilter the next filter
	 * @param request the request to send.
	 * @param step the current step in the handshake process
	 */
	void writeRequest(EIoFilter::NextFilter* nextFilter, sp<ESocksProxyRequest>& request, int step);
};

} /* namespace eio */
} /* namespace efc */
#endif /* ESOCKS5LOGICHANDLER_HH_ */
