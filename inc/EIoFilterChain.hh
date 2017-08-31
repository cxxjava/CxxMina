/*
 * EIoFilterChain.hh
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#ifndef EIOFILTERCHAIN_HH_
#define EIOFILTERCHAIN_HH_

#include "./EIoFilter.hh"
#include "./EThrowableObject.hh"

namespace efc {
namespace eio {

/**
 * A container of {@link IoFilter}s that forwards {@link IoHandler} events
 * to the consisting filters and terminal {@link IoHandler} sequentially.
 * Every {@link IoSession} has its own {@link IoFilterChain} (1-to-1 relationship).
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

interface EIoFilterChain: virtual public EObject {
	/**
	 * Represents a name-filter pair that an {@link IoFilterChain} contains.
	 *
	 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
	 */
	interface Entry: virtual public EObject {
		/**
		 * @return the name of the filter.
		 */
		virtual const char* getName() = 0;

		/**
		 * @return the filter.
		 */
		virtual EIoFilter* getFilter() = 0;

		/**
		 * @return The {@link NextFilter} of the filter.
		 */
		virtual EIoFilter::NextFilter* getNextFilter() = 0;

		/**
		 * Adds the specified filter with the specified name just before this entry.
		 *
		 * @param name The Filter's name
		 * @param filter The added Filter
		 */
		virtual void addBefore(const char* name, EIoFilter* filter) = 0;

		/**
		 * Adds the specified filter with the specified name just after this entry.
		 *
		 * @param name The Filter's name
		 * @param filter The added Filter
		 */
		virtual void addAfter(const char* name, EIoFilter* filter) = 0;

		/**
		 * Replace the filter of this entry with the specified new filter.
		 *
		 * @param newFilter The new filter that will be put in the chain
		 */
		virtual void replace(EIoFilter* newFilter) = 0;

		/**
		 * Removes this entry from the chain it belongs to.
		 */
		virtual void remove() = 0;
	};

	virtual ~EIoFilterChain() {
	}

	/**
	 * Returns the parent {@link IoSession} of this chain.
	 *
	 * @return {@link IoSession}
	 */
	virtual sp<EIoSession> getSession() = 0;

	/**
	 * Returns the {@link Entry} with the specified <tt>name</tt> in this chain.
	 *
	 * @param name The filter's name we are looking for
	 * @return <tt>null</tt> if there's no such name in this chain
	 */
	virtual Entry* getEntry(const char* name) = 0;

	/**
	 * Returns the {@link Entry} with the specified <tt>filter</tt> in this chain.
	 *
	 * @param filter  The Filter we are looking for
	 * @return <tt>null</tt> if there's no such filter in this chain
	 */
	virtual Entry* getEntry(EIoFilter* filter) = 0;

	/**
	 * Returns the {@link IoFilter} with the specified <tt>name</tt> in this chain.
	 *
	 * @param name the filter's name
	 * @return <tt>null</tt> if there's no such name in this chain
	 */
	virtual EIoFilter* get(const char* name) = 0;

	/**
	 * Returns the {@link NextFilter} of the {@link IoFilter} with the
	 * specified <tt>name</tt> in this chain.
	 *
	 * @param name The filter's name we want the next filter
	 * @return <tt>null</tt> if there's no such name in this chain
	 */
	virtual EIoFilter::NextFilter* getNextFilter(const char* name) = 0;

	/**
	 * Returns the {@link NextFilter} of the specified {@link IoFilter}
	 * in this chain.
	 *
	 * @param filter The filter for which we want the next filter
	 * @return <tt>null</tt> if there's no such name in this chain
	 */
	virtual EIoFilter::NextFilter* getNextFilter(EIoFilter* filter) = 0;

	/**
	 * @param name The filter's name we are looking for
	 *
	 * @return <tt>true</tt> if this chain contains an {@link IoFilter} with the
	 * specified <tt>name</tt>.
	 */
	virtual boolean contains(const char* name) = 0;

	/**
	 * @param filter The filter we are looking for
	 *
	 * @return <tt>true</tt> if this chain contains the specified <tt>filter</tt>.
	 */
	virtual boolean contains(EIoFilter* filter) = 0;

	/**
	 * Adds the specified filter with the specified name at the beginning of this chain.
	 *
	 * @param name The filter's name
	 * @param filter The filter to add
	 */
	virtual void addFirst(const char* name, EIoFilter* filter) = 0;

	/**
	 * Adds the specified filter with the specified name at the end of this chain.
	 *
	 * @param name The filter's name
	 * @param filter The filter to add
	 */
	virtual void addLast(const char* name, EIoFilter* filter) = 0;

	/**
	 * Adds the specified filter with the specified name just before the filter whose name is
	 * <code>baseName</code> in this chain.
	 *
	 * @param baseName The targeted Filter's name
	 * @param name The filter's name
	 * @param filter The filter to add
	 */
	virtual void addBefore(const char* baseName, const char* name, EIoFilter* filter) = 0;

	/**
	 * Adds the specified filter with the specified name just after the filter whose name is
	 * <code>baseName</code> in this chain.
	 *
	 * @param baseName The targeted Filter's name
	 * @param name The filter's name
	 * @param filter The filter to add
	 */
	virtual void addAfter(const char* baseName, const char* name,
			EIoFilter* filter) = 0;

	/**
	 * Replace the filter with the specified name with the specified new
	 * filter.
	 *
	 * @param name The name of the filter we want to replace
	 * @param newFilter The new filter
	 * @return The replaced filter
	 */
	virtual EIoFilter* replace(const char* name, EIoFilter* newFilter) = 0;

	/**
	 * Replace the filter with the specified name with the specified new
	 * filter.
	 *
	 * @param oldFilter The filter we want to replace
	 * @param newFilter The new filter
	 */
	virtual void replace(EIoFilter* oldFilter, EIoFilter* newFilter) = 0;

	/**
	 * Removes the filter with the specified name from this chain.
	 *
	 * @param name The name of the filter to remove
	 * @return The removed filter
	 */
	virtual EIoFilter* remove(const char* name) = 0;

	/**
	 * Replace the filter with the specified name with the specified new
	 * filter.
	 *
	 * @param name The filter to remove
	 */
	virtual void remove(EIoFilter* filter) = 0;

	/**
	 * Removes all filters added to this chain.
	 *
	 * @throws Exception If we weren't able to clear the filters
	 */
	virtual void clear() THROWS(EException) = 0;

	/**
	 * Fires a {@link IoHandler#sessionCreated(IoSession)} event. Most users don't need to
	 * call this method at all. Please use this method only when you implement a new transport
	 * or fire a virtual event.
	 */
	virtual void fireSessionCreated() = 0;

	/**
	 * Fires a {@link IoHandler#sessionOpened(IoSession)} event. Most users don't need to call
	 * this method at all. Please use this method only when you implement a new transport or
	 * fire a virtual event.
	 */
	virtual void fireSessionOpened() = 0;

	/**
	 * Fires a {@link IoHandler#sessionClosed(IoSession)} event. Most users don't need to call
	 * this method at all. Please use this method only when you implement a new transport or
	 * fire a virtual event.
	 */
	virtual void fireSessionClosed() = 0;

	/**
	 * Fires a {@link IoHandler#sessionIdle(IoSession, IdleStatus)} event. Most users don't
	 * need to call this method at all. Please use this method only when you implement a new
	 * transport or fire a virtual event.
	 *
	 * @param status The current status to propagate
	 */
	virtual void fireSessionIdle(EIdleStatus status) = 0;

	/**
	 * Fires a {@link IoHandler#messageReceived(Object)} event. Most users don't need to
	 * call this method at all. Please use this method only when you implement a new transport
	 * or fire a virtual event.
	 *
	 * @param message The received message
	 */
	virtual void fireMessageReceived(sp<EObject>& message) = 0;

	/**
	 * Fires a {@link IoHandler#messageSent(IoSession)} event. Most users don't need to call
	 * this method at all. Please use this method only when you implement a new transport or
	 * fire a virtual event.
	 *
	 * @param request The sent request
	 */
	virtual void fireMessageSent(sp<EWriteRequest>& request) = 0;

	/**
	 * Fires a {@link IoHandler#exceptionCaught(IoSession, Throwable)} event. Most users don't
	 * need to call this method at all. Please use this method only when you implement a new
	 * transport or fire a virtual event.
	 *
	 * @param cause The exception cause
	 */
	virtual void fireExceptionCaught(sp<EThrowableType> cause) = 0;

	/**
	 * Fires a {@link IoHandler#inputClosed(IoSession)} event. Most users don't
	 * need to call this method at all. Please use this method only when you
	 * implement a new transport or fire a virtual event.
	 */
	virtual void fireInputClosed() = 0;

	/**
	 * Fires a {@link IoSession#write(Object)} event. Most users don't need to call this
	 * method at all. Please use this method only when you implement a new transport or fire a
	 * virtual event.
	 *
	 * @param writeRequest The message to write
	 */
	virtual void fireFilterWrite(sp<EWriteRequest>& writeRequest) = 0;

	/**
	 * Fires a {@link IoSession#close()} event. Most users don't need to call this method at
	 * all. Please use this method only when you implement a new transport or fire a virtual
	 * event.
	 */
	virtual void fireFilterClose() = 0;
};

}
} // namespace efc::eio

#endif /* EIOFILTERCHAIN_HH_ */

