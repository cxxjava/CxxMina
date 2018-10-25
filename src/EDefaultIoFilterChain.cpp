/*
 * EDefaultIoFilterChain.cpp
 *
 *  Created on: 2016-1-19
 *      Author: cxxjava@163.com
 */

#include "../inc/EDefaultIoFilterChain.hh"
#include "../inc/EIoFilterLifeCycleException.hh"
#include "../inc/EIoFilterAdapter.hh"
#include "../inc/EThrowableObject.hh"
#include "../inc/EIoBuffer.hh"
#include "../inc/EWriteFuture.hh"
#include "../inc/EConnectFuture.hh"
#include "../inc/EIoHandler.hh"
#include "../inc/EAbstractIoService.hh"
#include "../inc/EIoFilterChain.hh"

namespace efc {
namespace eio {

sp<ELogger> EDefaultIoFilterChain::LOGGER = ELoggerManager::getLogger("EDefaultIoFilterChain");
sp<EAttributeKey> EDefaultIoFilterChain::SESSION_CREATED_FUTURE = new EAttributeKey(typeid(EDefaultIoFilterChain).name(), "connectFuture");

class HeadFilter : public EIoFilterAdapter {
public:
	virtual void filterWrite(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) {
		sp<EAbstractIoSession> s = dynamic_pointer_cast<EAbstractIoSession>(session);

		// Maintain counters.
		EIoBuffer* buffer = dynamic_cast<EIoBuffer*>(writeRequest->getMessage().get());
		if (buffer) {
			// I/O processor implementation will call buffer.reset()
			// it after the write operation is finished, because
			// the buffer will be specified with messageSent event.
			buffer->mark();
			int remaining = buffer->remaining();

			if (remaining > 0) {
				s->increaseScheduledWriteBytes(remaining);
			}
		} else {
			s->increaseScheduledWriteMessages();
		}

		EWriteRequestQueue* writeRequestQueue = s->getWriteRequestQueue();

		if (!s->isWriteSuspended()) {
			if (writeRequestQueue->isEmpty(session)) {
				// We can write directly the message
				s->getProcessor()->write(session, writeRequest);
			} else {
				s->getWriteRequestQueue()->offer(s, writeRequest);
				s->getProcessor()->flush(session);
			}
		} else {
			s->getWriteRequestQueue()->offer(s, writeRequest);
		}
	}

	virtual void filterClose(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session) {
		sp<EAbstractIoSession> s = dynamic_pointer_cast<EAbstractIoSession>(session);
		s->getProcessor()->remove(session);
	}
};

class TailFilter : public EIoFilterAdapter {
public:
	virtual void sessionCreated(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session) {
//		ON_FINALLY_NOTHROW(
				/** moved to EIoServiceListenerSupport.cpp
				// Notify the related future.
				sp<EConnectFuture> future = dynamic_pointer_cast<EConnectFuture>(
						session->removeAttribute(EDefaultIoFilterChain::SESSION_CREATED_FUTURE.get()));

				if (future != null) {
					EIoServiceListenerSupport* listeners = (dynamic_cast<EAbstractIoService*>(session->getService()))->getListeners();
					sp<EIoSession> s = listeners->getSession(session);
					future->setSession(s);
					future.dismiss();
				}
				*/
//		) {
			session->getHandler()->sessionCreated(session);
//		}}
	}

	virtual void sessionOpened(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session) {
		session->getHandler()->sessionOpened(session);
	}

