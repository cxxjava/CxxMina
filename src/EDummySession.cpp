/*
 * EDummySession.cpp
 *
 *  Created on: 2016-9-23
 *      Author: cxxjava@163.com
 */

#include "EDummySession.hh"
#include "EFileRegion.hh"
#include "EIoHandlerAdapter.hh"
#include "EDefaultIoFilterChain.hh"
#include "EAbstractIoAcceptor.hh"
#include "EAbstractIoSessionConfig.hh"
#include "EDefaultTransportMetadata.hh"
#include "EDefaultIoSessionDataStructureFactory.hh"

namespace efc {
namespace eio {

class DummyIoSessionConfig: public EAbstractIoSessionConfig {
public:
	virtual void doSetAll(EIoSessionConfig* config) {
		// Do nothing
	}
};

class DummyExecutor: public EExecutorService {
public:
	virtual void execute(sp<ERunnable> command) {
		// Do nothing
	}
	virtual void shutdown() {
		// Do nothing
	}
	virtual EArrayList<sp<ERunnable> > shutdownNow() {
		return EArrayList<sp<ERunnable> >();
	}
	virtual boolean isShutdown() {
		return false;
	}
	virtual boolean isTerminated() {
		return false;
	}
	virtual boolean awaitTermination() {
		return false;
	}
	virtual boolean awaitTermination(llong timeout, ETimeUnit* unit) {
		return false;
	}
};

class DummyIoAcceptor: public EAbstractIoAcceptor {
public:
	DummyIoAcceptor(DummyIoSessionConfig* config, EExecutorService* executor):
		EAbstractIoAcceptor(config, executor) {
	}

	virtual sp<ESet<EInetSocketAddress*> > bindInternal(EList<EInetSocketAddress*>& localAddresses) {
		throw EUnsupportedOperationException(__FILE__, __LINE__);
	}

	virtual void unbind0(EList<EInetSocketAddress*>& localAddresses) {
		throw EUnsupportedOperationException(__FILE__, __LINE__);
	}

	virtual sp<EIoSession> newSession(EInetSocketAddress *remoteAddress, EInetSocketAddress *localAddress){
		throw EUnsupportedOperationException(__FILE__, __LINE__);
	}

	virtual ETransportMetadata* getTransportMetadata() {
		static EDefaultTransportMetadata metadata("mina", "dummy", false, false);
		return &metadata;
	}

	virtual void dispose0() {
	}

	virtual EIoSessionConfig* getSessionConfig() {
		return EAbstractIoService::sessionConfig;
	}
};

class DummyIoProcessor: public EIoProcessor {
public:
	virtual void add(sp<EIoSession> session) {
		// Do nothing
	}

	virtual void flush(sp<EIoSession>& session) {
		sp<EDummySession> s = dynamic_pointer_cast<EDummySession>(session);
		sp<EWriteRequest> req = s->getWriteRequestQueue()->poll(session);

		// Chek that the request is not null. If the session has been closed,
		// we may not have any pending requests.
		if (req != null) {
			sp<EObject> m = req->getMessage();
			EFileRegion* file = dynamic_cast<EFileRegion*>(m.get());
			if (file) {
				try {
					file->getFileChannel()->position(file->getPosition() + file->getRemainingBytes());
					file->update(file->getRemainingBytes());
				} catch (EIOException& e) {
					s->getFilterChain()->fireExceptionCaught(new EThrowableObject<EIOException>(e));
				}
			}
			//FIXME? @see: getFilterChain()->fireMessageSent(req);
			s->getFilterChain()->fireMessageSent(req);
		}
	}

	virtual void write(sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) {
		EWriteRequestQueue* writeRequestQueue = session->getWriteRequestQueue();

		writeRequestQueue->offer(session, writeRequest);

		if (!session->isWriteSuspended()) {
			this->flush(session);
		}
	}

	virtual void remove(sp<EIoSession>& session) {
		if (!session->getCloseFuture()->isClosed()) {
			session->getFilterChain()->fireSessionClosed();
		}
	}

	virtual void updateTrafficControl(sp<EIoSession>& session) {
		// Do nothing
	}

	virtual void dispose() {
		// Do nothing
	}

	virtual boolean isDisposing() {
		return false;
	}

