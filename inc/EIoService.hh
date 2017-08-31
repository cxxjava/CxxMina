/*
 * EIoService.hh
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#ifndef EIOSERVICE_HH_
#define EIOSERVICE_HH_

#include "./EIoHandler.hh"
#include "./EWriteFuture.hh"
#include "./EIoServiceListener.hh"
#include "./EIoSessionConfig.hh"
#include "./EIoServiceStatistics.hh"
#include "./EDefaultIoFilterChainBuilder.hh"
#include "./EIoSessionDataStructureFactory.hh"
#include "./EThrowableObject.hh"
#include "./ETransportMetadata.hh"

namespace efc {
namespace eio {

/**
 * Base interface for all {@link IoAcceptor}s and {@link IoConnector}s
 * that provide I/O service and manage {@link IoSession}s.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

interface EIoService : virtual public EObject {
	virtual ~EIoService() {
	}

	/**
	 * @return the {@link TransportMetadata} that this service runs on.
	 */
	virtual ETransportMetadata* getTransportMetadata() = 0;

	/**
	 * Adds an {@link IoServiceListener} that listens any events related with
	 * this service.
	 *
	 * @param listener The listener to add
	 */
	virtual void addListener(sp<EIoServiceListener> listener) = 0;

	/**
	 * Removed an existing {@link IoServiceListener} that listens any events
	 * related with this service.
	 *
	 * @param listener The listener to use
	 */
	virtual void removeListener(EIoServiceListener* listener) = 0;

	/**
	 * Returns <tt>true</tt> if and if only {@link #dispose()} method has
	 * been called.  Please note that this method will return <tt>true</tt>
	 * even after all the related resources are released.
	 */
	virtual boolean isDisposing() = 0;

	/**
	 * Returns <tt>true</tt> if and if only all resources of this processor
	 * have been disposed.
	 */
	virtual boolean isDisposed() = 0;

	/**
	 * Releases any resources allocated by this service.  Please note that
	 * this method might block as long as there are any sessions managed by
	 * this service.
	 */
	virtual void dispose() = 0;

	/**
	 * Releases any resources allocated by this service.  Please note that
	 * this method might block as long as there are any sessions managed by this service.
	 *
	 * Warning : calling this method from a IoFutureListener with <code>awaitTermination</code> = true
	 * will probably lead to a deadlock.
	 *
	 * @param awaitTermination When true this method will block until the underlying ExecutorService is terminated
	 */
	virtual void dispose(boolean awaitTermination) = 0;

	/**
	 * Returns the handler which will handle all connections managed by this service.
	 */
	virtual EIoHandler* getHandler() = 0;

	/**
	 * Sets the handler which will handle all connections managed by this service.
	 *
	 * @param handler The IoHandler to use
	 */
	virtual void setHandler(EIoHandler* handler) = 0;

	/**
	 * Returns the map of all sessions which are currently managed by this
	 * service.  The key of map is the {@link IoSession#getId() ID} of the
	 * session.
	 *
	 * @return the sessions. An empty collection if there's no session.
	 */
	virtual EConcurrentMap<llong, EIoSession>* getManagedSessions() = 0;

	/**
	 * Returns the number of all sessions which are currently managed by this
	 * service.
	 */
	virtual int getManagedSessionCount() = 0;

	/**
	 * Returns the default configuration of the new {@link IoSession}s
	 * created by this service.
	 */
	virtual EIoSessionConfig* getSessionConfig() = 0;

	/**
	 * Returns the {@link IoFilterChainBuilder} which will build the
	 * {@link IoFilterChain} of all {@link IoSession}s which is created
	 * by this service.
	 * The default value is an empty {@link DefaultIoFilterChainBuilder}.
	 */
	virtual EIoFilterChainBuilder* getFilterChainBuilder() = 0;

	/**
	 * A shortcut for <tt>( ( DefaultIoFilterChainBuilder ) </tt>{@link #getFilterChainBuilder()}<tt> )</tt>.
	 * Please note that the returned object is not a <b>real</b> {@link IoFilterChain}
	 * but a {@link DefaultIoFilterChainBuilder}.  Modifying the returned builder
	 * won't affect the existing {@link IoSession}s at all, because
	 * {@link IoFilterChainBuilder}s affect only newly created {@link IoSession}s.
	 *
	 * @return The filter chain in use
	 * @throws IllegalStateException if the current {@link IoFilterChainBuilder} is
	 *                               not a {@link DefaultIoFilterChainBuilder}
	 */
	virtual EDefaultIoFilterChainBuilder* getFilterChain() = 0;

	/**
	 * Returns a value of whether or not this service is active
	 *
	 * @return whether of not the service is active.
	 */
	virtual boolean isActive() = 0;

	/**
	 * Returns the time when this service was activated.  It returns the last
	 * time when this service was activated if the service is not active now.
	 *
	 * @return The time by using {@link System#currentTimeMillis()}
	 */
	virtual long getActivationTime() = 0;

	/**
	 * Writes the specified {@code message} to all the {@link IoSession}s
	 * managed by this service.  This method is a convenience shortcut for
	 * {@link IoUtil#broadcast(Object, Collection)}.
	 *
	 * @param message the message to broadcast
     * @return The set of WriteFuture associated to the message being broadcasted
	 */
	virtual EArrayList<sp<EWriteFuture> > broadcast(sp<EObject> message) = 0;

	/**
	 * Returns the {@link IoSessionDataStructureFactory} that provides
	 * related data structures for a new session created by this service.
	 */
	virtual EIoSessionDataStructureFactory* getSessionDataStructureFactory() = 0;

	/**
	 * Returns the number of bytes scheduled to be written
	 *
	 * @return The number of bytes scheduled to be written
	 */
	virtual int getScheduledWriteBytes() = 0;

	/**
	 * Returns the number of messages scheduled to be written
	 *
	 * @return The number of messages scheduled to be written
	 */
	virtual int getScheduledWriteMessages() = 0;

	/**
	 * Returns the IoServiceStatistics object for this service.
	 *
	 * @return The statistics object for this service.
	 */
	virtual EIoServiceStatistics* getStatistics() = 0;
};

}
} // namespace efc::eio

#endif /* EIOSERVICE_HH_ */
