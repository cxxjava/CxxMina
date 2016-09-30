/*
 * EIoAcceptor.hh
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#ifndef EIOACCEPTOR_HH_
#define EIOACCEPTOR_HH_

#include "Efc.hh"

namespace efc {
namespace eio {

/**
 * Base interface for all {@link IoAcceptor}s and {@link IoConnector}s
 * that provide I/O service and manage {@link IoSession}s.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */
interface EIoAcceptor: virtual public EIoService {
	virtual ~EIoAcceptor(){}

	/**
	 * Returns the local address which is bound currently.  If more than one
	 * address are bound, only one of them will be returned, but it's not
	 * necessarily the firstly bound address.
	 *
	 * @return The bound LocalAddress
	 */
	virtual EInetSocketAddress* getLocalAddress() = 0;

	/**
	 * Returns a {@link Set} of the local addresses which are bound currently.
	 *
	 * @return The Set of bound LocalAddresses
	 */
	virtual ESet<EInetSocketAddress*>* getLocalAddresses() = 0;

	/**
	 * Returns the default local address to bind when no argument is specified
	 * in {@link #bind()} method.  Please note that the default will not be
	 * used if any local address is specified.  If more than one address are
	 * set, only one of them will be returned, but it's not necessarily the
	 * firstly specified address in {@link #setDefaultLocalAddresses(List)}.
	 *
	 * @return The default bound LocalAddress
	 */
	virtual EInetSocketAddress* getDefaultLocalAddress() = 0;

	/**
	 * Returns a {@link List} of the default local addresses to bind when no
	 * argument is specified in {@link #bind()} method.  Please note that the
	 * default will not be used if any local address is specified.
	 *
	 * @return The list of default bound LocalAddresses
	 */
	virtual EList<EInetSocketAddress*>* getDefaultLocalAddresses() = 0;

	/**
	 * Sets the default local address to bind when no argument is specified in
	 * {@link #bind()} method.  Please note that the default will not be used
	 * if any local address is specified.
	 *
	 * @param localAddress The local addresses to bind the acceptor on
	 */
	virtual void setDefaultLocalAddress(EInetSocketAddress* localAddress) = 0;
	virtual void setDefaultLocalAddress(const char* hostname, int port) = 0;

	/**
	 * Sets the default local addresses to bind when no argument is specified
	 * in {@link #bind()} method.  Please note that the default will not be
	 * used if any local address is specified.
	 *
	 * @param localAddresses The local addresses to bind the acceptor on
	 */
	virtual void setDefaultLocalAddresses(EIterable<EInetSocketAddress*> *localAddresses) = 0;

	/**
	 * Returns <tt>true</tt> if and only if all clients are closed when this
	 * acceptor unbinds from all the related local address (i.e. when the
	 * service is deactivated).
	 *
	 * @return <tt>true</tt> if the service sets the closeOnDeactivation flag
	 */
	virtual boolean isCloseOnDeactivation() = 0;

	/**
	 * Sets whether all client sessions are closed when this acceptor unbinds
	 * from all the related local addresses (i.e. when the service is
	 * deactivated).  The default value is <tt>true</tt>.
	 *
	 * @param closeOnDeactivation <tt>true</tt> if we should close on deactivation
	 */
	virtual void setCloseOnDeactivation(boolean closeOnDeactivation) = 0;

	/**
	 * Binds to the default local address(es) and start to accept incoming
	 * connections.
	 *
	 * @throws IOException if failed to bind
	 */
	virtual void bind() THROWS(EIOException) = 0;

	/**
	 * Binds to the specified local addresses and start to accept incoming
	 * connections. If no address is given, bind on the default local address.
	 *
	 * @param localddress The local address we will be bound to
	 *
	 * @throws IOException if failed to bind
	 */
	virtual void bind(EInetSocketAddress *localddress) THROWS(EIOException) = 0;
	virtual void bind(const char *hostname, int port) THROWS(EIOException) = 0;

	/**
	 * Binds to the specified local addresses and start to accept incoming
	 * connections.
	 *
	 * @param localAddresses The local address we will be bound to
	 * @throws IOException if failed to bind
	 */
	virtual void bind(EIterable<EInetSocketAddress*> *localAddresses) THROWS(EIOException) = 0;

	/**
	 * Unbinds from all local addresses that this service is bound to and stops
	 * to accept incoming connections.  All managed connections will be closed
	 * if {@link #setCloseOnDeactivation(boolean) disconnectOnUnbind} property
	 * is <tt>true</tt>.  This method returns silently if no local address is
	 * bound yet.
	 */
	virtual void unbind() = 0;

	/**
	 * Unbinds from the specified local addresses and stop to accept incoming
	 * connections.  All managed connections will be closed if
	 * {@link #setCloseOnDeactivation(boolean) disconnectOnUnbind} property is
	 * <tt>true</tt>.  This method returns silently if the default local
	 * addresses are not bound yet.
	 *
	 * @param localddress The local address to be unbound from
	 */
	virtual void unbind(EInetSocketAddress *localAddress) = 0;
	virtual void unbind(const char* hostname, int port) = 0;

	/**
	 * Unbinds from the specified local addresses and stop to accept incoming
	 * connections.  All managed connections will be closed if
	 * {@link #setCloseOnDeactivation(boolean) disconnectOnUnbind} property is
	 * <tt>true</tt>.  This method returns silently if the default local
	 * addresses are not bound yet.
	 *
	 * @param localAddresses The local address we will be unbound from
	 */
	virtual void unbind(EIterable<EInetSocketAddress*> *localAddresses) = 0;

	/**
	 * (Optional) Returns an {@link IoSession} that is bound to the specified
	 * <tt>localAddress</tt> and the specified <tt>remoteAddress</tt> which
	 * reuses the local address that is already bound by this service.
	 * <p>
	 * This operation is optional.  Please throw {@link UnsupportedOperationException}
	 * if the transport type doesn't support this operation.  This operation is
	 * usually implemented for connectionless transport types.
	 *
	 * @param remoteAddress The remote address bound to the service
	 * @param localAddress The local address the session will be bound to
	 * @throws UnsupportedOperationException if this operation is not supported
	 * @throws IllegalStateException if this service is not running.
	 * @throws IllegalArgumentException if this service is not bound to the
	 *                                  specified <tt>localAddress</tt>.
	 * @return The session bound to the the given localAddress and remote address
	 */
	virtual sp<EIoSession> newSession(EInetSocketAddress *remoteAddress, EInetSocketAddress *localAddress) = 0;
};

}
} // namespace efc::eio

#endif /* EIOACCEPTOR_HH_ */
