/*
 * EIoFilter.hh
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#ifndef EIOFILTER_HH_
#define EIOFILTER_HH_

#include "./EIoSession.hh"
#include "./EWriteRequest.hh"
#include "Efc.hh"

namespace efc {
namespace eio {
    
interface EIoFilterChain;

/**
 * A filter which intercepts {@link IoHandler} events like Servlet
 * filters.  Filters can be used for these purposes:
 * <ul>
 *   <li>Event logging,</li>
 *   <li>Performance measurement,</li>
 *   <li>Authorization,</li>
 *   <li>Overload control,</li>
 *   <li>Message transformation (e.g. encryption and decryption, ...),</li>
 *   <li>and many more.</li>
 * </ul>
 * <p>
 * <strong>Please NEVER implement your filters to wrap
 * {@link IoSession}s.</strong> Users can cache the reference to the
 * session, which might malfunction if any filters are added or removed later.
 *
 * <h3>The Life Cycle</h3>
 * {@link IoFilter}s are activated only when they are inside {@link IoFilterChain}.
 * <p>
 * When you add an {@link IoFilter} to an {@link IoFilterChain}:
 * <ol>
 *   <li>{@link #init()} is invoked by {@link ReferenceCountingFilter} if
 *       the filter is added at the first time.</li>
 *   <li>{@link #onPreAdd(IoFilterChain, String, NextFilter)} is invoked to notify
 *       that the filter will be added to the chain.</li>
 *   <li>The filter is added to the chain, and all events and I/O requests
 *       pass through the filter from now.</li>
 *   <li>{@link #onPostAdd(IoFilterChain, String, NextFilter)} is invoked to notify
 *       that the filter is added to the chain.</li>
 *   <li>The filter is removed from the chain if {@link #onPostAdd(IoFilterChain, String, org.apache.mina.core.filterchain.IoFilter.NextFilter)}
 *       threw an exception.  {@link #destroy()} is also invoked by
 *       {@link ReferenceCountingFilter} if the filter is the last filter which
 *       was added to {@link IoFilterChain}s.</li>
 * </ol>
 * <p>
 * When you remove an {@link IoFilter} from an {@link IoFilterChain}:
 * <ol>
 *   <li>{@link #onPreRemove(IoFilterChain, String, NextFilter)} is invoked to
 *       notify that the filter will be removed from the chain.</li>
 *   <li>The filter is removed from the chain, and any events and I/O requests
 *       don't pass through the filter from now.</li>
 *   <li>{@link #onPostRemove(IoFilterChain, String, NextFilter)} is invoked to
 *       notify that the filter is removed from the chain.</li>
 *   <li>{@link #destroy()} is invoked by {@link ReferenceCountingFilter} if
 *       the removed filter was the last one.</li>
 * </ol>
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 *
 * @see IoFilterAdapter
 */

interface EIoFilter : virtual public EObject {
    /**
     * Represents the next {@link IoFilter} in {@link IoFilterChain}.
     */
    interface NextFilter {
    	virtual ~NextFilter(){}

    	/**
		 * Forwards <tt>sessionCreated</tt> event to next filter.
		 *
		 * @param session The {@link IoSession} which has to process this invocation
		 */
		virtual void sessionCreated(sp<EIoSession>& session) = 0;

		/**
		 * Forwards <tt>sessionOpened</tt> event to next filter.
		 *
		 * @param session The {@link IoSession} which has to process this invocation
		 */
		virtual void sessionOpened(sp<EIoSession>& session) = 0;

		/**
		 * Forwards <tt>sessionClosed</tt> event to next filter.
		 *
		 * @param session The {@link IoSession} which has to process this invocation
		 */
		virtual void sessionClosed(sp<EIoSession>& session) = 0;

		/**
		 * Forwards <tt>sessionIdle</tt> event to next filter.
		 *
		 * @param session The {@link IoSession} which has to process this invocation
		 * @param status The {@link IdleStatus} type
		 */
		virtual void sessionIdle(sp<EIoSession>& session, EIdleStatus status) = 0;

		/**
		 * Forwards <tt>exceptionCaught</tt> event to next filter.
		 *
		 * @param session The {@link IoSession} which has to process this invocation
		 * @param cause The exception that cause this event to be received
		 */
		virtual void exceptionCaught(sp<EIoSession>& session, sp<EThrowableType>& cause) = 0;

		/**
		 *
		 * @param session The {@link IoSession} which has to process this invocation
		 */
		virtual void inputClosed(sp<EIoSession>& session) = 0;

		/**
		 * Forwards <tt>messageReceived</tt> event to next filter.
		 *
		 * @param session The {@link IoSession} which has to process this invocation
		 * @param message The received message
		 */
		virtual void messageReceived(sp<EIoSession>& session, sp<EObject>& message) = 0;

