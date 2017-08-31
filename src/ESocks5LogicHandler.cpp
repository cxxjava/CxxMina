/*
 * ESocks5LogicHandler.cpp
 *
 *  Created on: 2016-7-6
 *      Author: cxxjava@163.com
 */

#include "../inc/ESocks5LogicHandler.hh"

namespace efc {
namespace eio {

sp<ELogger> ESocks5LogicHandler::LOGGER = ELoggerManager::getLogger("ESocks5LogicHandler");
sp<EString> ESocks5LogicHandler::SELECTED_AUTH_METHOD = new EString("ESocks5LogicHandler.SelectedAuthMethod");
sp<EString> ESocks5LogicHandler::HANDSHAKE_STEP = new EString("ESocks5LogicHandler.HandshakeStep");
sp<EString> ESocks5LogicHandler::GSS_CONTEXT = new EString("ESocks5LogicHandler.GSSContext");
sp<EString> ESocks5LogicHandler::GSS_TOKEN = new EString("ESocks5LogicHandler.GSSToken");

ESocks5LogicHandler::~ESocks5LogicHandler() {
	//
}

    ESocks5LogicHandler::ESocks5LogicHandler(sp<EProxyIoSession>& proxyIoSession) : EAbstractSocksLogicHandler(proxyIoSession) {
	throw ETODOEXCEPTION;
}

void ESocks5LogicHandler::doHandshake(EIoFilter::NextFilter* nextFilter) {
	throw ETODOEXCEPTION;
}

void ESocks5LogicHandler::messageReceived(EIoFilter::NextFilter* nextFilter,
		sp<EIoBuffer>& buf) {
	throw ETODOEXCEPTION;
}

void ESocks5LogicHandler::handleResponse(EIoFilter::NextFilter* nextFilter,
		sp<EIoBuffer>& buf, int step) {
	throw ETODOEXCEPTION;
}

void ESocks5LogicHandler::closeSession(const char* message) {
	throw ETODOEXCEPTION;
}

sp<EIoBuffer> ESocks5LogicHandler::encodeInitialGreetingPacket(
		sp<ESocksProxyRequest>& request) {
	throw ETODOEXCEPTION;
}

sp<EIoBuffer> ESocks5LogicHandler::encodeProxyRequestPacket(
		sp<ESocksProxyRequest>& request) {
	throw ETODOEXCEPTION;
}

sp<EIoBuffer> ESocks5LogicHandler::encodeAuthenticationPacket(
		sp<ESocksProxyRequest>& request) {
	throw ETODOEXCEPTION;
}

sp<EIoBuffer> ESocks5LogicHandler::encodeGSSAPIAuthenticationPacket(
		sp<ESocksProxyRequest>& request) {
	throw ETODOEXCEPTION;
}

void ESocks5LogicHandler::writeRequest(EIoFilter::NextFilter* nextFilter,
		sp<ESocksProxyRequest>& request, int step) {
	throw ETODOEXCEPTION;
}

} /* namespace eio */
} /* namespace efc */
