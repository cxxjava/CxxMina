/*
 * ENioSocketConnector.cpp
 *
 *  Created on: 2016-3-22
 *      Author: cxxjava@163.com
 */

#include "../inc/ENioSocketConnector.hh"
#include "../inc/ENioSocketSession.hh"

namespace efc {
namespace eio {

class SocketChannelIterator : public EIterator<sp<ESocketChannel> > {
private:
	sp<EIterator<sp<ESelectionKey> > > i;

public:
	virtual ~SocketChannelIterator() {
		//
	}

	SocketChannelIterator(ECollection<sp<ESelectionKey> >* selectedKeys) {
		this->i = selectedKeys->iterator();
	}

	/**
	 * {@inheritDoc}
	 */
	virtual boolean hasNext() {
		return i->hasNext();
	}

	/**
	 * {@inheritDoc}
	 */
	virtual sp<ESocketChannel> next() {
		sp<ESelectionKey> key = i->next();
		return dynamic_pointer_cast<ESocketChannel>(key->channel());
	}

	/**
	 * {@inheritDoc}
	 */
	virtual void remove() {
		i->remove();
	}

	sp<ESocketChannel> moveOut() {
		sp<ESelectionKey> key = i->moveOut();
		sp<ESocketChannel> channel = dynamic_pointer_cast<ESocketChannel>(key->channel());
		return channel;
	}
};

ENioSocketConnector::~ENioSocketConnector() {
	delete selector;
	delete dssc;
}

ENioSocketConnector::ENioSocketConnector():
		EAbstractPollingIoConnector<sp<ESocketChannel> >(dssc = new EDefaultSocketSessionConfig()) {
	this->init();
	dssc->init(this);
}

ENioSocketConnector::ENioSocketConnector(int processorCount):
		EAbstractPollingIoConnector<sp<ESocketChannel> >(dssc = new EDefaultSocketSessionConfig(), processorCount) {
	this->init();
	dssc->init(this);
}

ENioSocketConnector::ENioSocketConnector(EIoProcessor* processor):
		EAbstractPollingIoConnector<sp<ESocketChannel> >(dssc = new EDefaultSocketSessionConfig(), processor) {
	this->init();
	dssc->init(this);
}

ENioSocketConnector::ENioSocketConnector(EExecutorService* executor,
		EIoProcessor* processor) :
		EAbstractPollingIoConnector<sp<ESocketChannel> >(
				new EDefaultSocketSessionConfig(), executor, processor) {
	this->init();
	EDefaultSocketSessionConfig* dssc = dynamic_cast<EDefaultSocketSessionConfig*>(getSessionConfig());
	dssc->init(this);
}

void ENioSocketConnector::init() {
	{
		ON_SCOPE_EXIT(
			if (!selectable) {
				try {
					destroy();
				} catch (EException& e) {
					EExceptionMonitor::getInstance()->exceptionCaught(e);
				}
			}
		);

		try {
			// Initialize the selector
			selector = ESelector::open();

			// The selector is now ready, we can switch the
			// flag to true so that incoming connection can be accepted
			selectable = true;
		} catch (ERuntimeException& e) {
			throw e;
		} catch (EException& e) {
			throw ERuntimeIoException(__FILE__, __LINE__, "Failed to initialize.");
		}
	}
}

void ENioSocketConnector::destroy() {
	if (selector != null) {
		selector->close();
	}
}

ETransportMetadata* ENioSocketConnector::getTransportMetadata() {
	return ENioSocketSession::METADATA();
}

EInetSocketAddress* ENioSocketConnector::getDefaultRemoteAddress() {
	return EAbstractIoConnector::getDefaultRemoteAddress();
}

void ENioSocketConnector::setDefaultRemoteAddress(EInetSocketAddress* defaultRemoteAddress) {
	EAbstractIoConnector::setDefaultRemoteAddress(defaultRemoteAddress);
}

sp<ESocketChannel> ENioSocketConnector::newHandle(
		EInetSocketAddress* localAddress) {
	sp<ESocketChannel> ch = ESocketChannel::open();

	int receiveBufferSize = getSessionConfig()->getReceiveBufferSize();

	if (receiveBufferSize > 65535) {
		ch->socket()->setReceiveBufferSize(receiveBufferSize);
	}

	if (localAddress != null) {
		try {
			ch->socket()->bind(localAddress);
		} catch (EIOException& ioe) {
			// Add some info regarding the address we try to bind to the
			// message
			EString msg("Error while binding on ");
			msg << localAddress->toString() << "\nCause:\n  " << ioe.getMessage();
			EIOException e(__FILE__, __LINE__, msg.c_str());

			// Preemptively close the channel
			ch->close();
			throw e;
		}
	}

	ch->configureBlocking(false);

	return ch;
}

boolean ENioSocketConnector::connect(sp<ESocketChannel> handle,
		EInetSocketAddress* remoteAddress) {
	return handle->connect(remoteAddress);
}

boolean ENioSocketConnector::finishConnect(sp<ESocketChannel> handle) {
	if (handle->finishConnect()) {
		sp<ESelectionKey> key = handle->keyFor(selector);

		if (key != null) {
			key->cancel();
		}
		return true;
	}
	return false;
}

sp<EIoSession> ENioSocketConnector::newSession(EIoProcessor* processor,
		sp<ESocketChannel> handle) {
	return sp<EIoSession>(new ENioSocketSession(this, processor, handle));
}

void ENioSocketConnector::close(sp<ESocketChannel> handle) {
	sp<ESelectionKey> key = handle->keyFor(selector);

	if (key != null) {
		key->cancel();
    }

	handle->close();
}

void ENioSocketConnector::wakeup() {
	selector->wakeup();
}

int ENioSocketConnector::select(int timeout) {
	return selector->select(timeout);
}

sp<EIterator<sp<ESocketChannel> > > ENioSocketConnector::selectedHandles() {
	return new SocketChannelIterator(selector->selectedKeys());
}

sp<EIterator<sp<ESocketChannel> > > ENioSocketConnector::allHandles() {
	return new SocketChannelIterator(selector->keys());
}

void ENioSocketConnector::register_(sp<ESocketChannel> handle, sp<ConnectionRequest> request) {
	handle->register_(selector, ESelectionKey::OP_CONNECT, new wp<ConnectionRequest>(request));
}

sp<EAbstractPollingIoConnector<sp<ESocketChannel> >::ConnectionRequest> ENioSocketConnector::getConnectionRequest(
		sp<ESocketChannel> handle) {
	sp<ESelectionKey> key = handle->keyFor(selector);

	if ((key == null) || (!key->isValid())) {
		return null;
	}

	wp<ConnectionRequest>* cr = (wp<ConnectionRequest>*)(key->attachment());
	return cr ? (*cr).lock() : null;
}

ESocketSessionConfig* ENioSocketConnector::getSessionConfig() {
	return dynamic_cast<ESocketSessionConfig*>(sessionConfig);
}

} /* namespace eio */
} /* namespace efc */
