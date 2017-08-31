/*
 * EDefaultConnectFuture.cpp
 *
 *  Created on: 2016-3-22
 *      Author: cxxjava@163.com
 */

#include "../inc/EDefaultConnectFuture.hh"
#include "../inc/ERuntimeIoException.hh"

namespace efc {
namespace eio {

class CanceledObject: public EObject {
};

EDefaultConnectFuture::EDefaultConnectFuture() :
		EDefaultIoFuture(null) {
}

sp<EIoSession> EDefaultConnectFuture::getSession() {
	EObject* v = getValue();

	/* @see:
	if (v instanceof IoSession) {
		return (IoSession) v;
	} else if (v instanceof RuntimeException) {
		throw (RuntimeException) v;
	} else if (v instanceof Error) {
		throw (Error) v;
	} else if (v instanceof Throwable) {
		throw (RuntimeIoException) new RuntimeIoException("Failed to get the session.").initCause((Throwable) v);
	} else  {
		return null;
	}
	*/

	sp<EIoSession>* is = dynamic_cast<sp<EIoSession>*>(v);
	if (is) {
		return sp<EIoSession>(*is);
	}

	EThrowableType* tt = dynamic_cast<EThrowableType*>(v);
	if (tt) {
		tt->throwException(); //throw (RuntimeException) v;
	}

	EThrowable* t = dynamic_cast<EThrowable*>(v);
	if (t) {
		throw ERuntimeIoException(t->getSourceFile(), t->getSourceLine(), "Failed to get the session.");
	}

	return null;
}

EThrowable* EDefaultConnectFuture::getException() {
	EObject* v = getValue();

	EThrowableType* tt = dynamic_cast<EThrowableType*>(v);
	if (tt) {
		return tt->getThrowable();
	} else {
		return null;
	}
}

void EDefaultConnectFuture::tryThrowException() {
	EObject* obj = getValue();
	EThrowableType* tt = dynamic_cast<EThrowableType*>(obj);
	if (tt) {
		tt->throwException();
	}
}

boolean EDefaultConnectFuture::isConnected() {
	return instanceof<sp<EIoSession> >(getValue());
}

boolean EDefaultConnectFuture::isCanceled() {
	EObject* v = getValue();
	return (instanceof<CanceledObject>(v));
}

void EDefaultConnectFuture::setSession(sp<EIoSession> session) {
	if (session == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "session");
	}

	setValue(new sp<EIoSession>(session));
}

void EDefaultConnectFuture::setException(EObject* exception) {
	if (exception == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "session");
	}

	setValue(exception);
}

boolean EDefaultConnectFuture::cancel() {
	return setValue(new CanceledObject());
}

EConnectFuture* EDefaultConnectFuture::newFailedFuture(EObject* exception) {
	EDefaultConnectFuture* failedFuture = new EDefaultConnectFuture();
	failedFuture->setException(exception);

	return failedFuture;
}

} /* namespace eio */
} /* namespace efc */