	virtual void sessionClosed(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session) {
		sp<EAbstractIoSession> s = dynamic_pointer_cast<EAbstractIoSession>(session);

		/* @see:
		try {
			s.getHandler().sessionClosed(session);
		} finally {
			try {
				s.getWriteRequestQueue().dispose(session);
			} finally {
				try {
					s.getAttributeMap().dispose(session);
				} finally {
					try {
						// Remove all filters.
						session.getFilterChain().clear();
					} finally {
						if (s.getConfig().isUseReadOperation()) {
							s.offerClosedReadFuture();
						}
					}
				}
			}
		}
		*/
		ON_FINALLY_NOTHROW(
			ON_FINALLY_NOTHROW(
				ON_FINALLY_NOTHROW(
					ON_FINALLY_NOTHROW(
						if (s->getConfig()->isUseReadOperation()) {
							s->offerClosedReadFuture();
						}
					) {
						session->getFilterChain()->clear();
                    }}
				) {
					s->getAttributeMap()->dispose(session);
                }}
			) {
				s->getWriteRequestQueue()->dispose(session);
            }}
		) {
			s->getHandler()->sessionClosed(session);
        }}
	}

	virtual void sessionIdle(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, EIdleStatus status) {
		session->getHandler()->sessionIdle(session, status);
	}

	virtual void exceptionCaught(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, sp<EThrowableType>& cause) {
		sp<EAbstractIoSession> s = dynamic_pointer_cast<EAbstractIoSession>(session);

		ON_FINALLY_NOTHROW(
			if (s->getConfig()->isUseReadOperation()) {
				s->offerFailedReadFuture(cause.dismiss());
			}
		) {
			s->getHandler()->exceptionCaught(session, cause); //!
        }}
	}

	virtual void inputClosed(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session) {
		session->getHandler()->inputClosed(session);
	}

	virtual void messageReceived(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, sp<EObject>& message) {
		sp<EAbstractIoSession> s = dynamic_pointer_cast<EAbstractIoSession>(session);

		EIoBuffer* buffer = dynamic_cast<EIoBuffer*>(message.get());
		if (!buffer) {
			s->increaseReadMessages(ESystem::currentTimeMillis());
		} else if (!buffer->hasRemaining()) {
			s->increaseReadMessages(ESystem::currentTimeMillis());
		}

		// Update the statistics
		EAbstractIoService* service = dynamic_cast<EAbstractIoService*>(session->getService());
		if (service) {
			service->getStatistics()->updateThroughput(ESystem::currentTimeMillis());
		}

		// Propagate the message
		ON_FINALLY_NOTHROW(
			if (s->getConfig()->isUseReadOperation()) {
				s->offerReadFuture(message); //!!!
			}
		) {
			session->getHandler()->messageReceived(session, message);
        }}
	}

	virtual void messageSent(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) {
		sp<EAbstractIoSession> s = dynamic_pointer_cast<EAbstractIoSession>(session);
		s->increaseWrittenMessages(writeRequest, ESystem::currentTimeMillis());

		// Update the statistics
		EAbstractIoService* service = dynamic_cast<EAbstractIoService*>(session->getService());
		if (service) {
			service->getStatistics()->updateThroughput(ESystem::currentTimeMillis());
		}

		// Propagate the message
        sp<EObject> message = writeRequest->getMessage();
		session->getHandler()->messageSent(session, message);
	}

	virtual void filterClose(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session) {
		nextFilter->filterClose(session);
	}

	virtual void filterWrite(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) {
		nextFilter->filterWrite(session, writeRequest);
	}
};

EDefaultIoFilterChain::~EDefaultIoFilterChain() {
	delete head->getFilter(); //!
	delete head;
	delete tail->getFilter(); //!
	delete tail;
	delete name2entry;
}

EDefaultIoFilterChain::EDefaultIoFilterChain(const sp<EIoSession> session) {
	if (session == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "session");
	}

	this->session = session;
	head = new EntryImpl(null, null, "head", new HeadFilter(), this);
	tail = new EntryImpl(head, null, "tail", new TailFilter(), this);
	head->nextEntry = tail;

	name2entry = new EHashMap<EString*, EntryImpl*>();
}

sp<EIoSession> EDefaultIoFilterChain::getSession() {
	return session.lock();
}

EIoFilterChain::Entry* EDefaultIoFilterChain::getEntry(const char* name) {
	EString ns(name);
	return name2entry->get(&ns);
}

EIoFilterChain::Entry* EDefaultIoFilterChain::getEntry(EIoFilter* filter) {
	EntryImpl* e = head->nextEntry;

	while (e != tail) {
		if (e->getFilter() == filter) {
			return e;
		}

		e = e->nextEntry;
	}

	return null;
}

EIoFilter* EDefaultIoFilterChain::get(const char* name) {
	EIoFilterChain::Entry* e = getEntry(name);

	if (e == null) {
		return null;
	}

	return e->getFilter();
}

