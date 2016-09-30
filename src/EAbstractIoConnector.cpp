/*
 * EAbstractIoConnector.cpp
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#include "EAbstractIoConnector.hh"
#include "EIllegalArgumentException.hh"
#include "EIoHandler.hh"

namespace efc {
namespace eio {

EAbstractIoConnector::~EAbstractIoConnector() {
    delete defaultRemoteAddress;
    delete defaultLocalAddress;
}

long EAbstractIoConnector::getConnectTimeoutCheckInterval() {
	return connectTimeoutCheckInterval;
}

void EAbstractIoConnector::setConnectTimeoutCheckInterval(
		long minimumConnectTimeout) {
	if (getConnectTimeoutMillis() < minimumConnectTimeout) {
		this->connectTimeoutInMillis = minimumConnectTimeout;
	}

	this->connectTimeoutCheckInterval = minimumConnectTimeout;
}

long EAbstractIoConnector::getConnectTimeoutMillis() {
	return connectTimeoutInMillis;
}

void EAbstractIoConnector::setConnectTimeoutMillis(
		long connectTimeoutInMillis) {
	if (connectTimeoutInMillis <= connectTimeoutCheckInterval) {
		this->connectTimeoutCheckInterval = connectTimeoutInMillis;
	}
	this->connectTimeoutInMillis = connectTimeoutInMillis;
}

EInetSocketAddress* EAbstractIoConnector::getDefaultRemoteAddress() {
	return defaultRemoteAddress;
}

void EAbstractIoConnector::setDefaultRemoteAddress(EInetSocketAddress* defaultRemoteAddress) {
    if (defaultRemoteAddress == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "defaultRemoteAddress");
	}

    /*
	if (!getTransportMetadata()->getAddressType()->isAssignableFrom(defaultRemoteAddress->getClass())) {
		throw new IllegalArgumentException("defaultRemoteAddress type: " + defaultRemoteAddress.getClass()
				+ " (expected: " + getTransportMetadata().getAddressType() + ")");
	}
    */

    delete this->defaultRemoteAddress;
	this->defaultRemoteAddress = new EInetSocketAddress(*defaultRemoteAddress);
}

void EAbstractIoConnector::setDefaultRemoteAddress(const char* hostname, int port) {
	EInetSocketAddress isa(hostname, port);
	this->setDefaultRemoteAddress(&isa);
}

sp<EConnectFuture> EAbstractIoConnector::connect() {
	EInetSocketAddress* defaultRemoteAddress = getDefaultRemoteAddress();
	if (defaultRemoteAddress == null) {
		throw EIllegalStateException(__FILE__, __LINE__, "defaultRemoteAddress is not set.");
	}

	return connect(defaultRemoteAddress, null, null);
}

sp<EConnectFuture> EAbstractIoConnector::connect(
		EIoSessionInitializer* sessionInitializer) {
	EInetSocketAddress* defaultRemoteAddress = getDefaultRemoteAddress();
	if (defaultRemoteAddress == null) {
		throw EIllegalStateException(__FILE__, __LINE__, "defaultRemoteAddress is not set.");
	}

	return connect(defaultRemoteAddress, null, sessionInitializer);
}

sp<EConnectFuture> EAbstractIoConnector::connect(
		EInetSocketAddress* remoteAddress) {
	return connect(remoteAddress, null, null);
}

sp<EConnectFuture> EAbstractIoConnector::connect(const char* hostname, int port) {
	EInetSocketAddress remoteAddress(hostname, port);
	return connect(&remoteAddress, null, null);
}

sp<EConnectFuture> EAbstractIoConnector::connect(EInetSocketAddress* remoteAddress,
		EIoSessionInitializer* sessionInitializer) {
	return connect(remoteAddress, null, sessionInitializer);
}

sp<EConnectFuture> EAbstractIoConnector::connect(const char* hostname, int port,
				EIoSessionInitializer* sessionInitializer) {
	EInetSocketAddress remoteAddress(hostname, port);
	return connect(&remoteAddress, null, sessionInitializer);
}

sp<EConnectFuture> EAbstractIoConnector::connect(EInetSocketAddress* remoteAddress,
		EInetSocketAddress* localAddress) {
	return connect(remoteAddress, localAddress, null);
}

sp<EConnectFuture> EAbstractIoConnector::connect(const char* rhostname, int rport,
			const char* lhostname, int lport) {
	return connect(rhostname, rport, lhostname, lport, null);
}

