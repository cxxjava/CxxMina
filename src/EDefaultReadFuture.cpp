/*
 * EDefaultReadFuture.cpp
 *
 *  Created on: 2016-1-11
 *      Author: cxxjava@163.com
 */

#include "EDefaultReadFuture.hh"
#include "ERuntimeIoException.hh"

namespace efc {
namespace eio {

class CloseObject: public EObject {
};

EDefaultReadFuture::~EDefaultReadFuture() {
    //
}

EDefaultReadFuture::EDefaultReadFuture(sp<EIoSession> session) : EDefaultIoFuture(session) {
}

EObject* EDefaultReadFuture::getMessage() {
	if (EDefaultIoFuture::isDone()) {
		EObject* v = getValue();

		if (instanceof<CloseObject>(v)) {
			return null;
		}

		/* @see:
		if (v instanceof RuntimeException) {
			throw (RuntimeException) v;
		}

		if (v instanceof Error) {
			throw (Error) v;
		}

		if (v instanceof IOException || v instanceof Exception) {
			throw new RuntimeIoException((Exception) v);
		}
		*/

		EThrowableType* tt = dynamic_cast<EThrowableType*>(v);
		if (tt) {
			ERuntimeException* re = dynamic_cast<ERuntimeException*>(tt->getThrowable());
			if (re) {
				tt->throwException(); //throw (RuntimeException) v;
			}

			EIOException* ioe = dynamic_cast<EIOException*>(tt->getThrowable());
			if (ioe) {
				throw ERuntimeIoException(ioe->getSourceFile(), ioe->getSourceLine(), ioe->getMessage());
			}

			EException* e = dynamic_cast<EException*>(tt->getThrowable());
			if (e) {
				throw ERuntimeIoException(e->getSourceFile(), e->getSourceLine(), e->getMessage());
			}
		}

		return v;
	}

	return null;
}

boolean EDefaultReadFuture::isRead() {
	if (EDefaultIoFuture::isDone()) {
		EObject* v = getValue();

		EThrowable* t = dynamic_cast<EThrowable*>(v);
		return (!instanceof<CloseObject>(v) && !t);
	}

	return false;
}

boolean EDefaultReadFuture::isClosed() {
	if (EDefaultIoFuture::isDone()) {
		EObject* v = getValue();
		return (instanceof<CloseObject>(v));
	}

	return false;
}

EThrowable* EDefaultReadFuture::getException() {
	if (isDone()) {
		EObject* obj = getValue();
		EThrowableType* tt = dynamic_cast<EThrowableType*>(obj);
		if (tt) {
			return tt->getThrowable();
		}
	}

	return null;
}

void EDefaultReadFuture::tryThrowException() {
	if (isDone()) {
		EObject* obj = getValue();
		EThrowableType* tt = dynamic_cast<EThrowableType*>(obj);
		if (tt) {
			tt->throwException();
		}
	}
}

void EDefaultReadFuture::setRead(EObject* message) {
	if (message == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "message");
	}

	setValue(message);
}

void EDefaultReadFuture::setClosed() {
	setValue(new CloseObject());
}

void EDefaultReadFuture::setException(EObject* cause) {
	if (cause == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "exception");
	}

	setValue(cause);
}

} /* namespace eio */
} /* namespace efc */
