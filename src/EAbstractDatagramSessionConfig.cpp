/*
 * EAbstractDatagramSessionConfig.cpp
 *
 *  Created on: 2016-8-30
 *      Author: cxxjava@163.com
 */

#include "../inc/EAbstractDatagramSessionConfig.hh"

namespace efc {
namespace eio {

EAbstractDatagramSessionConfig::EAbstractDatagramSessionConfig():
		closeOnPortUnreachable(DEFAULT_CLOSE_ON_PORT_UNREACHABLE) {
}

boolean EAbstractDatagramSessionConfig::isCloseOnPortUnreachable() {
	return closeOnPortUnreachable;
}

void EAbstractDatagramSessionConfig::setCloseOnPortUnreachable(
		boolean closeOnPortUnreachable) {
	this->closeOnPortUnreachable = closeOnPortUnreachable;
}

void EAbstractDatagramSessionConfig::doSetAll(EIoSessionConfig* config) {
	if (!(instanceof<EDatagramSessionConfig>(config))) {
		return;
	}

	EAbstractDatagramSessionConfig* cfg = dynamic_cast<EAbstractDatagramSessionConfig*>(config);
	if (cfg) {
		// Minimize unnecessary system calls by checking all 'propertyChanged' properties.
		if (cfg->isBroadcastChanged()) {
			setBroadcast(cfg->isBroadcast());
		}
		if (cfg->isReceiveBufferSizeChanged()) {
			setReceiveBufferSize(cfg->getReceiveBufferSize());
		}
		if (cfg->isReuseAddressChanged()) {
			setReuseAddress(cfg->isReuseAddress());
		}
		if (cfg->isSendBufferSizeChanged()) {
			setSendBufferSize(cfg->getSendBufferSize());
		}
		if (cfg->isTrafficClassChanged() && getTrafficClass() != cfg->getTrafficClass()) {
			setTrafficClass(cfg->getTrafficClass());
		}
	} else {
		EDatagramSessionConfig* cfg = dynamic_cast<EDatagramSessionConfig*>(config);
		setBroadcast(cfg->isBroadcast());
		setReceiveBufferSize(cfg->getReceiveBufferSize());
		setReuseAddress(cfg->isReuseAddress());
		setSendBufferSize(cfg->getSendBufferSize());
		if (getTrafficClass() != cfg->getTrafficClass()) {
			setTrafficClass(cfg->getTrafficClass());
		}
	}
}

boolean EAbstractDatagramSessionConfig::isBroadcastChanged() {
	return true;
}

boolean EAbstractDatagramSessionConfig::isReceiveBufferSizeChanged() {
	return true;
}

boolean EAbstractDatagramSessionConfig::isReuseAddressChanged() {
	return true;
}

boolean EAbstractDatagramSessionConfig::isSendBufferSizeChanged() {
	return true;
}

boolean EAbstractDatagramSessionConfig::isTrafficClassChanged() {
	return true;
}

} /* namespace eio */
} /* namespace efc */
