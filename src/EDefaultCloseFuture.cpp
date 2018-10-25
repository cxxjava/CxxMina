/*
 * EDefaultCloseFuture.cpp
 *
 *  Created on: 2016-1-21
 *      Author: cxxjava@163.com
 */

#include "../inc/EDefaultCloseFuture.hh"

namespace efc {
namespace eio {

EDefaultCloseFuture::~EDefaultCloseFuture() {
    //
}

EDefaultCloseFuture::EDefaultCloseFuture(sp<EIoSession> session) :
		EDefaultIoFuture(session) {
}

boolean EDefaultCloseFuture::isClosed() {
	if (isDone()) {
		return (dynamic_pointer_cast<EBoolean>(getValue()))->booleanValue();
	} else {
		return false;
	}
}

void EDefaultCloseFuture::setClosed() {
	setValue(new EBoolean(TRUE));
}

} /* namespace eio */
} /* namespace efc */
