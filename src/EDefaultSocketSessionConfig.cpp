/*
 * EDefaultSocketSessionConfig.cpp
 *
 *  Created on: 2016-1-19
 *      Author: cxxjava@163.com
 */

#include "../inc/EDefaultSocketSessionConfig.hh"
#include "../inc/ESocketAcceptor.hh"

namespace efc {
namespace eio {

EDefaultSocketSessionConfig::EDefaultSocketSessionConfig() :
		parent(null),
		defaultReuseAddress(false),
		reuseAddress(false),
		receiveBufferSize(-1),
		sendBufferSize(-1),
		trafficClass(DEFAULT_TRAFFIC_CLASS),
		keepAlive(DEFAULT_KEEP_ALIVE),
		oobInline(DEFAULT_OOB_INLINE),
		soLinger(DEFAULT_SO_LINGER),
		tcpNoDelay(DEFAULT_TCP_NO_DELAY) {
	// Do nothing
}

void EDefaultSocketSessionConfig::init(EIoService* parent) {
	this->parent = parent;

	ESocketAcceptor* sa = dynamic_cast<ESocketAcceptor*>(parent);
	if (sa) {
		defaultReuseAddress = true;
	} else {
		defaultReuseAddress = DEFAULT_REUSE_ADDRESS;
	}

	reuseAddress = defaultReuseAddress;
}

boolean EDefaultSocketSessionConfig::isReuseAddress() {
	return reuseAddress;
}

void EDefaultSocketSessionConfig::setReuseAddress(boolean reuseAddress) {
	this->reuseAddress = reuseAddress;
}

int EDefaultSocketSessionConfig::getReceiveBufferSize() {
	return receiveBufferSize;
}

void EDefaultSocketSessionConfig::setReceiveBufferSize(int receiveBufferSize) {
	this->receiveBufferSize = receiveBufferSize;
}

int EDefaultSocketSessionConfig::getSendBufferSize() {
	return sendBufferSize;
}

void EDefaultSocketSessionConfig::setSendBufferSize(int sendBufferSize) {
	this->sendBufferSize = sendBufferSize;
}

int EDefaultSocketSessionConfig::getTrafficClass() {
	return trafficClass;
}

void EDefaultSocketSessionConfig::setTrafficClass(int trafficClass) {
	this->trafficClass = trafficClass;
}

boolean EDefaultSocketSessionConfig::isKeepAlive() {
	return keepAlive;
}

void EDefaultSocketSessionConfig::setKeepAlive(boolean keepAlive) {
	this->keepAlive = keepAlive;
}

boolean EDefaultSocketSessionConfig::isOobInline() {
	return oobInline;
}

void EDefaultSocketSessionConfig::setOobInline(boolean oobInline) {
	this->oobInline = oobInline;
}

int EDefaultSocketSessionConfig::getSoLinger() {
	return soLinger;
}

void EDefaultSocketSessionConfig::setSoLinger(int soLinger) {
	this->soLinger = soLinger;
}

boolean EDefaultSocketSessionConfig::isTcpNoDelay() {
	return tcpNoDelay;
}

void EDefaultSocketSessionConfig::setTcpNoDelay(boolean tcpNoDelay) {
	this->tcpNoDelay = tcpNoDelay;
}

boolean EDefaultSocketSessionConfig::isKeepAliveChanged() {
	return keepAlive != DEFAULT_KEEP_ALIVE;
}

boolean EDefaultSocketSessionConfig::isOobInlineChanged() {
	return oobInline != DEFAULT_OOB_INLINE;
}

boolean EDefaultSocketSessionConfig::isReceiveBufferSizeChanged() {
	return receiveBufferSize != -1;
}

boolean EDefaultSocketSessionConfig::isReuseAddressChanged() {
	return reuseAddress != defaultReuseAddress;
}

boolean EDefaultSocketSessionConfig::isSendBufferSizeChanged() {
	return sendBufferSize != -1;
}

boolean EDefaultSocketSessionConfig::isSoLingerChanged() {
	return soLinger != DEFAULT_SO_LINGER;
}

boolean EDefaultSocketSessionConfig::isTcpNoDelayChanged() {
	return tcpNoDelay != DEFAULT_TCP_NO_DELAY;
}

boolean EDefaultSocketSessionConfig::isTrafficClassChanged() {
	return trafficClass != DEFAULT_TRAFFIC_CLASS;
}

} /* namespace eio */
} /* namespace efc */
