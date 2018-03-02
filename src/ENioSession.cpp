/*
 * ENioSession.cpp
 *
 *  Created on: 2016-1-19
 *      Author: cxxjava@163.com
 */

#include "../inc/ENioSession.hh"
#include "../inc/EDefaultIoFilterChain.hh"
#include "../inc/EIoService.hh"

namespace efc {
namespace eio {

ENioSession::~ENioSession() {
	delete filterChain;
}

ENioSession::ENioSession(EIoProcessor* processor,
		EIoService* service, sp<EByteChannel> channel) :
		EAbstractIoSession(service) {
	this->channel = channel;
	this->processor = processor;
	this->key = null;
	this->destroyed = false;
}

void ENioSession::init() {
	EAbstractIoSession::init();
	filterChain = new EDefaultIoFilterChain(shared_from_this());
}

sp<ESelectionKey> ENioSession::getSelectionKey() {
	return atomic_load(&key);
}

void ENioSession::setSelectionKey(sp<ESelectionKey> key) {
	atomic_store(&this->key, key);
}

EIoFilterChain* ENioSession::getFilterChain() {
	return filterChain;
}

EIoProcessor* ENioSession::getProcessor() {
	return processor;
}

boolean ENioSession::isActive() {
	//@see: key.isValid();
	SYNCBLOCK(&keyLock) {
		return (key != null) ? key->isValid() : false;
    }}
}

boolean ENioSession::isDestroyed() {
	return destroyed;
}

void ENioSession::destroy() {
	sp<ESelectionKey> k = null;

	SYNCBLOCK(&keyLock) {
		if (destroyed) {
			return;
		}
		destroyed = true;
		k = key;
		key = null;
    }}

	if (k != null) {
		k->cancel();
	}
	channel->close();
}

} /* namespace eio */
} /* namespace efc */
