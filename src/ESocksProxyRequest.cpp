/*
 * ESocksProxyRequest.cpp
 *
 *  Created on: 2016-7-6
 *      Author: cxxjava@lotuseed.com
 */

#include "../inc/ESocksProxyRequest.hh"
#include "../inc/ESocksProxyConstants.hh"

namespace efc {
namespace eio {

ESocksProxyRequest::~ESocksProxyRequest() {
	//
}

ESocksProxyRequest::ESocksProxyRequest(byte protocolVersion, byte commandCode,
		EInetSocketAddress* endpointAddress, const char* userName) :
		EProxyRequest(endpointAddress) {
	this->protocolVersion = protocolVersion;
	this->commandCode = commandCode;
	this->userName = userName;
}

ESocksProxyRequest::ESocksProxyRequest(byte commandCode, const char* host,
		int port, const char* userName) {
	this->protocolVersion = ESocksProxyConstants::SOCKS_VERSION_4;
	this->commandCode = commandCode;
	this->userName = userName;
	this->host = host;
	this->port = port;
}

int ESocksProxyRequest::getIpAddress() {
	if (getEndpointAddress() == null) {
		return EInetAddress::getByName(FAKE_IP).getAddress();
	}

	return getEndpointAddress()->getAddress()->getAddress();
}

int ESocksProxyRequest::getPort() {
	/* @see:
	byte[] port = new byte[2];
	int p = (getEndpointAddress() == null ? this.port : getEndpointAddress().getPort());
	port[1] = (byte) p;
	port[0] = (byte) (p >> 8);
	return port;
	 */
	return (getEndpointAddress() == null ? this->port : getEndpointAddress()->getPort());
}

byte ESocksProxyRequest::getCommandCode() {
	return commandCode;
}

byte ESocksProxyRequest::getProtocolVersion() {
	return protocolVersion;
}

EString ESocksProxyRequest::getUserName() {
	return userName;
}

EString ESocksProxyRequest::getHost() {
	SYNCHRONIZED(this) {
		if (host.isEmpty()) {
			EInetSocketAddress* adr = getEndpointAddress();

			if (adr != null && !adr->isUnresolved()) {
				host = adr->getHostName();
			}
		}

		return host;
    }}
}

EString ESocksProxyRequest::getPassword() {
	return password;
}

void ESocksProxyRequest::setPassword(const char* password) {
	this->password = password;
}

EString ESocksProxyRequest::getServiceKerberosName() {
	return serviceKerberosName;
}

void ESocksProxyRequest::setServiceKerberosName(
		const char* serviceKerberosName) {
	this->serviceKerberosName = serviceKerberosName;
}

} /* namespace eio */
} /* namespace efc */