EIoFilter::NextFilter* EDefaultIoFilterChain::getNextFilter(const char* name) {
	EIoFilterChain::Entry* e = getEntry(name);

	if (e == null) {
		return null;
	}

	return e->getNextFilter();
}

EIoFilter::NextFilter* EDefaultIoFilterChain::getNextFilter(EIoFilter* filter) {
	EIoFilterChain::Entry* e = getEntry(filter);
	if (e == null) {
		return null;
	}
	return e->getNextFilter();
}

void EDefaultIoFilterChain::addFirst(const char* name, EIoFilter* filter) {
	SYNCHRONIZED(this) {
		checkAddable(name);
		register_(head, name, filter);
    }}
}

void EDefaultIoFilterChain::addLast(const char* name, EIoFilter* filter) {
	SYNCHRONIZED(this) {
		checkAddable(name);
		register_(tail->prevEntry, name, filter);
    }}
}

void EDefaultIoFilterChain::addBefore(const char* baseName, const char* name,
		EIoFilter* filter) {
	SYNCHRONIZED(this) {
		EntryImpl* baseEntry = checkOldName(baseName);
		checkAddable(name);
		register_(baseEntry->prevEntry, name, filter);
    }}
}

void EDefaultIoFilterChain::addAfter(const char* baseName, const char* name,
		EIoFilter* filter) {
	SYNCHRONIZED(this) {
		EntryImpl* baseEntry = checkOldName(baseName);
		checkAddable(name);
		register_(baseEntry, name, filter);
    }}
}

EIoFilter* EDefaultIoFilterChain::remove(const char* name) {
	SYNCHRONIZED(this) {
		EntryImpl* entry = checkOldName(name);
		deregister(entry);
		EIoFilter* filter = entry->getFilter();
		delete entry; //!
		return filter;
    }}
}

void EDefaultIoFilterChain::remove(EIoFilter* filter) {
	SYNCHRONIZED(this) {
		EntryImpl* e = head->nextEntry;

		while (e != tail) {
			if (e->getFilter() == filter) {
				deregister(e);
				delete e; //!

				return;
			}

			e = e->nextEntry;
		}

		EString msg("Filter not found: ");
		msg += filter->toString();
		throw EIllegalArgumentException(__FILE__, __LINE__, msg.c_str());
    }}
}

EIoFilter* EDefaultIoFilterChain::replace(const char* name,
		EIoFilter* newFilter) {
	SYNCHRONIZED(this) {
		EntryImpl* entry = checkOldName(name);
		EIoFilter* oldFilter = entry->getFilter();

		// Call the preAdd method of the new filter
		try {
			newFilter->onPreAdd(this, name, entry->getNextFilter());
		} catch (EException& e) {
			EString msg("onPreAdd(): ");
			msg += name;
			msg += ":";
			msg += newFilter->toString();
			msg += " in ";
			msg += getSession()->toString();
			throw EIoFilterLifeCycleException(__FILE__, __LINE__, msg.c_str());
		}

		// Now, register the new Filter replacing the old one.
		entry->setFilter(newFilter);

		// Call the postAdd method of the new filter
		try {
			newFilter->onPostAdd(this, name, entry->getNextFilter());
		} catch (EException& e) {
			entry->setFilter(oldFilter);

			EString msg("onPostAdd(): ");
			msg += name;
			msg += ":";
			msg += newFilter->toString();
			msg += " in ";
			msg += getSession()->toString();
			throw EIoFilterLifeCycleException(__FILE__, __LINE__, msg.c_str());
		}

		return oldFilter;
    }}
}