		/**
		 * Forwards <tt>messageSent</tt> event to next filter.
		 *
		 * @param session The {@link IoSession} which has to process this invocation
		 * @param writeRequest The {@link WriteRequest} to process
		 */
		virtual void messageSent(sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) = 0;

		/**
		 * Forwards <tt>filterWrite</tt> event to next filter.
		 *
		 * @param session The {@link IoSession} which has to process this invocation
		 * @param writeRequest The {@link WriteRequest} to process
		 */
        virtual void filterWrite(sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) = 0;

        /**
		 * Forwards <tt>filterClose</tt> event to next filter.
		 *
		 * @param session The {@link IoSession} which has to process this invocation
		 */
        virtual void filterClose(sp<EIoSession>& session) = 0;

    };

	virtual ~EIoFilter(){}

    /**
     * Invoked by {@link ReferenceCountingFilter} when this filter
     * is added to a {@link IoFilterChain} at the first time, so you can
     * initialize shared resources.  Please note that this method is never
     * called if you don't wrap a filter with {@link ReferenceCountingFilter}.
     *
     * @throws Exception If an error occurred while processing the event
     */
	virtual void init() THROWS(EException) = 0;

    /**
     * Invoked by {@link ReferenceCountingFilter} when this filter
     * is not used by any {@link IoFilterChain} anymore, so you can destroy
     * shared resources.  Please note that this method is never called if
     * you don't wrap a filter with {@link ReferenceCountingFilter}.
     *
     * @throws Exception If an error occurred while processing the event
     */
	virtual void destroy() THROWS(EException) = 0;

    /**
     * Invoked before this filter is added to the specified <tt>parent</tt>.
     * Please note that this method can be invoked more than once if
     * this filter is added to more than one parents.  This method is not
     * invoked before {@link #init()} is invoked.
     *
     * @param parent the parent who called this method
     * @param name the name assigned to this filter
     * @param nextFilter the {@link NextFilter} for this filter.  You can reuse
     *                   this object until this filter is removed from the chain.
     *
     * @throws Exception If an error occurred while processing the event
     */
	virtual void onPreAdd(EIoFilterChain* parent, const char* name, NextFilter* nextFilter) THROWS(EException) = 0;

    /**
     * Invoked after this filter is added to the specified <tt>parent</tt>.
     * Please note that this method can be invoked more than once if
     * this filter is added to more than one parents.  This method is not
     * invoked before {@link #init()} is invoked.
     *
     * @param parent the parent who called this method
     * @param name the name assigned to this filter
     * @param nextFilter the {@link NextFilter} for this filter.  You can reuse
     *                   this object until this filter is removed from the chain.
     *
     * @throws Exception If an error occurred while processing the event
     */
	virtual void onPostAdd(EIoFilterChain* parent, const char* name, NextFilter* nextFilter) THROWS(EException) = 0;

    /**
     * Invoked before this filter is removed from the specified <tt>parent</tt>.
     * Please note that this method can be invoked more than once if
     * this filter is removed from more than one parents.
     * This method is always invoked before {@link #destroy()} is invoked.
     *
     * @param parent the parent who called this method
     * @param name the name assigned to this filter
     * @param nextFilter the {@link NextFilter} for this filter.  You can reuse
     *                   this object until this filter is removed from the chain.
     *
     * @throws Exception If an error occurred while processing the event
     */
	virtual void onPreRemove(EIoFilterChain* parent, const char* name, NextFilter* nextFilter) THROWS(EException) = 0;

    /**
     * Invoked after this filter is removed from the specified <tt>parent</tt>.
     * Please note that this method can be invoked more than once if
     * this filter is removed from more than one parents.
     * This method is always invoked before {@link #destroy()} is invoked.
     *
     * @param parent the parent who called this method
     * @param name the name assigned to this filter
     * @param nextFilter the {@link NextFilter} for this filter.  You can reuse
     *                   this object until this filter is removed from the chain.
     * @throws Exception If an error occurred while processing the event
     */
	virtual void onPostRemove(EIoFilterChain* parent, const char* name, NextFilter* nextFilter) THROWS(EException) = 0;

	/**
	 * Filters {@link IoHandler#sessionCreated(IoSession)} event.
	 *
	 * @param nextFilter
	 *            the {@link NextFilter} for this filter. You can reuse this
	 *            object until this filter is removed from the chain.
	 * @param session The {@link IoSession} which has received this event
	 * @throws Exception If an error occurred while processing the event
	 */
	virtual void sessionCreated(NextFilter* nextFilter, sp<EIoSession>& session) THROWS(EException) = 0;

