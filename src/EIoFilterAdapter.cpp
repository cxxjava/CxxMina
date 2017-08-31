/*
 * EIoFilterAdapter.cpp
 *
 *  Created on: 2016-1-22
 *      Author: cxxjava@163.com
 */

#include "../inc/EIoFilterAdapter.hh"
#include "../inc/EIoFilterChain.hh"

namespace efc {
namespace eio {

void EIoFilterAdapter::init() {
}

void EIoFilterAdapter::destroy() {
}

void EIoFilterAdapter::onPreAdd(EIoFilterChain* parent,
		const char* name, EIoFilter::NextFilter* nextFilter) {
}

void EIoFilterAdapter::onPostAdd(EIoFilterChain* parent,
		const char* name, EIoFilter::NextFilter* nextFilter) {
}

void EIoFilterAdapter::onPreRemove(EIoFilterChain* parent,
		const char* name, EIoFilter::NextFilter* nextFilter) {
}

void EIoFilterAdapter::onPostRemove(EIoFilterChain* parent,
		const char* name, EIoFilter::NextFilter* nextFilter) {
}

void EIoFilterAdapter::sessionCreated(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session) {
	nextFilter->sessionCreated(session);
}

void EIoFilterAdapter::sessionOpened(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session) {
	nextFilter->sessionOpened(session);
}

void EIoFilterAdapter::sessionClosed(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session) {
	nextFilter->sessionClosed(session);
}

void EIoFilterAdapter::sessionIdle(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, EIdleStatus status) {
	nextFilter->sessionIdle(session, status);
}

void EIoFilterAdapter::exceptionCaught(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, sp<EThrowableType>& cause) {
	nextFilter->exceptionCaught(session, cause);
}

void EIoFilterAdapter::inputClosed(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session) {
	nextFilter->inputClosed(session);
}

void EIoFilterAdapter::messageReceived(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, sp<EObject>& message) {
	nextFilter->messageReceived(session, message);
}

void EIoFilterAdapter::messageSent(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) {
	nextFilter->messageSent(session, writeRequest);
}

void EIoFilterAdapter::filterClose(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session) {
	nextFilter->filterClose(session);
}

void EIoFilterAdapter::filterWrite(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) {
	nextFilter->filterWrite(session, writeRequest);
}

EStringBase EIoFilterAdapter::toString() {
	return "EIoFilterAdapter";
}

} /* namespace eio */
} /* namespace efc */