sp<EConnectFuture> EAbstractIoConnector::connect(EInetSocketAddress* remoteAddress,
		EInetSocketAddress* localAddress,
		EIoSessionInitializer* sessionInitializer) {
	if (isDisposing()) {
		throw EIllegalStateException(__FILE__, __LINE__, "The connector has been disposed.");
	}

	if (remoteAddress == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "remoteAddress");
	}

	/*
	if (!getTransportMetadata().getAddressType().isAssignableFrom(remoteAddress.getClass())) {
		throw EIllegalArgumentException("remoteAddress type: " + remoteAddress.getClass() + " (expected: "
				+ getTransportMetadata().getAddressType() + ")");
	}

	if (localAddress != null && !getTransportMetadata().getAddressType().isAssignableFrom(localAddress.getClass())) {
		throw EIllegalArgumentException("localAddress type: " + localAddress.getClass() + " (expected: "
				+ getTransportMetadata().getAddressType() + ")");
	}
	*/

	if (getHandler() == null) {
		if (getSessionConfig()->isUseReadOperation()) {
			class DummyIoHandler : public EIoHandler
			{
			public:
				void sessionCreated(sp<EIoSession>& session) {
					// Empty handler
				}

				void sessionOpened(sp<EIoSession>& session) {
					// Empty handler
				}

				void sessionClosed(sp<EIoSession>& session) {
					// Empty handler
				}

				void sessionIdle(sp<EIoSession>& session, EIdleStatus status) {
					// Empty handler
				}

				void exceptionCaught(sp<EIoSession>& session, sp<EThrowableType>& cause) {
					// Empty handler
				}

				void messageReceived(sp<EIoSession>& session, sp<EObject>& message) {
					// Empty handler
				}

				void messageSent(sp<EIoSession>& session, sp<EObject>& message) {
					// Empty handler
				}

				void inputClosed(sp<EIoSession>& session) {
					// Empty handler
				}
			};
			setHandler(new DummyIoHandler());
		} else {
			throw EIllegalStateException(__FILE__, __LINE__, "handler is not set.");
		}
	}

	return connect0(remoteAddress, localAddress, sessionInitializer);
}

sp<EConnectFuture> EAbstractIoConnector::connect(const char* rhostname, int rport,
				const char* lhostname, int lport,
				EIoSessionInitializer* sessionInitializer) {
	EInetSocketAddress risa(rhostname, rport);
	EInetSocketAddress lisa(lhostname, lport);
	return this->connect(&risa, &lisa, sessionInitializer);
}

EStringBase EAbstractIoConnector::toString() {
	return EStringBase::formatOf("(nio connector: managedSessionCount: %d)", getManagedSessionCount());
}

EAbstractIoConnector::EAbstractIoConnector(EIoSessionConfig* sessionConfig,
		EExecutorService* executor) :
		EAbstractIoService(sessionConfig, executor), connectTimeoutCheckInterval(
				50L), connectTimeoutInMillis(60 * 1000L), defaultRemoteAddress(
				null), defaultLocalAddress(null) {
}

EInetSocketAddress* EAbstractIoConnector::getDefaultLocalAddress() {
	return defaultLocalAddress;
}

void EAbstractIoConnector::setDefaultLocalAddress(EInetSocketAddress* localAddress) {
    if (localAddress == null) {
        throw EIllegalArgumentException(__FILE__, __LINE__, "localAddress");
    }
    
    delete defaultLocalAddress;
    defaultLocalAddress = new EInetSocketAddress(*localAddress);
}

void EAbstractIoConnector::setDefaultLocalAddress(const char* hostname, int port) {
	EInetSocketAddress isa(hostname, port);
	this->setDefaultLocalAddress(&isa);
}

void EAbstractIoConnector::finishSessionInitialization0(sp<EIoSession>& session,
		sp<EIoFuture>& future) {
	class ConnectFutureListener : public EIoFutureListener {
	private:
		sp<EIoSession> session;
	public:
		ConnectFutureListener(sp<EIoSession>& s) : session(s) {
		}
		void operationComplete(EIoFuture* future) {
			EConnectFuture* f = dynamic_cast<EConnectFuture*>(future);
			ES_ASSERT(f);
			if (f->isCanceled()) {
				session->closeNow();
			}
		}
	};
	// In case that ConnectFuture.cancel() is invoked before
	// setSession() is invoked, add a listener that closes the
	// connection immediately on cancellation.
	future->addListener(new ConnectFutureListener(session));
}

} /* namespace eio */
} /* namespace efc */
