/*
 * EHttpRequestImpl.cpp
 *
 *  Created on: 2017-1-4
 *      Author: cxxjava@163.com
 */

#include "../inc/EHttpRequestImpl.hh"

namespace efc {
namespace eio {

EHttpRequestImpl::~EHttpRequestImpl() {
	//
}

EHttpRequestImpl::EHttpRequestImpl(EString* version, EString* method,
		EString* requestedPath, EString* queryString,
			sp<EMap<EString*, EString*> > headers) {
	this->version = version;
	this->method = method;
	this->requestedPath = requestedPath ? new EString(requestedPath) : null;
	this->queryString = queryString ? new EString(queryString) : null;
	this->headers = headers;
}

const char* EHttpRequestImpl::getProtocolVersion() {
	return version.c_str();
}

sp<EString> EHttpRequestImpl::getContentType() {
	EString k("content-type");
	return new EString(headers->get(&k));
}

boolean EHttpRequestImpl::isKeepAlive() {
	return false;
}

sp<EString> EHttpRequestImpl::getHeader(const char* name) {
	EString k(name);
	EString* v = headers->get(&k);
	return new EString(v);
}

boolean EHttpRequestImpl::containsHeader(const char* name) {
	EString key(name);
	return headers->containsKey(&key);
}

sp<EMap<EString*, EString*> > EHttpRequestImpl::getHeaders() {
	return headers;
}

boolean EHttpRequestImpl::containsParameter(const char* name) {
	EString s("[&]"); s << name << "=([^&]*)";
	EPattern p(s.c_str());
	EString t("&"); t << queryString->c_str();
	EMatcher m(&p, t.c_str());
	return m.find();
}

sp<EString> EHttpRequestImpl::getParameter(const char* name) {
	EString s("[&]"); s << name << "=([^&]*)";
	EPattern p(s.c_str());
	EString t("&"); t << queryString->c_str();
	EMatcher m(&p, t.c_str());
	if (m.find()) {
		return new EString(m.group(1));
	} else {
		return null;
	}
}

sp<EString> EHttpRequestImpl::getQueryString() {
	return queryString;
}

sp<EMap<EString*, EList<EString*>*> > EHttpRequestImpl::getParameters() {
	sp<EMap<EString*, EList<EString*>*> > parameters = new EHashMap<EString*, EList<EString*>*>();
	EArray<EString*> params = EPattern::split("&", queryString->c_str());
	if (params.length() == 1) {
		return parameters;
	}
	for (int i = 0; i < params.length(); i++) {
		EArray<EString*> param = EPattern::split("=", params[i]->c_str());
		EString* name = param[0];
		EString* value = param.length() == 2 ? param[1] : null;
		if (!parameters->containsKey(name)) {
			parameters->put(new EString(name), new EArrayList<EString*>());
		}
		parameters->get(name)->add(new EString(value));
	}
	return parameters;
}

const char* EHttpRequestImpl::getMethod() {
	return method.c_str();
}

sp<EString> EHttpRequestImpl::getRequestPath() {
	return requestedPath;
}


EString EHttpRequestImpl::toString() {
	EString sb;
	sb.append("HTTP REQUEST METHOD: ").append(method).append('\n');
	sb.append("VERSION: ").append(version).append('\n');
	sb.append("PATH: ").append(requestedPath.get()).append('\n');
	sb.append("QUERY:").append(queryString.get()).append('\n');

	sb.append("--- HEADER --- \n");

	sp<EIterator<EMapEntry<EString*,EString*>*> > iter = headers->entrySet()->iterator();
	while (iter->hasNext()) {
		EMapEntry<EString*,EString*>* entry = iter->next();
		sb.append(entry->getKey()).append(':').append(entry->getValue()).append('\n');
	}

	sb.append("--- PARAMETERS --- \n");
	sp<EMap<EString*, EList<EString*>*> > parameters = getParameters();

	sp<EIterator<EMapEntry<EString*,EList<EString*>*>*> > iter2 = parameters->entrySet()->iterator();
	while (iter2->hasNext()) {
		EMapEntry<EString*,EList<EString*>*>* entry = iter2->next();

		EString* key = entry->getKey();

		sp<EIterator<EString*> > i = entry->getValue()->iterator();
		while (i->hasNext()) {
			EString* value = i->next();
			sb.append(key).append(':').append(value).append('\n');
		}
	}

	return sb;
}

} /* namespace eio */
} /* namespace efc */
