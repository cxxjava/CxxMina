/*
 * EHttpProxyRequest.cpp
 *
 *  Created on: 2016-6-8
 *      Author: cxxjava@163.com
 */

#include "../inc/EHttpProxyRequest.hh"
#include "../inc/EHttpProxyConstants.hh"

namespace efc {
namespace eio {

sp<ELogger> EHttpProxyRequest::logger = ELoggerManager::getLogger("EHttpProxyRequest");

EHttpProxyRequest::~EHttpProxyRequest() {
	//
}

EHttpProxyRequest::EHttpProxyRequest(EInetSocketAddress* endpointAddress) {
	this->httpVerb = EHttpProxyConstants_CONNECT;
	if (!endpointAddress->isUnresolved()) {
		this->httpURI << endpointAddress->getHostName() << ":" << endpointAddress->getPort();
	} else {
		this->httpURI << endpointAddress->getAddress()->getHostAddress() << ":" << endpointAddress->getPort();
	}

	this->httpVersion = EHttpProxyConstants_HTTP_1_0;
}

EHttpProxyRequest::EHttpProxyRequest(EInetSocketAddress* endpointAddress,
		const char* httpVersion) {
	this->httpVerb = EHttpProxyConstants_CONNECT;
	if (!endpointAddress->isUnresolved()) {
		this->httpURI << endpointAddress->getHostName() << ":" << endpointAddress->getPort();
	} else {
		this->httpURI << endpointAddress->getAddress()->getHostAddress() << ":" << endpointAddress->getPort();
	}

	this->httpVersion = httpVersion;
}

EHttpProxyRequest::EHttpProxyRequest(const char* httpURI) {
	this->httpVerb = EHttpProxyConstants_GET;
	this->httpURI = httpURI;
	this->httpVersion = EHttpProxyConstants_HTTP_1_0;
}

EHttpProxyRequest::EHttpProxyRequest(const char* httpURI,
		const char* httpVersion) {
	this->httpVerb = EHttpProxyConstants_GET;
	this->httpURI = httpURI;
	this->httpVersion = httpVersion;
}

EHttpProxyRequest::EHttpProxyRequest(const char* httpVerb, const char* httpURI,
		const char* httpVersion) {
	this->httpVerb = httpVerb;
	this->httpURI = httpURI;
	this->httpVersion = httpVersion;
    this->httpVerb.toUpperCase();
}

const char* EHttpProxyRequest::getHttpVerb() {
	return httpVerb.c_str();
}

const char* EHttpProxyRequest::getHttpVersion() {
	return httpVersion.c_str();
}

void EHttpProxyRequest::setHttpVersion(const char* httpVersion) {
	this->httpVersion = httpVersion;
}

const char* EHttpProxyRequest::getHost() {
	if (host == null) {
		if (getEndpointAddress() != null && !getEndpointAddress()->isUnresolved()) {
			host = new EString(getEndpointAddress()->getHostName());
		}

		if (host == null && httpURI.length() > 0) {
			try {
				EURI uri(httpURI.c_str());
				host = new EString(uri.getHost());
			} catch (EMalformedURLException& e) {
				logger->debug(__FILE__, __LINE__, "Malformed URL", e);
			}
		}
	}

    return (host != null ? host->c_str() : null);
}

const char* EHttpProxyRequest::getHttpURI() {
	return httpURI.c_str();
}

EMap<EString*, EList<EString*>*>* EHttpProxyRequest::getHeaders() {
	return &headers;
}

void EHttpProxyRequest::setHeaders(EMap<EString*, EList<EString*>*>* headers) {
	if (!headers) return;

	this->headers.clear(); // Clear old.

	sp<EIterator<EMapEntry<EString*, EList<EString*>*>*> > iter = headers->entrySet()->iterator();
	while (iter->hasNext()) {
		EMapEntry<EString*, EList<EString*>*>* header = iter->next();
		EArrayList<EString*>* listCopy = null;
		EList<EString*>* values = header->getValue();
		if (values) {
			listCopy = new EArrayList<EString*>();
			sp<EIterator<EString*> > iter2 = values->iterator();
			while (iter2->hasNext()) {
				listCopy->add(new EString(iter2->next()));
			}
		}
		this->headers.put(new EString(header->getKey()->c_str()), listCopy);
	}
}

EMap<EString*, EString*>* EHttpProxyRequest::getProperties() {
	return &properties;
}

void EHttpProxyRequest::setProperties(EMap<EString*, EString*>* properties) {
	if (!properties) return;

	this->properties.clear(); // Clear old.

	sp<EIterator<EMapEntry<EString*, EString*>*> > iter = properties->entrySet()->iterator();
	while (iter->hasNext()) {
		EMapEntry<EString*, EString*>* header = iter->next();
		this->properties.put(new EString(header->getKey()->c_str()), new EString(header->getValue()->c_str()));
	}
}

void EHttpProxyRequest::checkRequiredProperties(const char* propNames, ...) {
	if (!propNames) return;

	EString sb;

	va_list args;
	va_start(args, propNames);
	char *v;
	while ((v = va_arg (args, char*))) {
		EString key(v);
		if (properties.get(&key) == null) {
			sb.append(v).append(' ');
		}
	}
	va_end(args);

	if (sb.length() > 0) {
		sb.append("property(ies) missing in request");
		throw EProxyAuthException(__FILE__, __LINE__, sb.c_str());
	}
}

EString EHttpProxyRequest::toHttpString() {
	EString sb;

	sb.append(getHttpVerb()).append(' ').append(getHttpURI()).append(' ').append(getHttpVersion())
			.append(EHttpProxyConstants_CRLF);

	boolean hostHeaderFound = false;

	if (getHeaders() != null) {
		sp<EIterator<EMapEntry<EString*, EList<EString*>*>*> > iter = getHeaders()->entrySet()->iterator();
		while (iter->hasNext()) {
			EMapEntry<EString*, EList<EString*>*>* header = iter->next();
			if (!hostHeaderFound) {
				hostHeaderFound = header->getKey()->equalsIgnoreCase("host");
			}

			EList<EString*>* values = header->getValue();
			if (values) {
				sp<EIterator<EString*> > iter2 = values->iterator();
				while (iter2->hasNext()) {
					sb.append(header->getKey()->c_str()).append(": ").append(iter2->next()).append(EHttpProxyConstants_CRLF);
				}
			}
		}

		if (!hostHeaderFound && (eso_strcmp(getHttpVersion(), EHttpProxyConstants_HTTP_1_1) == 0)) {
			sb.append("Host: ").append(getHost()).append(EHttpProxyConstants_CRLF);
		}
	}

	sb.append(EHttpProxyConstants_CRLF);

	return sb;
}

} /* namespace eio */
} /* namespace efc */
