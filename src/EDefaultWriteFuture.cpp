/*
 * EDefaultWriteFuture.cpp
 *
 *  Created on: 2016-1-12
 *      Author: cxxjava@163.com
 */

#include "../inc/EDefaultWriteFuture.hh"

namespace efc {
namespace eio {

EWriteFuture* EDefaultWriteFuture::newWrittenFuture(sp<EIoSession> session) {
	EDefaultWriteFuture* writtenFuture = new EDefaultWriteFuture(session);
	writtenFuture->setWritten();

	return writtenFuture;
}

EWriteFuture* EDefaultWriteFuture::newNotWrittenFuture(sp<EIoSession> session,
		EObject* cause) {
	EDefaultWriteFuture* unwrittenFuture = new EDefaultWriteFuture(session);
	unwrittenFuture->setException(cause);

	return unwrittenFuture;
}

EDefaultWriteFuture::~EDefaultWriteFuture() {
    //
}

EDefaultWriteFuture::EDefaultWriteFuture(sp<EIoSession> session) : EDefaultIoFuture(session) {
}

boolean EDefaultWriteFuture::isWritten() {
	if (isDone()) {
		EObject* v = getValue();

		EBoolean* b = dynamic_cast<EBoolean*>(v);
		if (b) {
			return b->booleanValue();
		}
	}

	return false;
}

EThrowable* EDefaultWriteFuture::getException() {
	if (isDone()) {
		EObject* v = getValue();

		EThrowableObject<EThrowable>* t = dynamic_cast<EThrowableObject<EThrowable>*>(v);
		if (t) {
			return t->getThrowable();
		}
	}

	return null;
}

void EDefaultWriteFuture::tryThrowException() {
	if (isDone()) {
		EObject* obj = getValue();
		EThrowableType* tt = dynamic_cast<EThrowableType*>(obj);
		if (tt) {
			tt->throwException();
		}
	}
}
    
void EDefaultWriteFuture::setWritten() {
	setValue(new EBoolean(true));
}

void EDefaultWriteFuture::setException(EObject* cause) {
	if (cause == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "exception");
	}

	setValue(cause);
}

} /* namespace eio */
} /* namespace efc */
