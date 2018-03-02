/*
 * EAbstractIoAcceptor.hh
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#ifndef EABSTRACTIOACCEPTOR_HH_
#define EABSTRACTIOACCEPTOR_HH_

#include "./EAbstractIoService.hh"
#include "./EIoAcceptor.hh"
#include "./EIoSessionConfig.hh"

namespace efc {
namespace eio {

/**
 * A base implementation of {@link IoAcceptor}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @org.apache.xbean.XBean
 */
abstract class EAbstractIoAcceptor: public EAbstractIoService,
		virtual public EIoAcceptor {
public:
	class AcceptorOperationFuture : public ServiceOperationFuture {
	public:
		virtual ~AcceptorOperationFuture() {
			delete localAddresses;
		}

		AcceptorOperationFuture(EList<EInetSocketAddress*>& localAddresses) {
			this->localAddresses = new EArrayList<EInetSocketAddress*>(localAddresses.size());
			for (int i = 0; i < localAddresses.size(); i++) {
				this->localAddresses->add(new EInetSocketAddress(* localAddresses.getAt(i)));
			}
		}

		EList<EInetSocketAddress*>* getLocalAddresses() {
			return localAddresses;
		}

		/**
		 * @see Object#toString()
		 */
		virtual EString toString() {
			EString sb("Acceptor operation : ");

			if (localAddresses != null) {
				boolean isFirst = true;

				sp<EIterator<EInetSocketAddress*> > iter = localAddresses->listIterator();
				while (iter->hasNext()) {
					EInetSocketAddress* address = iter->next();
					if (isFirst) {
						isFirst = false;
					} else {
						sb.append(", ");
					}

					sb.append(address->toString().c_str());
				}
			}
			return sb;
		}
	private:
		EArrayList<EInetSocketAddress*>* localAddresses;
	};

public:
	virtual ~EAbstractIoAcceptor();

	/**
	 * {@inheritDoc}
	 */
	virtual EInetSocketAddress* getLocalAddress();

	/**
	 * {@inheritDoc}
	 */
	virtual ESet<EInetSocketAddress*>* getLocalAddresses();

	/**
	 * {@inheritDoc}
	 */
	virtual EInetSocketAddress* getDefaultLocalAddress();

	/**
	 * {@inheritDoc}
	 */
	virtual EList<EInetSocketAddress*>* getDefaultLocalAddresses();

	/**
	 * {@inheritDoc}
	 */
	virtual void setDefaultLocalAddress(EInetSocketAddress* localAddress);
	virtual void setDefaultLocalAddress(const char* hostname, int port);

	/**
	 * {@inheritDoc}
	 */
	virtual void setDefaultLocalAddresses(EIterable<EInetSocketAddress*>* localAddresses);

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isCloseOnDeactivation();

	/**
	 * {@inheritDoc}
	 */
	virtual void setCloseOnDeactivation(boolean disconnectClientsOnUnbind);

	/**
	 * {@inheritDoc}
	 */
	virtual void bind() THROWS(EIOException);

	/**
	 * {@inheritDoc}
	 */
	virtual void bind(EInetSocketAddress *localddress) THROWS(EIOException);
	virtual void bind(const char *hostname, int port) THROWS(EIOException);

	/**
	 * {@inheritDoc}
	 */
	virtual void bind(EIterable<EInetSocketAddress*> *localAddresses) THROWS(EIOException);

	/**
	 * {@inheritDoc}
	 */
	virtual void unbind();

	/**
	 * {@inheritDoc}
	 */
	virtual void unbind(EInetSocketAddress *localAddress);
	virtual void unbind(const char* hostname, int port);

	/**
	 * {@inheritDoc}
	 */
	virtual void unbind(EIterable<EInetSocketAddress*> *localAddresses);

	/**
	 * {@inheritDoc}
	 */
	virtual EString toString();

protected:
	/**
	 * The lock object which is acquired while bind or unbind operation is performed.
	 * Acquire this lock in your property setters which shouldn't be changed while
	 * the service is bound.
	 */
	EReentrantLock bindLock;

	/**
	 * Constructor for {@link AbstractIoAcceptor}. You need to provide a default
	 * session configuration and an {@link Executor} for handling I/O events. If
	 * null {@link Executor} is provided, a default one will be created using
	 * {@link Executors#newCachedThreadPool()}.
	 *
	 * {@see AbstractIoService#AbstractIoService(IoSessionConfig, Executor)}
	 *
	 * @param sessionConfig
	 *            the default configuration for the managed {@link IoSession}
	 * @param executor
	 *            the {@link Executor} used for handling execution of I/O
	 *            events. Can be <code>null</code>.
	 */
	EAbstractIoAcceptor(EIoSessionConfig* sessionConfig, EExecutorService* executor);

	/**
	 * Starts the acceptor, and register the given addresses
	 *
	 * @param localAddresses The address to bind to
	 * @return the {@link Set} of the local addresses which is bound actually
	 * @throws Exception If the bind failed
	 */
	virtual sp<ESet<EInetSocketAddress*> > bindInternal(EList<EInetSocketAddress*>& localAddresses) THROWS(EIOException) = 0;

	/**
	 * Implement this method to perform the actual unbind operation.
	 *
	 * @param localAddresses The address to unbind from
	 * @throws Exception If the unbind failed
	 */
	virtual void unbind0(EList<EInetSocketAddress*>& localAddresses) THROWS(EIOException) = 0;

private:
	EList<EInetSocketAddress*>* defaultLocalAddresses_;// = new ArrayList<SocketAddress>();
	ESet<EInetSocketAddress*>* boundAddresses_;// = new HashSet<SocketAddress>();
	EReentrantLock boundAddressesLock;
	boolean disconnectOnUnbind_; // = true;

	void checkAddressType(EInetSocketAddress* a);
};

} /* namespace eio */
} /* namespace efc */

#endif /* EABSTRACTIOACCEPTOR_HH_ */
