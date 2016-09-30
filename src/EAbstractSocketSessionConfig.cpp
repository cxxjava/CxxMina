/*
 * EAbstractSocketSessionConfig.cpp
 *
 *  Created on: 2016-1-19
 *      Author: cxxjava@163.com
 */

#include "EAbstractSocketSessionConfig.hh"

namespace efc {
namespace eio {

void EAbstractSocketSessionConfig::doSetAll(EIoSessionConfig* config) {
	ESocketSessionConfig* ssc = dynamic_cast<ESocketSessionConfig*>(config);
	if (!ssc) {
		return;
	}

	EAbstractSocketSessionConfig* cfg = dynamic_cast<EAbstractSocketSessionConfig*>(config);
	if (cfg) {
        // Minimize unnecessary system calls by checking all 'propertyChanged' properties.
        if (cfg->isKeepAliveChanged()) {
            setKeepAlive(cfg->isKeepAlive());
        }
        if (cfg->isOobInlineChanged()) {
            setOobInline(cfg->isOobInline());
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
        if (cfg->isSoLingerChanged()) {
            setSoLinger(cfg->getSoLinger());
        }
        if (cfg->isTcpNoDelayChanged()) {
            setTcpNoDelay(cfg->isTcpNoDelay());
        }
        if (cfg->isTrafficClassChanged() && getTrafficClass() != cfg->getTrafficClass()) {
            setTrafficClass(cfg->getTrafficClass());
        }
    } else {
        ESocketSessionConfig* cfg = dynamic_cast<ESocketSessionConfig*>(config);
        setKeepAlive(cfg->isKeepAlive());
        setOobInline(cfg->isOobInline());
        setReceiveBufferSize(cfg->getReceiveBufferSize());
        setReuseAddress(cfg->isReuseAddress());
        setSendBufferSize(cfg->getSendBufferSize());
        setSoLinger(cfg->getSoLinger());
        setTcpNoDelay(cfg->isTcpNoDelay());
        if (getTrafficClass() != cfg->getTrafficClass()) {
            setTrafficClass(cfg->getTrafficClass());
        }
    }
}

} /* namespace eio */
} /* namespace efc */