	virtual boolean isDisposed() {
		return false;
	}
};

static DummyIoSessionConfig *dummyIoSessionConfig;
static DummyExecutor *dummyExecutor;
static DummyIoAcceptor *dummyIoAcceptor;
static EIoHandlerAdapter *defaultIoHandlerAdapter;

DEFINE_STATIC_INITZZ_BEGIN(EDummySession)
ESystem::_initzz_();
dummyIoSessionConfig = new DummyIoSessionConfig();
dummyExecutor = new DummyExecutor();
dummyIoAcceptor = new DummyIoAcceptor(dummyIoSessionConfig, dummyExecutor);
defaultIoHandlerAdapter = new EIoHandlerAdapter();
DEFINE_STATIC_INITZZ_END

//=============================================================================

EDummySession::~EDummySession() {
	delete filterChain;
	delete localAddress;
	delete remoteAddress;
	delete processor;
}

EDummySession::EDummySession(): EAbstractIoSession(dummyIoAcceptor) {
	this->config = dummyIoSessionConfig;
	this->handler = defaultIoHandlerAdapter;
	this->filterChain = null;
	this->localAddress = new EInetSocketAddress(0, 0); //ANONYMOUS_ADDRESS;
	this->remoteAddress = new EInetSocketAddress(0, 0); //ANONYMOUS_ADDRESS;
	this->transportMetadata = dummyIoAcceptor->getTransportMetadata();

	this->processor = new DummyIoProcessor();
	this->service = EAbstractIoSession::getService();

	EDefaultIoSessionDataStructureFactory factory;
	setAttributeMap(factory.getAttributeMap(this));
	setWriteRequestQueue(factory.getWriteRequestQueue(this));
}

void EDummySession::init() {
	EAbstractIoSession::init();
	filterChain = new EDefaultIoFilterChain(shared_from_this());
}

void EDummySession::setConfig(EIoSessionConfig* config) {
	if (config == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "config");
	}

	this->config = config;
}

void EDummySession::setHandler(EIoHandler* handler) {
	if (handler == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "handler");
	}

	this->handler = handler;
}

void EDummySession::setLocalAddress(EInetSocketAddress* localAddress) {
	if (localAddress == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "localAddress");
	}

	EInetSocketAddress* old = this->localAddress;
	this->localAddress = new EInetSocketAddress(*localAddress);
	delete old;
}

void EDummySession::setRemoteAddress(EInetSocketAddress* remoteAddress) {
	if (remoteAddress == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "remoteAddress");
	}

	EInetSocketAddress* old = this->remoteAddress;
	this->remoteAddress = new EInetSocketAddress(*localAddress);
	delete old;
}

void EDummySession::setService(EIoService* service) {
	if (service == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "service");
	}

	this->service = service;
}

void EDummySession::setTransportMetadata(
		ETransportMetadata* transportMetadata) {
	if (transportMetadata == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "transportMetadata");
	}

	this->transportMetadata = transportMetadata;
}

void EDummySession::updateThroughput(boolean force) {
	EAbstractIoSession::updateThroughput(ESystem::currentTimeMillis(), force);
}

EIoSessionConfig* EDummySession::getConfig() {
	return config;
}

EIoFilterChain* EDummySession::getFilterChain() {
	return filterChain;
}

EIoHandler* EDummySession::getHandler() {
	return handler;
}

EInetSocketAddress* EDummySession::getLocalAddress() {
	return localAddress;
}

EInetSocketAddress* EDummySession::getRemoteAddress() {
	return remoteAddress;
}

EIoService* EDummySession::getService() {
	return service;
}

EIoProcessor* EDummySession::getProcessor() {
	return processor;
}

ETransportMetadata* EDummySession::getTransportMetadata() {
	return transportMetadata;
}

void EDummySession::setScheduledWriteBytes(int byteCount) {
	EAbstractIoSession::setScheduledWriteBytes(byteCount);
}

void EDummySession::setScheduledWriteMessages(int messages) {
	EAbstractIoSession::setScheduledWriteMessages(messages);
}

boolean EDummySession::isDestroyed() {
	return true;
}

void EDummySession::destroy() {
	// Do nothing
}

} /* namespace eio */
} /* namespace efc */
