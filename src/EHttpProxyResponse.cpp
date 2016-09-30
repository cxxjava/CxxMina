/*
 * EHttpProxyResponse.cpp
 *
 *  Created on: 2016-6-16
 *      Author: cxxjava@lotuseed.com
 */

#include "EHttpProxyResponse.hh"

namespace efc {
namespace eio {

const char* EHttpProxyResponse::getHttpVersion() {
	return httpVersion.c_str();
}

int EHttpProxyResponse::getStatusCode() {
	return statusCode;
}

const char* EHttpProxyResponse::getStatusLine() {
	return statusLine.c_str();
}

const char* EHttpProxyResponse::getBody() {
	return body.c_str();
}

void EHttpProxyResponse::setBody(const char* body) {
	this->body = body;
}

EMap<EString*, EList<EString*>*>* EHttpProxyResponse::getHeaders() {
	return &headers;
}

EHttpProxyResponse::EHttpProxyResponse(const char* httpVersion,
		const char* statusLine, EMap<EString*, EList<EString*>*>* headers) {
	this->httpVersion = httpVersion;
	this->statusLine = statusLine;

	// parses the status code from the status line
	EString statusLine_(statusLine);
	this->statusCode = statusLine_.charAt(0) == ' ' ? EInteger::parseInt(statusLine_.substring(1, 4).c_str()) : EInteger
			::parseInt(statusLine_.substring(0, 3).c_str());

	this->headers.clear(); // Clear old.

	if (headers) {
		sp<EIterator<EMapEntry<EString*, EList<EString*>*>*> > iter = headers->entrySet()->iterator();
		while (iter->hasNext()) {
			EMapEntry<EString*, EList<EString*>*>* header = iter->next();
			EList<EString*>* v = header->getValue();
			if (v) {
				EArrayList<EString*>* list = new EArrayList<EString*>(1);
				sp<EIterator<EString*> > it = v->iterator();
				while (it->hasNext()) {
					list->add(new EString(it->next()));
				}
				this->headers.put(new EString(header->getKey()->c_str()), list);
			}
			else {
				this->headers.put(new EString(header->getKey()->c_str()), null);
			}
		}
	}
}

} /* namespace eio */
} /* namespace efc */