	/**
	 * Filters {@link IoHandler#sessionOpened(IoSession)} event.
	 *
	 * @param nextFilter
	 *            the {@link NextFilter} for this filter. You can reuse this
	 *            object until this filter is removed from the chain.
	 * @param session The {@link IoSession} which has received this event
	 * @throws Exception If an error occurred while processing the event
	 */
	virtual void sessionOpened(NextFilter* nextFilter, sp<EIoSession>& session) THROWS(EException) = 0;

	/**
	 * Filters {@link IoHandler#sessionClosed(IoSession)} event.
	 *
	 * @param nextFilter
	 *            the {@link NextFilter} for this filter. You can reuse this
	 *            object until this filter is removed from the chain.
	 * @param session The {@link IoSession} which has received this event
	 * @throws Exception If an error occurred while processing the event
	 */
	virtual void sessionClosed(NextFilter* nextFilter, sp<EIoSession>& session) THROWS(EException) = 0;

	/**
	 * Filters {@link IoHandler#sessionIdle(IoSession,IdleStatus)} event.
	 *
	 * @param nextFilter
	 *            the {@link NextFilter} for this filter. You can reuse this
	 *            object until this filter is removed from the chain.
	 * @param session The {@link IoSession} which has received this event
	 * @param status The {@link IdleStatus} type
	 * @throws Exception If an error occurred while processing the event
	 */
	virtual void sessionIdle(NextFilter* nextFilter, sp<EIoSession>& session, EIdleStatus status) THROWS(EException) = 0;

	/**
	 * Filters {@link IoHandler#exceptionCaught(IoSession,Throwable)} event.
	 *
	 * @param nextFilter
	 *            the {@link NextFilter} for this filter. You can reuse this
	 *            object until this filter is removed from the chain.
	 * @param session The {@link IoSession} which has received this event
	 * @param cause The exception that cause this event to be received
	 * @throws Exception If an error occurred while processing the event
	 */
	virtual void exceptionCaught(NextFilter* nextFilter, sp<EIoSession>& session, sp<EThrowableType>& cause) THROWS(EException) = 0;

	/**
	 * Filters {@link IoHandler#inputClosed(IoSession)} event.
	 *
	 * @param nextFilter
	 *            the {@link NextFilter} for this filter. You can reuse this
	 *            object until this filter is removed from the chain.
	 * @param session The {@link IoSession} which has received this event
	 * @throws Exception If an error occurred while processing the event
	 */
	virtual void inputClosed(NextFilter* nextFilter, sp<EIoSession>& session) THROWS(EException) = 0;

	/**
	 * Filters {@link IoHandler#messageReceived(IoSession,Object)} event.
	 *
	 * @param nextFilter
	 *            the {@link NextFilter} for this filter. You can reuse this
	 *            object until this filter is removed from the chain.
	 * @param session The {@link IoSession} which has received this event
	 * @param message The received message
	 * @throws Exception If an error occurred while processing the event
	 */
	virtual void messageReceived(NextFilter* nextFilter, sp<EIoSession>& session, sp<EObject>& message) THROWS(EException) = 0;

	/**
	 * Filters {@link IoHandler#messageSent(IoSession,Object)} event.
	 *
	 * @param nextFilter
	 *            the {@link NextFilter} for this filter. You can reuse this
	 *            object until this filter is removed from the chain.
	 * @param session The {@link IoSession} which has received this event
	 * @param writeRequest The {@link WriteRequest} that contains the sent message
	 * @throws Exception If an error occurred while processing the event
	 */
	virtual void messageSent(NextFilter* nextFilter, sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) THROWS(EException) = 0;

	/**
	 * Filters {@link IoSession#close(boolean)} method invocation.
	 *
	 * @param nextFilter
	 *            the {@link NextFilter} for this filter. You can reuse this
	 *            object until this filter is removed from the chain.
	 * @param session
	 *            The {@link IoSession} which has to process this method
	 *            invocation
	 * @throws Exception If an error occurred while processing the event
	 */
	virtual void filterClose(NextFilter* nextFilter, sp<EIoSession>& session) THROWS(EException) = 0;

	/**
	 * Filters {@link IoSession#write(Object)} method invocation.
	 *
	 * @param nextFilter
	 *            the {@link NextFilter} for this filter. You can reuse this
	 *            object until this filter is removed from the chain.
	 * @param session The {@link IoSession} which has to process this invocation
	 * @param writeRequest The {@link WriteRequest} to process
	 * @throws Exception If an error occurred while processing the event
	 */
	virtual void filterWrite(NextFilter* nextFilter, sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) THROWS(EException) = 0;
};

}
} // namespace efc::eio

#endif /* EIOFILTER_HH_ */