void EDefaultIoFilterChain::replace(EIoFilter* oldFilter,
		EIoFilter* newFilter) {
	SYNCHRONIZED(this) {
		EntryImpl* entry = head->nextEntry;

		// Search for the filter to replace
		while (entry != tail) {
			if (entry->getFilter() == oldFilter) {
				EString* oldFilterName = null;

				// Get the old filter name. It's not really efficient...
				/*
				sp<EIterator<EString*> > iter = name2entry->keySet()->iterator();
				while (iter->hasNext()) {
					EString* name = iter->next();
					if (entry == name2entry->get(name)) {
						oldFilterName = name;

						break;
					}
				}
				*/
				sp<EIterator<EMapEntry<EString*, EntryImpl*>*> > iter = name2entry->entrySet()->iterator();
				while (iter->hashCode()) {
					EMapEntry<EString*, EntryImpl*>* mapping = iter->next();
					if (entry == mapping->getValue()) {
						oldFilterName = mapping->getKey();

						break;
					}
				}

				// Call the preAdd method of the new filter
				try {
					newFilter->onPreAdd(this, oldFilterName->c_str(), entry->getNextFilter());
				} catch (EException& e) {
					EString msg("onPreAdd(): ");
					msg += oldFilterName->c_str();
					msg += ":";
					msg += newFilter->toString();
					msg += " in ";
					msg += getSession()->toString();
					throw EIoFilterLifeCycleException(__FILE__, __LINE__, msg.c_str());
				}

				// Now, register the new Filter replacing the old one.
				entry->setFilter(newFilter);

				// Call the postAdd method of the new filter
				try {
					newFilter->onPostAdd(this, oldFilterName->c_str(), entry->getNextFilter());
				} catch (EException& e) {
					entry->setFilter(oldFilter);
					EString msg("onPostAdd(): ");
					msg += oldFilterName->c_str();
					msg += ":";
					msg += newFilter->toString();
					msg += " in ";
					msg += getSession()->toString();
					throw EIoFilterLifeCycleException(__FILE__, __LINE__, msg.c_str());
				}

				return;
			}

			entry = entry->nextEntry;
		}

		EString msg("Filter not found: ");
		msg += typeid(*oldFilter).name();
		throw EIllegalArgumentException(__FILE__, __LINE__, msg.c_str());
    }}
}

void EDefaultIoFilterChain::clear() {
	SYNCHRONIZED(this) {
		sp<EIterator<EMapEntry<EString*, EntryImpl*>*> > iter = name2entry->entrySet()->iterator();
		while (iter->hasNext()) {
			EMapEntry<EString*, EntryImpl*>* entry = iter->next();
			try {
				EntryImpl* e = entry->getValue();
				deregister(e);
				delete e; //!
			} catch (EException& e) {
				EString msg("clear(): ");
				msg << entry->getValue()->getName() << " in " << getSession()->toString();
				throw EIoFilterLifeCycleException(__FILE__, __LINE__, msg.c_str());
			}
		}
    }}
}

void EDefaultIoFilterChain::fireSessionCreated() {
	sp<EIoSession> s = session.lock();
	callNextSessionCreated(head, s);
}

void EDefaultIoFilterChain::callNextSessionCreated(EIoFilterChain::Entry* entry, sp<EIoSession>& session) {
	try {
		EIoFilter* filter = entry->getFilter();
		EIoFilter::NextFilter* nextFilter = entry->getNextFilter();
		filter->sessionCreated(nextFilter, session);
	} catch (EException& e) {
		fireExceptionCaught(new EThrowableObject<EException>(e));
	}
	/* @see:
	 catch (Error e) {
		fireExceptionCaught(e);
		throw e;
	}
	*/
}

void EDefaultIoFilterChain::fireSessionOpened() {
	sp<EIoSession> s = session.lock();
	callNextSessionOpened(head, s);
}

void EDefaultIoFilterChain::callNextSessionOpened(Entry* entry, sp<EIoSession>& session) {
	try {
		EIoFilter* filter = entry->getFilter();
		EIoFilter::NextFilter* nextFilter = entry->getNextFilter();
		filter->sessionOpened(nextFilter, session);
	} catch (EException& e) {
		fireExceptionCaught(new EThrowableObject<EException>(e));
	}
	/* @see:
	 catch (Error e) {
		fireExceptionCaught(e);
		throw e;
	}
	*/
}

void EDefaultIoFilterChain::fireSessionClosed() {
	sp<EIoSession> s = session.lock();
	// Update future.
	try {
		s->getCloseFuture()->setClosed();
	} catch (EException& e) {
		fireExceptionCaught(new EThrowableObject<EException>(e));
	}
	/* @see:
	 catch (Error e) {
		fireExceptionCaught(e);
		throw e;
	}
	*/

	// And start the chain.
	callNextSessionClosed(head, s);
}

