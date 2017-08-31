/*
 * ESocksProxyRequest.hh
 *
 *  Created on: 2016-7-6
 *      Author: cxxjava@lotuseed.com
 */

#ifndef ESOCKSPROXYREQUEST_HH_
#define ESOCKSPROXYREQUEST_HH_

#include "./EProxyRequest.hh"

namespace efc {
namespace eio {

/**
 * SocksProxyRequest.java - Wrapper class for SOCKS requests.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since MINA 2.0.0-M3
 */

class ESocksProxyRequest: public EProxyRequest, public ESynchronizeable {
public:
	virtual ~ESocksProxyRequest();

	/**
	 * Constructor used when building a SOCKS4 request.
	 *
	 * @param protocolVersion the protocol version
	 * @param commandCode the command code
	 * @param endpointAddress the endpoint address
	 * @param userName the user name
	 */
	ESocksProxyRequest(byte protocolVersion, byte commandCode, EInetSocketAddress* endpointAddress, const char* userName);

	/**
	 * Constructor used when building a SOCKS4a request.
	 *
	 * @param commandCode the command code
	 * @param host the server host name
	 * @param port the server port
	 * @param userName the user name
	 */
	ESocksProxyRequest(byte commandCode, const char* host, int port, const char* userName);

	/**
	 * @return the endpoint address resulting from the {@link #getEndpointAddress()}.
	 * If not set, it will return the {@link SocksProxyConstants#FAKE_IP} constant
	 * value which will be ignored in a SOCKS v4 request.
	 */
	int getIpAddress();

	/**
	 * Return the server port as a byte array.
	 *
	 * @return the server port
	 */
	int getPort(); //@see: byte[] getPort();

	/**
	 * Return the command code.
	 *
	 * @return the command code
	 */
	byte getCommandCode();

	/**
	 * Return the protocol version.
	 *
	 * @return the protocol version
	 */
	byte getProtocolVersion();

	/**
	 * Return the user name.
	 *
	 * @return the user name
	 */
	EString getUserName();

	/**
	 * Return the server host name.
	 *
	 * @return the server host name
	 */
	synchronized EString getHost();

	/**
	 * Return the user password.
	 *
	 * @return the user password
	 */
	EString getPassword();

	/**
	 * Set the user password
	 *
	 * @param password the user password value
	 */
	void setPassword(const char* password);

	/**
	 * Return the Kerberos service name.
	 *
	 * @return the Kerberos service name
	 */
	EString getServiceKerberosName();

	/**
	 * Set the Kerberos service name.
	 *
	 * @param serviceKerberosName the Kerberos service name
	 */
	void setServiceKerberosName(const char* serviceKerberosName);

private:
	/**
	 * The SOCKS protocol version.
	 */
	byte protocolVersion;

	/**
	 * The command code.
	 */
	byte commandCode;

	/**
	 * The user name used when authenticating to the proxy server.
	 */
	EString userName;

	/**
	 * The user's password used when authenticating to the proxy server.
	 */
	EString password;

	/**
	 * The SOCKS server host name.
	 */
	EString host;

	/**
	 * The SOCKS server port.
	 */
	int port;

	/**
	 * The Kerberos service name used in GSSAPI authentication mode.
	 */
	EString serviceKerberosName;
};

} /* namespace eio */
} /* namespace efc */
#endif /* ESOCKSPROXYREQUEST_HH_ */
