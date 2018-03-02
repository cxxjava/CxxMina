/*
 * EDefaultIoSessionDataStructureFactory.hh
 *
 *  Created on: 2015-12-31
 *      Author: cxxjava@163.com
 */

#ifndef DEFAULTIOSESSIONDATASTRUCTUREFACTORY_HH_
#define DEFAULTIOSESSIONDATASTRUCTUREFACTORY_HH_

#include "./EIoSessionDataStructureFactory.hh"
#include "./EWriteRequestQueue.hh"
#include "./EWriteRequest.hh"

namespace efc {
namespace eio {

/**
 * The default {@link IoSessionDataStructureFactory} implementation
 * that creates a new {@link HashMap}-based {@link IoSessionAttributeMap}
 * instance and a new synchronized {@link ConcurrentLinkedQueue} instance per
 * {@link IoSession}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EDefaultIoSessionDataStructureFactory : public EIoSessionDataStructureFactory {
public:
	EIoSessionAttributeMap* getAttributeMap(EIoSession* session) THROWS(EException);

	EWriteRequestQueue* getWriteRequestQueue(EIoSession* session) THROWS(EException);

private:
	class DefaultIoSessionAttributeMap : public EIoSessionAttributeMap {
	public:
		/**
		 * Default constructor
		 */
		DefaultIoSessionAttributeMap();

		sp<EObject> getAttribute(EIoSession* session, EObject* key, EObject* defaultValue);
		sp<EObject> setAttribute(EIoSession* session, sp<EObject> key, sp<EObject> value);
		sp<EObject> setAttributeIfAbsent(EIoSession* session, sp<EObject> key, sp<EObject> value);
		sp<EObject> removeAttribute(EIoSession* session, EObject* key);
		boolean removeAttribute(EIoSession* session, EObject* key, EObject* value);
		boolean replaceAttribute(EIoSession* session, EObject* key, EObject* oldValue, sp<EObject> newValue);
		boolean containsAttribute(EIoSession* session, EObject* key);
		ESet<sp<EObject> >* getAttributeKeys(EIoSession* session);
		void dispose(sp<EIoSession> session) THROWS(EException);

	private:
		EConcurrentHashMap<EObject, EObject> attributes;
		EReentrantLock attributesLock;
	};

	class DefaultWriteRequestQueue : public EWriteRequestQueue {
	public:
		/**
		 * Default constructor
		 */
		DefaultWriteRequestQueue();

		sp<EWriteRequest> poll(sp<EIoSession> session);
		void offer(sp<EIoSession> session, EWriteRequest* writeRequest);
		void offer(sp<EIoSession> session, sp<EWriteRequest> writeRequest);
		boolean isEmpty(sp<EIoSession> session);
		void clear(sp<EIoSession> session);
		void dispose(sp<EIoSession> session);
		int size();

		virtual EString toString();

	private:
		/** A queue to store incoming write requests */
		EConcurrentLiteQueue<EWriteRequest> q;// = new ConcurrentLinkedQueue<WriteRequest>();
	};
};

} /* namespace eio */
} /* namespace efc */
#endif /* DEFAULTIOSESSIONDATASTRUCTUREFACTORY_HH_ */