void EDefaultIoFilterChain::callNextSessionClosed(Entry* entry, sp<EIoSession>& session) {
	try {
		EIoFilter* filter = entry->getFilter();
		EIoFilter::NextFilter* nextFilter = entry->getNextFilter();
		filter->sessionClosed(nextFilter, session);
	} catch (EException& e) {
		fireExceptionCaught(new EThrowableObject<EException>(e));
	}
	/* @see:
	 catch (Error e) {
		fireExceptionCaught(e);
		throw e;
	}
	*/
}

void EDefaultIoFilterChain::fireSessionIdle(EIdleStatus status) {
	sp<EIoSession> s = session.lock();
	sp<EAbstractIoSession> as = dynamic_pointer_cast<EAbstractIoSession>(s);
	as->increaseIdleCount(status, ESystem::currentTimeMillis());
	callNextSessionIdle(head, s, status);
}

void EDefaultIoFilterChain::callNextSessionIdle(Entry* entry, sp<EIoSession>& session, EIdleStatus status) {
	try {
		EIoFilter* filter = entry->getFilter();
		EIoFilter::NextFilter* nextFilter = entry->getNextFilter();
		filter->sessionIdle(nextFilter, session, status);
	} catch (EException& e) {
		fireExceptionCaught(new EThrowableObject<EException>(e));
	}
	/* @see:
	 catch (Error e) {
		fireExceptionCaught(e);
		throw e;
	}
	*/
}

void EDefaultIoFilterChain::fireMessageReceived(sp<EObject>& message) {
	sp<EIoSession> s = session.lock();
	EIoBuffer* buf = dynamic_cast<EIoBuffer*>(message.get());
	if (buf) {
		sp<EAbstractIoSession> as = dynamic_pointer_cast<EAbstractIoSession>(s);
		as->increaseReadBytes(buf->remaining(), ESystem::currentTimeMillis());
	}

	callNextMessageReceived(head, s, message);
}

void EDefaultIoFilterChain::callNextMessageReceived(Entry* entry, sp<EIoSession>& session, sp<EObject>& message) {
	try {
		EIoFilter* filter = entry->getFilter();
		EIoFilter::NextFilter* nextFilter = entry->getNextFilter();
		filter->messageReceived(nextFilter, session, message);
	} catch (EException& e) {
		fireExceptionCaught(new EThrowableObject<EException>(e));
	}
	/* @see:
	 catch (Error e) {
		fireExceptionCaught(e);
		throw e;
	}
	*/
}

void EDefaultIoFilterChain::fireMessageSent(sp<EWriteRequest>& request) {
	try {
		request->getFuture()->setWritten();
	} catch (EException& e) {
		fireExceptionCaught(new EThrowableObject<EException>(e));
	}
	/* @see:
	 catch (Error e) {
		fireExceptionCaught(e);
		throw e;
	}
	*/

	if (!request->isEncoded()) {
		sp<EIoSession> s = session.lock();
		callNextMessageSent(head, s, request);
	}
}

void EDefaultIoFilterChain::callNextMessageSent(Entry* entry, sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) {
	try {
		EIoFilter* filter = entry->getFilter();
		EIoFilter::NextFilter* nextFilter = entry->getNextFilter();
		filter->messageSent(nextFilter, session, writeRequest);
	} catch (EException& e) {
		fireExceptionCaught(new EThrowableObject<EException>(e));
	}
	/* @see:
	 catch (Error e) {
		fireExceptionCaught(e);
		throw e;
	}
	*/
}

void EDefaultIoFilterChain::fireExceptionCaught(sp<EThrowableType> cause) {
	sp<EIoSession> s = session.lock();
	sp<EThrowableType> throwable(cause); //!
	callNextExceptionCaught(head, s, throwable);
}

