/*
 * EHttpResponseImpl.cpp
 *
 *  Created on: 2017-1-4
 *      Author: cxxjava@163.com
 */

#include "../inc/EHttpResponseImpl.hh"

namespace efc {
namespace eio {

EHttpResponseImpl::~EHttpResponseImpl() {
	//
}

EHttpResponseImpl::EHttpResponseImpl(EString* version, int status, sp<EMap<EString*, EString*> > headers) {
	this->version = version;
	this->status = status;
	this->headers = headers;
}

const char* EHttpResponseImpl::getProtocolVersion() {
	return version.c_str();
}

sp<EString> EHttpResponseImpl::getContentType() {
	EString k("content-type");
	return new EString(headers->get(&k));
}

boolean EHttpResponseImpl::isKeepAlive() {
	// TODO check header and version for keep alive
	return false;
}

sp<EString> EHttpResponseImpl::getHeader(const char* name) {
	EString k(name);
	return new EString(headers->get(&k));
}

boolean EHttpResponseImpl::containsHeader(const char* name) {
	EString k(name);
	return headers->containsKey(&k);
}

sp<EMap<EString*, EString*> > EHttpResponseImpl::getHeaders() {
	return headers;
}

int EHttpResponseImpl::getStatus() {
	return status;
}

EStringBase EHttpResponseImpl::toString() {
	EString sb;
	sb.append("HTTP RESPONSE STATUS: " ).append(EHttpStatus::valueOf(status)->line()).append('\n');
	sb.append("VERSION: ").append(version).append('\n');

	sb.append("-- HEADER --- \n");

	sp<EIterator<EMapEntry<EString*,EString*>*> > iter = headers->entrySet()->iterator();
	while (iter->hasNext()) {
		EMapEntry<EString*,EString*>* entry = iter->next();
		sb.append(entry->getKey()).append(':').append(entry->getValue()).append('\n');
	}

	return sb;
}

} /* namespace eio */
} /* namespace efc */
