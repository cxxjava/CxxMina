/*
 * EDefaultIoSessionDataStructureFactory.cpp
 *
 *  Created on: 2015-12-31
 *      Author: cxxjava@163.com
 */

#include "../inc/EDefaultIoSessionDataStructureFactory.hh"
#include "../inc/EAbstractIoSession.hh"

namespace efc {
namespace eio {

EIoSessionAttributeMap* EDefaultIoSessionDataStructureFactory::getAttributeMap(EIoSession* session) {
	return new DefaultIoSessionAttributeMap();
}

EWriteRequestQueue* EDefaultIoSessionDataStructureFactory::getWriteRequestQueue(EIoSession* session) {
	return new DefaultWriteRequestQueue();
}

EDefaultIoSessionDataStructureFactory::DefaultIoSessionAttributeMap::DefaultIoSessionAttributeMap() {
}

sp<EObject> EDefaultIoSessionDataStructureFactory::DefaultIoSessionAttributeMap::getAttribute(
		EIoSession* session, EObject* key, EObject* defaultValue) {
	if (key == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "key");
	}

	if (defaultValue == null) {
		return attributes.get(key);
	}

	sp<EObject> object = attributes.putIfAbsent(key, defaultValue);

	if (object == null) {
		return defaultValue;
	} else {
		return object;
	}
}

sp<EObject> EDefaultIoSessionDataStructureFactory::DefaultIoSessionAttributeMap::setAttribute(
		EIoSession* session, sp<EObject> key, sp<EObject> value) {
	if (key == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "key");
	}

	if (value == null) {
		return attributes.remove(key.get());
	}

	return attributes.put(key, value);
}

sp<EObject> EDefaultIoSessionDataStructureFactory::DefaultIoSessionAttributeMap::setAttributeIfAbsent(
		EIoSession* session, sp<EObject> key, sp<EObject> value) {
	if (key == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "key");
	}

	if (value == null) {
		return null;
	}

	return attributes.putIfAbsent(key, value);
}

sp<EObject> EDefaultIoSessionDataStructureFactory::DefaultIoSessionAttributeMap::removeAttribute(
		EIoSession* session, EObject* key) {
	if (key == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "key");
	}

	return attributes.remove(key);
}

boolean EDefaultIoSessionDataStructureFactory::DefaultIoSessionAttributeMap::removeAttribute(
		EIoSession* session, EObject* key, EObject* value) {
	if (key == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "key");
	}

	if (value == null) {
		return false;
	}

	try {
		return attributes.remove(key, value);
	} catch (ENullPointerException& e) {
		return false;
	}
}

boolean EDefaultIoSessionDataStructureFactory::DefaultIoSessionAttributeMap::replaceAttribute(
		EIoSession* session, EObject* key, EObject* oldValue,
		sp<EObject> newValue) {
	try {
		return attributes.replace(key, oldValue, newValue);
	} catch (ENullPointerException& e) {
	}

	return false;
}

boolean EDefaultIoSessionDataStructureFactory::DefaultIoSessionAttributeMap::containsAttribute(
		EIoSession* session, EObject* key) {
	return attributes.containsKey(key);
}

ESet<sp<EObject> >* EDefaultIoSessionDataStructureFactory::DefaultIoSessionAttributeMap::getAttributeKeys(
		EIoSession* session) {
	SYNCBLOCK(&attributesLock) {
		return attributes.keySet();
    }}
}

void EDefaultIoSessionDataStructureFactory::DefaultIoSessionAttributeMap::dispose(
		sp<EIoSession> session) {
	// Do nothing
}

EDefaultIoSessionDataStructureFactory::DefaultWriteRequestQueue::DefaultWriteRequestQueue() {
}

sp<EWriteRequest> EDefaultIoSessionDataStructureFactory::DefaultWriteRequestQueue::poll(
		sp<EIoSession> session) {
	sp<EWriteRequest> answer = q.poll();

	if (answer == EAbstractIoSession::CLOSE_REQUEST) {
		session->closeNow();
		dispose(session);
		answer = null;
	}

	return answer;
}

void EDefaultIoSessionDataStructureFactory::DefaultWriteRequestQueue::offer(
		sp<EIoSession> session, EWriteRequest* writeRequest) {
	q.offer(writeRequest);
}

void EDefaultIoSessionDataStructureFactory::DefaultWriteRequestQueue::offer(
		sp<EIoSession> session, sp<EWriteRequest> writeRequest) {
//	SYNCHRONIZED(this) {
		q.offer(writeRequest);
//    }}
}

boolean EDefaultIoSessionDataStructureFactory::DefaultWriteRequestQueue::isEmpty(
		sp<EIoSession> session) {
	return q.isEmpty();
}

void EDefaultIoSessionDataStructureFactory::DefaultWriteRequestQueue::clear(
		sp<EIoSession> session) {
	q.clear();
}

void EDefaultIoSessionDataStructureFactory::DefaultWriteRequestQueue::dispose(
		sp<EIoSession> session) {
	// Do nothing
}

int EDefaultIoSessionDataStructureFactory::DefaultWriteRequestQueue::size() {
	return q.size();
}

EString EDefaultIoSessionDataStructureFactory::DefaultWriteRequestQueue::toString() {
	return q.toString();
}

} /* namespace eio */
} /* namespace efc */