void EDefaultIoFilterChain::callNextExceptionCaught(Entry* entry, sp<EIoSession>& session, sp<EThrowableType>& cause) {
	// Notify the related future.
	sp<EConnectFuture> future = dynamic_pointer_cast<EConnectFuture>(session->removeAttribute(SESSION_CREATED_FUTURE.get()));
	if (future == null) {
		try {
			EIoFilter* filter = entry->getFilter();
			EIoFilter::NextFilter* nextFilter = entry->getNextFilter();
			filter->exceptionCaught(nextFilter, session, cause);
		} catch (EThrowable& e) {
			LOGGER->warn(__FILE__, __LINE__, "Unexpected exception from exceptionCaught handler.");
		}
	} else {
		// Please note that this place is not the only place that
		// calls ConnectFuture.setException().
		if (!session->isClosing()) {
			// Call the closeNow method only if needed
			session->closeNow();
		}
		future->setException(cause.dismiss()); //!
	}
}

void EDefaultIoFilterChain::fireInputClosed() {
	Entry* head = this->head;
	sp<EIoSession> s = session.lock();
	callNextInputClosed(head, s);
}

void EDefaultIoFilterChain::callNextInputClosed(Entry* entry, sp<EIoSession>& session) {
	try {
		EIoFilter* filter = entry->getFilter();
		EIoFilter::NextFilter* nextFilter = entry->getNextFilter();
		filter->inputClosed(nextFilter, session);
	} catch (EThrowable& e) {
		fireExceptionCaught(new EThrowableObject<EThrowable>(e));
	}
}

void EDefaultIoFilterChain::fireFilterWrite(sp<EWriteRequest>& writeRequest) {
	sp<EIoSession> s = session.lock();
	callPreviousFilterWrite(tail, s, writeRequest);
}

void EDefaultIoFilterChain::callPreviousFilterWrite(Entry* entry, sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) {
	try {
		EIoFilter* filter = entry->getFilter();
		EIoFilter::NextFilter* nextFilter = entry->getNextFilter();
		filter->filterWrite(nextFilter, session, writeRequest);
	} catch (EException& e) {
		writeRequest->getFuture()->setException(new EThrowableObject<EException>(e));
		fireExceptionCaught(new EThrowableObject<EException>(e));
	}
	/* @see:
	catch (Error e) {
		writeRequest.getFuture().setException(e);
		fireExceptionCaught(e);
		throw e;
	}
	*/
}

void EDefaultIoFilterChain::fireFilterClose() {
	sp<EIoSession> s = session.lock();
	callPreviousFilterClose(tail, s);
}

void EDefaultIoFilterChain::callPreviousFilterClose(Entry* entry, sp<EIoSession>& session) {
	try {
		EIoFilter* filter = entry->getFilter();
		EIoFilter::NextFilter* nextFilter = entry->getNextFilter();
		filter->filterClose(nextFilter, session);
	} catch (EException& e) {
		fireExceptionCaught(new EThrowableObject<EException>(e));
	}
	/* @see:
	catch (Error e) {
		fireExceptionCaught(e);
		throw e;
	}
	*/
}

EArrayList<EIoFilterChain::Entry*> EDefaultIoFilterChain::getAll() {
	EArrayList<Entry*> list(false);
	EntryImpl* e = head->nextEntry;

	while (e != tail) {
		list.add(e);
		e = e->nextEntry;
	}

	return list;
}

EArrayList<EIoFilterChain::Entry*> EDefaultIoFilterChain::getAllReversed() {
	EArrayList<Entry*> list(false);
	EntryImpl* e = tail->prevEntry;

	while (e != head) {
		list.add(e);
		e = e->prevEntry;
	}

	return list;
}

boolean EDefaultIoFilterChain::contains(const char* name) {
	return getEntry(name) != null;
}

boolean EDefaultIoFilterChain::contains(EIoFilter* filter) {
	return getEntry(filter) != null;
}

EString EDefaultIoFilterChain::toString() {
	EString buf("{ ");

	boolean empty = true;

	EntryImpl* e = head->nextEntry;

	while (e != tail) {
		if (!empty) {
			buf.append(", ");
		} else {
			empty = false;
		}

		buf.append('(');
		buf.append(e->getName());
		buf.append(':');
		buf.append(e->getFilter()->toString());
		buf.append(')');

		e = e->nextEntry;
	}

	if (empty) {
		buf.append("empty");
	}

	buf.append(" }");

	return buf;
}

