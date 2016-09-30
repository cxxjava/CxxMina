/*
 * ENioSession.cpp
 *
 *  Created on: 2016-1-19
 *      Author: cxxjava@163.com
 */

#include "ENioSession.hh"
#include "EDefaultIoFilterChain.hh"
#include "EIoService.hh"

namespace efc {
namespace eio {

ENioSession::~ENioSession() {
	delete filterChain;
}

ENioSession::ENioSession(EIoProcessor* processor,
		EIoService* service, EByteChannel* channel) :
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

ESelectionKey* ENioSession::getSelectionKey() {
	return key;
}

void ENioSession::setSelectionKey(ESelectionKey* key) {
	this->key = key;
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
		return key ? key->isValid() : false;
    }}
}

boolean ENioSession::isDestroyed() {
	return destroyed;
}

void ENioSession::destroy() {
	ESelectionKey* k = null;

	SYNCBLOCK(&keyLock) {
		if (destroyed) {
			return;
		}
		destroyed = true;
		k = key;
		key = null;
    }}

	if (k != null) {
		//@see: key->cancel();

		//! cxxjava@163.com
		sp<ENioSession>* s = null;
		SYNCHRONIZED(k) {
			s = (sp<ENioSession>*)(k->attachment());
			k->attach(null);
        }}
		delete s;
	}
	channel->close();
}

} /* namespace eio */
} /* namespace efc */
