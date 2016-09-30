/*
 * ENioSocketAcceptor.cpp
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#include "ENioSocketAcceptor.hh"
#include "ENioSocketSession.hh"

namespace efc {
namespace eio {

/**
 * Defines an iterator for the selected-key Set returned by the
 * selector.selectedKeys(). It replaces the SelectionKey operator.
 */
class ServerSocketChannelIterator : public EIterator<EServerSocketChannel*> {
private:
	/** The selected-key iterator */
	sp<EIterator<ESelectionKey*> > iterator;

public:
	virtual ~ServerSocketChannelIterator() {
		//
	}

	/**
	 * Build a SocketChannel iterator which will return a SocketChannel instead of
	 * a SelectionKey.
	 *
	 * @param selectedKeys The selector selected-key set
	 */
	ServerSocketChannelIterator(ECollection<ESelectionKey*>* selectedKeys) {
		iterator = selectedKeys->iterator();
	}

	/**
	 * Tells if there are more SockectChannel left in the iterator
	 * @return <code>true</code> if there is at least one more
	 * SockectChannel object to read
	 */
	boolean hasNext() {
		return iterator->hasNext();
	}

	/**
	 * Get the next SocketChannel in the operator we have built from
	 * the selected-key et for this selector.
	 *
	 * @return The next SocketChannel in the iterator
	 */
	EServerSocketChannel* next() {
		ESelectionKey* key = iterator->next();

		if (key->isValid() && key->isAcceptable()) {
			return (EServerSocketChannel*) key->channel();
		}

		return null;
	}

	/**
	 * Remove the current SocketChannel from the iterator
	 */
	void remove() {
		iterator->remove();
	}

	EServerSocketChannel* moveOut() {
		ESelectionKey* key = iterator->moveOut();
		EServerSocketChannel* channel = (EServerSocketChannel*) key->channel();
		return channel;
	}
};

ENioSocketAcceptor::~ENioSocketAcceptor() {
	delete dssc;
	delete selector;
}

ENioSocketAcceptor::ENioSocketAcceptor() : EAbstractPollingIoAcceptor<EServerSocketChannel*>(dssc = new EDefaultSocketSessionConfig()) {
	this->init();
	dssc->init(this);
}

ENioSocketAcceptor::ENioSocketAcceptor(int processorCount) :
		EAbstractPollingIoAcceptor<EServerSocketChannel*>(dssc = new EDefaultSocketSessionConfig(), processorCount) {
	this->init();
	dssc->init(this);
}

ENioSocketAcceptor::ENioSocketAcceptor(EIoProcessor* processor) :
		EAbstractPollingIoAcceptor<EServerSocketChannel*>(dssc = new EDefaultSocketSessionConfig(), processor) {
	this->init();
	dssc->init(this);
}

ENioSocketAcceptor::ENioSocketAcceptor(EExecutorService* executor,
		EIoProcessor* processor) :
		EAbstractPollingIoAcceptor<EServerSocketChannel*>(dssc = new EDefaultSocketSessionConfig(), executor, processor) {
	this->init();
	dssc->init(this);
}

void ENioSocketAcceptor::init() {
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
			throw ERuntimeIoException(__FILE__, __LINE__, "Failed to initialize.", &e);
		}
	}
}

void ENioSocketAcceptor::destroy() {
	if (selector != null) {
		selector->close();
	}
}

int ENioSocketAcceptor::select() {
	return selector->select();
}

void ENioSocketAcceptor::wakeup() {
	selector->wakeup();
}

sp<EIterator<EServerSocketChannel*> > ENioSocketAcceptor::selectedHandles() {
	return new ServerSocketChannelIterator(selector->selectedKeys());
}

EServerSocketChannel* ENioSocketAcceptor::open(
		EInetSocketAddress* localAddress) {
	// Creates the listening ServerSocket

	EServerSocketChannel* channel = EServerSocketChannel::open();

	boolean success = false;

	{
		ON_SCOPE_EXIT(
			if (!success) {
				close(channel);
			}
		);

		// This is a non blocking socket channel
		channel->configureBlocking(false);

		// Configure the server socket,
		EServerSocket* socket = channel->socket();

		// Set the reuseAddress flag accordingly with the setting
		socket->setReuseAddress(EAbstractPollingIoAcceptor::isReuseAddress());

		// and bind.
		try {
			socket->bind(localAddress, EAbstractPollingIoAcceptor::getBacklog());
		} catch (EIOException& ioe) {
			// Add some info regarding the address we try to bind to the
			// message
			EString newMessage;
			newMessage << "Error while binding on " << localAddress->toString() << "\noriginal message : "
						<< ioe.getMessage();
			EIOException e(__FILE__, __LINE__, newMessage.c_str());

			// And close the channel
			channel->close();

			throw e;
		}

		// Register the channel within the selector for ACCEPT event
		channel->register_(selector, ESelectionKey::OP_ACCEPT);
		success = true;
	}

	return channel;
}

EInetSocketAddress* ENioSocketAcceptor::localAddress(
		EServerSocketChannel* handle) {
	return handle->socket()->getLocalSocketAddress();
}

sp<EIoSession> ENioSocketAcceptor::accept(EIoProcessor* processor,
		EServerSocketChannel* handle) {
	ESelectionKey* key = null;

	if (handle != null) {
		key = handle->keyFor(selector);
	}

	if ((key == null) || (!key->isValid()) || (!key->isAcceptable())) {
		return null;
	}

	// accept the connection from the client
	ESocketChannel* ch = handle->accept();

	if (ch == null) {
		return null;
	}

//	return new ENioSocketSession(this, processor, ch);
	return sp<EIoSession>(new ENioSocketSession(this, processor, ch));
}

void ENioSocketAcceptor::close(EServerSocketChannel* handle) {
	ESelectionKey* key = handle->keyFor(selector);

	if (key != null) {
		key->cancel();
	}

	handle->close();
}

ETransportMetadata* ENioSocketAcceptor::getTransportMetadata() {
	return ENioSocketSession::METADATA();
}

EInetSocketAddress* ENioSocketAcceptor::getLocalAddress() {
	return EAbstractPollingIoAcceptor<EServerSocketChannel*>::getLocalAddress();
}

EInetSocketAddress* ENioSocketAcceptor::getDefaultLocalAddress() {
	return EAbstractPollingIoAcceptor<EServerSocketChannel*>::getDefaultLocalAddress();
}

void ENioSocketAcceptor::setDefaultLocalAddress(EInetSocketAddress* localAddress) {
	EAbstractPollingIoAcceptor<EServerSocketChannel*>::setDefaultLocalAddress(localAddress);
}

boolean ENioSocketAcceptor::isReuseAddress() {
	return EAbstractPollingIoAcceptor<EServerSocketChannel*>::isReuseAddress();
}

void ENioSocketAcceptor::setReuseAddress(boolean reuseAddress) {
	EAbstractPollingIoAcceptor<EServerSocketChannel*>::setReuseAddress(reuseAddress);
}

int ENioSocketAcceptor::getBacklog() {
	return EAbstractPollingIoAcceptor<EServerSocketChannel*>::getBacklog();
}

void ENioSocketAcceptor::setBacklog(int backlog) {
	EAbstractPollingIoAcceptor<EServerSocketChannel*>::setBacklog(backlog);
}

} /* namespace eio */
} /* namespace efc */