void EDefaultIoFilterChain::checkAddable(const char* name) {
    EString ns(name);
	if (name2entry->containsKey(&ns)) {
		EString msg("Other filter is using the same name '");
		msg += name;
		msg += "'";
		throw EIllegalArgumentException(__FILE__, __LINE__, msg.c_str());
	}
}

void EDefaultIoFilterChain::register_(EntryImpl* prevEntry, const char* name, EIoFilter* filter) {
	EntryImpl* newEntry = new EntryImpl(prevEntry, prevEntry->nextEntry, name, filter, this);

	try {
		filter->onPreAdd(this, name, newEntry->getNextFilter());
	} catch (EException& e) {
		delete newEntry; //!

		EString msg("onPreAdd(): ");
		msg += name;
		msg += ":";
		msg += filter->toString();
		msg += " in ";
		msg += getSession()->toString();
		throw EIoFilterLifeCycleException(__FILE__, __LINE__, msg.c_str());
	}

	prevEntry->nextEntry->prevEntry = newEntry;
	prevEntry->nextEntry = newEntry;
	delete name2entry->put(new EString(name), newEntry);

	try {
		filter->onPostAdd(this, name, newEntry->getNextFilter());
	} catch (EException& e) {
		deregister0(newEntry);
		delete newEntry; //!

		EString msg("onPostAdd(): ");
		msg += name;
		msg += ":";
		msg += filter->toString();
		msg += " in ";
		msg += getSession()->toString();
		throw EIoFilterLifeCycleException(__FILE__, __LINE__, msg.c_str());
	}
}

void EDefaultIoFilterChain::deregister(EntryImpl* entry) {
	EIoFilter* filter = entry->getFilter();

	try {
		filter->onPreRemove(this, entry->getName(), entry->getNextFilter());
	} catch (EException& e) {
		EString msg("onPreRemove(): ");
		msg << entry->getName() << ':' << filter->toString() << " in " <<  getSession()->toString();
		throw EIoFilterLifeCycleException(__FILE__, __LINE__, msg.c_str());
	}

	deregister0(entry);

	try {
		filter->onPostRemove(this, entry->getName(), entry->getNextFilter());
	} catch (EException& e) {
		EString msg("onPostRemove(): ");
		msg << entry->getName() << ':' << filter->toString() << " in " <<  getSession()->toString();
		throw EIoFilterLifeCycleException(__FILE__, __LINE__, msg.c_str());
	}
}

void EDefaultIoFilterChain::deregister0(EntryImpl* entry) {
	EntryImpl* prevEntry = entry->prevEntry;
	EntryImpl* nextEntry = entry->nextEntry;
	prevEntry->nextEntry = nextEntry;
	nextEntry->prevEntry = prevEntry;

    EString ns(entry->name);
	name2entry->remove(&ns); //delay to free the entry!
}

EDefaultIoFilterChain::EntryImpl* EDefaultIoFilterChain::checkOldName(const char* baseName) {
	EString ns(baseName);
	EntryImpl* e = dynamic_cast<EntryImpl*>(name2entry->get(&ns));

	if (e == null) {
		EString msg("Filter not found:");
		msg += baseName;
		throw EIllegalArgumentException(__FILE__, __LINE__, msg.c_str());
	}

	return e;
}

//EntryImpl

EDefaultIoFilterChain::EntryImpl::~EntryImpl() {
	delete nextFilter;
}

