/*
 * EWriteRequestWrapper.cpp
 *
 *  Created on: 2016-5-21
 *      Author: cxxjava@163.com
 */

#include "../inc/EWriteRequestWrapper.hh"

namespace efc {
namespace eio {

EWriteRequestWrapper::~EWriteRequestWrapper() {

}

EWriteRequestWrapper::EWriteRequestWrapper(sp<EWriteRequest>& parentRequest) {
	if (parentRequest == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "parentRequest");
	}
	this->parentRequest = parentRequest;
}

EInetSocketAddress* EWriteRequestWrapper::getDestination() {
	return parentRequest->getDestination();
}

sp<EWriteFuture> EWriteRequestWrapper::getFuture() {
	return parentRequest->getFuture();
}

sp<EObject> EWriteRequestWrapper::getMessage() {
	return parentRequest->getMessage();
}

EWriteRequest* EWriteRequestWrapper::getOriginalRequest() {
	return parentRequest->getOriginalRequest();
}

sp<EWriteRequest> EWriteRequestWrapper::getParentRequest() {
	return parentRequest;
}

EStringBase EWriteRequestWrapper::toString() {
	return EStringBase("WR Wrapper") + parentRequest->toString();
}

boolean EWriteRequestWrapper::isEncoded() {
	return false;
}

} /* namespace eio */
} /* namespace efc */
