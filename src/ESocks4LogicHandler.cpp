/*
 * ESocks4LogicHandler.cpp
 *
 *  Created on: 2016-7-6
 *      Author: cxxjava@163.com
 */

#include "../inc/ESocks4LogicHandler.hh"
#include "../inc/ESocksProxyConstants.hh"

namespace efc {
namespace eio {

sp<ELogger> ESocks4LogicHandler::logger = ELoggerManager::getLogger("ESocks4LogicHandler");

ESocks4LogicHandler::ESocks4LogicHandler(sp<EProxyIoSession>& proxyIoSession) :
		EAbstractSocksLogicHandler(proxyIoSession) {
}

void ESocks4LogicHandler::doHandshake(EIoFilter::NextFilter* nextFilter) {
	logger->debug(__FILE__, __LINE__, " doHandshake()");

	// Send request
	writeRequest(nextFilter, request);
}

void ESocks4LogicHandler::messageReceived(EIoFilter::NextFilter* nextFilter,
		sp<EIoBuffer>& buf) {
	try {
		if (buf->remaining() >= ESocksProxyConstants::SOCKS_4_RESPONSE_SIZE) {
			handleResponse(buf);
		}
	} catch (EException& ex) {
		closeSession("Proxy handshake failed: ", ex);
	}
}

void ESocks4LogicHandler::writeRequest(EIoFilter::NextFilter* nextFilter,
		sp<ESocksProxyRequest>& request) {
	try {
		//@see: boolean isV4ARequest = Arrays.equals(request.getIpAddress(), SocksProxyConstants.FAKE_IP);
		boolean isV4ARequest = (request->getIpAddress() == EInetAddress::getByName(FAKE_IP).getAddress());
		EString userID = request->getUserName();
		EString host = isV4ARequest ? request->getHost() : null;

		int len = 9 + userID.length();

		if (isV4ARequest) {
			len += host.length() + 1;
		}

		sp<EIoBuffer> buf = EIoBuffer::allocate(len);

		buf->put(request->getProtocolVersion());
		buf->put(request->getCommandCode());
		buf->putShort(request->getPort());
		buf->putInt(request->getIpAddress());
		buf->putString(userID.c_str());
		buf->put(ESocksProxyConstants::TERMINATOR);

		if (isV4ARequest) {
			buf->putString(host.c_str());
			buf->put(ESocksProxyConstants::TERMINATOR);
		}

		if (isV4ARequest) {
			logger->debug(__FILE__, __LINE__, "  sending SOCKS4a request");
		} else {
			logger->debug(__FILE__, __LINE__, "  sending SOCKS4 request");
		}

		buf->flip();
		writeData(nextFilter, buf);
	} catch (EException& ex) {
		closeSession("Unable to send Socks request: ", ex);
	}
}

void ESocks4LogicHandler::handleResponse(sp<EIoBuffer>& buf) {
	byte first = buf->get(0);

	if (first != 0) {
		throw EException(__FILE__, __LINE__, "Socks response seems to be malformed");
	}

	byte status = buf->get(1);

	// Consumes all the response data from the buffer
	buf->position(buf->position() + ESocksProxyConstants::SOCKS_4_RESPONSE_SIZE);

	if (status == ESocksProxyConstants::V4_REPLY_REQUEST_GRANTED) {
		setHandshakeComplete();
	} else {
		throw EException(__FILE__, __LINE__, 
				EString::formatOf("Proxy handshake failed - Code: 0x%x (%s)",
						status,
						ESocksProxyConstants::getReplyCodeAsString(status)).c_str());
	}
}

} /* namespace eio */
} /* namespace efc */
