/*
 * EDefaultIoFilterChain.hh
 *
 *  Created on: 2016-1-19
 *      Author: cxxjava@163.com
 */

#ifndef EDEFAULTIOFILTERCHAIN_HH_
#define EDEFAULTIOFILTERCHAIN_HH_

#include "./EIoFilterChain.hh"
#include "./ENioSession.hh"
#include "./EAttributeKey.hh"
#include "ELog.hh"

namespace efc {
namespace eio {

/**
 * A default implementation of {@link IoFilterChain} that provides
 * all operations for developers who want to implement their own
 * transport layer once used with {@link AbstractIoSession}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EDefaultIoFilterChain: virtual public EIoFilterChain, public ESynchronizeable {
public:
	/**
	 * A session attribute that stores an {@link IoFuture} related with
	 * the {@link IoSession}.  {@link DefaultIoFilterChain} clears this
	 * attribute and notifies the future when {@link #fireSessionCreated()}
	 * or {@link #fireExceptionCaught(Throwable)} is invoked.
	 */
	static sp<EAttributeKey> SESSION_CREATED_FUTURE;// = new AttributeKey(DefaultIoFilterChain.class, "connectFuture");

public:
	virtual ~EDefaultIoFilterChain();

	/**
	 * Create a new default chain, associated with a session. It will only contain a
	 * HeadFilter and a TailFilter.
	 *
	 * @param session The session associated with the created filter chain
	 */
	EDefaultIoFilterChain(const sp<EIoSession> session);

	virtual sp<EIoSession> getSession();

	virtual Entry* getEntry(const char* name);

	virtual Entry* getEntry(EIoFilter* filter);

	virtual EIoFilter* get(const char* name);

	virtual EIoFilter::NextFilter* getNextFilter(const char* name);

	virtual EIoFilter::NextFilter* getNextFilter(EIoFilter* filter);

	virtual synchronized void addFirst(const char* name, EIoFilter* filter);
	virtual synchronized void addLast(const char* name, EIoFilter* filter);
	virtual synchronized void addBefore(const char* baseName, const char* name, EIoFilter* filter);
	virtual synchronized void addAfter(const char* baseName, const char* name, EIoFilter* filter);
	virtual synchronized EIoFilter* remove(const char* name);
	virtual synchronized void remove(EIoFilter* filter);
	virtual synchronized EIoFilter* replace(const char* name, EIoFilter* newFilter);
	virtual synchronized void replace(EIoFilter* oldFilter, EIoFilter* newFilter);
	virtual synchronized void clear() THROWS(EException);

	virtual void fireSessionCreated();
	virtual void fireSessionOpened();
	virtual void fireSessionClosed();
	virtual void fireSessionIdle(EIdleStatus status);
	virtual void fireMessageReceived(sp<EObject>& message);
	virtual void fireMessageSent(sp<EWriteRequest>& request);
	virtual void fireExceptionCaught(sp<EThrowableType> cause);
	virtual void fireInputClosed();
	virtual void fireFilterWrite(sp<EWriteRequest>& writeRequest);
	virtual void fireFilterClose();

	EArrayList<Entry*> getAll();
	EArrayList<Entry*> getAllReversed();

	virtual boolean contains(const char* name);
	virtual boolean contains(EIoFilter* filter);

	virtual EString toString();

private:
	class EntryImpl : public virtual Entry {
	public:
		~EntryImpl();
		EntryImpl(EntryImpl* prevEntry, EntryImpl* nextEntry, const char* name,
				EIoFilter* filter, EDefaultIoFilterChain* difc);
		virtual const char* getName();
		virtual EIoFilter* getFilter();
		virtual EIoFilter::NextFilter* getNextFilter();
		virtual void addBefore(const char* name, EIoFilter* filter);
		virtual void addAfter(const char* name, EIoFilter* filter);
		virtual void replace(EIoFilter* newFilter);
		virtual void remove();
		virtual EString toString();

		void setFilter(EIoFilter* filter);
	public:
		EntryImpl* prevEntry;
		EntryImpl* nextEntry;
		EString name;
		EIoFilter* filter;
		EIoFilter::NextFilter* nextFilter;
		EDefaultIoFilterChain* owner;
	};

private:
	/** The associated session */
	wp<EIoSession> session;

	/** The mapping between the filters and their associated name */
	//cxxjava: name2entry use HashMap at mina-2.0.0-RC1 and use ConcurrentHashMap at later versions.
	EMap<EString*, EntryImpl*>* name2entry;// = new ConcurrentHashMap<String, Entry>();

	/** The chain head */
	EntryImpl* head;

	/** The chain tail */
	EntryImpl* tail;

	/** The logger for this class */
	static sp<ELogger> LOGGER;// = LoggerFactory.getLogger(DefaultIoFilterChain.class);

	/**
	 * Checks the specified filter name is already taken and throws an exception if already taken.
	 */
	void checkAddable(const char* name);

	/**
	 * Register the newly added filter, inserting it between the previous and
	 * the next filter in the filter's chain. We also call the preAdd and
	 * postAdd methods.
	 */
	void register_(EntryImpl* prevEntry, const char* name, EIoFilter* filter);

	void deregister(EntryImpl* entry);
	void deregister0(EntryImpl* entry);

	/**
	 * Throws an exception when the specified filter name is not registered in this chain.
	 *
	 * @return An filter entry with the specified name.
	 */
	EntryImpl* checkOldName(const char* baseName);

	void callNextSessionCreated(Entry* entry, sp<EIoSession>& session);
	void callNextSessionOpened(Entry* entry, sp<EIoSession>& session);
	void callNextSessionClosed(Entry* entry, sp<EIoSession>& session);
	void callNextSessionIdle(Entry* entry, sp<EIoSession>& session, EIdleStatus status);
	void callNextMessageReceived(Entry* entry, sp<EIoSession>& session, sp<EObject>& message);
	void callNextMessageSent(Entry* entry, sp<EIoSession>& session, sp<EWriteRequest>& writeRequest);
	void callNextExceptionCaught(Entry* entry, sp<EIoSession>& session, sp<EThrowableType>& cause);
	void callNextInputClosed(Entry* entry, sp<EIoSession>& session);
	void callPreviousFilterWrite(Entry* entry, sp<EIoSession>& session, sp<EWriteRequest>& writeRequest);
	void callPreviousFilterClose(Entry* entry, sp<EIoSession>& session);
};

} /* namespace eio */
} /* namespace efc */
#endif /* EDEFAULTIOFILTERCHAIN_HH_ */