EDefaultIoFilterChain::EntryImpl::EntryImpl(
		EDefaultIoFilterChain::EntryImpl* prevEntry,
		EDefaultIoFilterChain::EntryImpl* nextEntry, const char* name,
		EIoFilter* filter, EDefaultIoFilterChain* difc) {
	if (filter == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "filter");
	}

	if (name == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "name");
	}

	this->prevEntry = prevEntry;
	this->nextEntry = nextEntry;
	this->name = name;
	this->filter = filter;
	this->owner = difc;

	class _NextFilter: public EIoFilter::NextFilter {
	private:
		EntryImpl* ei;
		EDefaultIoFilterChain* ifc;
	public:
		_NextFilter(EntryImpl* e, EDefaultIoFilterChain* f): ei(e), ifc(f) {
		}

		virtual ~_NextFilter() {
		}

		virtual void sessionCreated(sp<EIoSession>& session) {
			Entry* nextEntry = ei->nextEntry;
			ifc->callNextSessionCreated(nextEntry, session);
		}

		virtual void sessionOpened(sp<EIoSession>& session) {
			Entry* nextEntry = ei->nextEntry;
			ifc->callNextSessionOpened(nextEntry, session);
		}

		virtual void sessionClosed(sp<EIoSession>& session) {
			Entry* nextEntry = ei->nextEntry;
			ifc->callNextSessionClosed(nextEntry, session);
		}

		virtual void sessionIdle(sp<EIoSession>& session, EIdleStatus status) {
			Entry* nextEntry = ei->nextEntry;
			ifc->callNextSessionIdle(nextEntry, session, status);
		}

		virtual void exceptionCaught(sp<EIoSession>& session, sp<EThrowableType>& cause) {
			Entry* nextEntry = ei->nextEntry;
			ifc->callNextExceptionCaught(nextEntry, session, cause);
		}

		virtual void inputClosed(sp<EIoSession>& session) {
			Entry* nextEntry = ei->nextEntry;
			ifc->callNextInputClosed(nextEntry, session);
		}

		virtual void messageReceived(sp<EIoSession>& session, sp<EObject>& message) {
			Entry* nextEntry = ei->nextEntry;
			ifc->callNextMessageReceived(nextEntry, session, message);
		}

		void messageSent(sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) {
			Entry* nextEntry = ei->nextEntry;
			ifc->callNextMessageSent(nextEntry, session, writeRequest);
		}

		virtual void filterWrite(sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) {
			Entry* nextEntry = ei->prevEntry;
			ifc->callPreviousFilterWrite(nextEntry, session, writeRequest);
		}

		virtual void filterClose(sp<EIoSession>& session) {
			Entry* nextEntry = ei->prevEntry;
			ifc->callPreviousFilterClose(nextEntry, session);
		}

		virtual EString toString() {
			return ei->nextEntry->name;
		}
	};

	this->nextFilter = new _NextFilter(this, difc);
}

const char* EDefaultIoFilterChain::EntryImpl::getName() {
	return name.c_str();
}

EIoFilter* EDefaultIoFilterChain::EntryImpl::getFilter() {
	return filter;
}

EIoFilter::NextFilter* EDefaultIoFilterChain::EntryImpl::getNextFilter() {
	return nextFilter;
}

void EDefaultIoFilterChain::EntryImpl::addBefore(const char* name,
EIoFilter* filter) {
	owner->addBefore(getName(), name, filter);
}

void EDefaultIoFilterChain::EntryImpl::addAfter(const char* name,
EIoFilter* filter) {
	owner->addAfter(getName(), name, filter);
}

void EDefaultIoFilterChain::EntryImpl::replace(EIoFilter* newFilter) {
	owner->replace(getName(), newFilter);
}

void EDefaultIoFilterChain::EntryImpl::remove() {
	owner->remove(getName());
}

EString EDefaultIoFilterChain::EntryImpl::toString() {
	EString sb;

	// Add the current filter
	sb.append("('").append(getName()).append('\'');

	// Add the previous filter
	sb.append(", prev: '");

	if (prevEntry != null) {
		sb.append(prevEntry->name);
		sb.append(':');
		sb.append(typeid(prevEntry->getFilter()).name());
	} else {
		sb.append("null");
	}

	// Add the next filter
	sb.append("', next: '");

	if (nextEntry != null) {
		sb.append(nextEntry->name);
		sb.append(':');
		sb.append(typeid(nextEntry->getFilter()).name());
	} else {
		sb.append("null");
	}

	sb.append("')");

	return sb;
}

void EDefaultIoFilterChain::EntryImpl::setFilter(EIoFilter* filter) {
	if (filter == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "filter");
	}

	delete this->filter; //!
	this->filter = filter;
}

} /* namespace eio */
} /* namespace efc */
