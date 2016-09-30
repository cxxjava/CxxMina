/*
 * EDefaultWriteRequest.cpp
 *
 *  Created on: 2016-1-6
 *      Author: cxxjava@163.com
 */

#include "EDefaultWriteRequest.hh"
#include "EIoFutureListener.hh"
#include "EWriteFuture.hh"

namespace efc {
namespace eio {

class UnusedWriteFuture : public EWriteFuture {
public:
	boolean isWritten() {
		return false;
	}

	void setWritten() {
		// Do nothing
	}

	sp<EIoSession> getSession() {
		return null;
	}

	void join() {
		// Do nothing
	}

	boolean join(long timeoutInMillis) {
		return true;
	}

	boolean isDone() {
		return true;
	}

	void addListener(sp<EIoFutureListener> listener) {
		throw EIllegalStateException(__FILE__, __LINE__, "You can't add a listener to a dummy future.");
	}

	void removeListener(EIoFutureListener* listener) {
		throw EIllegalStateException(__FILE__, __LINE__, "You can't add a listener to a dummy future.");
	}

	void await() {
		//
	}

	boolean await(llong timeout, ETimeUnit* unit) {
		return true;
	}

	boolean await(llong timeoutMillis) {
		return true;
	}

	void awaitUninterruptibly() {
		//
	}

	boolean awaitUninterruptibly(llong timeout, ETimeUnit* unit) {
		return true;
	}

	boolean awaitUninterruptibly(llong timeoutMillis) {
		return true;
	}

	EThrowable* getException() {
		return null;
	}

	void setException(EObject* cause) {
		// Do nothing
	}
    
    virtual void tryThrowException() {
        // Do nothing
    }
};

EDefaultWriteRequest::~EDefaultWriteRequest() {
    //
}

EDefaultWriteRequest::EDefaultWriteRequest(sp<EObject> message) {
	init(message, null, null);
}

EDefaultWriteRequest::EDefaultWriteRequest(sp<EObject> message,
		sp<EWriteFuture> future) {
	init(message, future, null);
}

EDefaultWriteRequest::EDefaultWriteRequest(sp<EObject> message,
		sp<EWriteFuture> future, EInetSocketAddress* destination) {
	init(message, future, destination);
}

void EDefaultWriteRequest::init(sp<EObject>& message,
		sp<EWriteFuture> future, EInetSocketAddress* destination) {
	if (message == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "message");
	}

	if (future == null) {
		future = new UnusedWriteFuture();
	}

	this->message = message;
	this->future = future;
	this->destination = destination;
}

sp<EWriteFuture> EDefaultWriteRequest::getFuture() {
	return future;
}

sp<EObject> EDefaultWriteRequest::getMessage() {
	return message;
}

EWriteRequest* EDefaultWriteRequest::getOriginalRequest() {
	return this;
}

EInetSocketAddress* EDefaultWriteRequest::getDestination() {
	return destination;
}

boolean EDefaultWriteRequest::isEncoded() {
	return false;
}

EStringBase EDefaultWriteRequest::toString() {
	EStringBase sb("WriteRequest: ");

	// Special case for the CLOSE_REQUEST writeRequest : it just
	// carries a native Object instance
	//@see: if (message.getClass().getName().equals(Object.class.getName())) {
	if (strcmp(typeid(message).name(), typeid(EObject).name()) == 0) {
		sb.append("CLOSE_REQUEST");
	} else {
		if (getDestination() == null) {
			sb.append(message->toString());
		} else {
			sb.append(message->toString());
			sb.append(" => ");
			sb.append(getDestination() ? getDestination()->toString() : "null");
		}
	}

	return sb;
}

} /* namespace eio */
} /* namespace efc */
