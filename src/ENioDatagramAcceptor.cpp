/*
 * ENioDatagramAcceptor.cpp
 *
 *  Created on: 2016-8-30
 *      Author: cxxjava@163.com
 */

#include "../inc/ENioDatagramAcceptor.hh"
#include "../inc/ENioDatagramSession.hh"
#include "../inc/EExpiringSessionRecycler.hh"
#include "../inc/EExceptionMonitor.hh"
#include "../inc/ENioDatagramSessionConfig.hh"
#include "../inc/ERuntimeIoException.hh"

namespace efc {
namespace eio {

EIoSessionRecycler* ENioDatagramAcceptor::DEFAULT_RECYCLER = new EExpiringSessionRecycler();

ENioDatagramAcceptor::~ENioDatagramAcceptor() {
	delete ddsc;
	delete selector;
	delete boundHandles;
}

ENioDatagramAcceptor::ENioDatagramAcceptor():
		EAbstractIoAcceptor(ddsc = new EDefaultDatagramSessionConfig, null),
		lock(1) {
	this->init();
}

ENioDatagramAcceptor::ENioDatagramAcceptor(EExecutorService* executor):
		EAbstractIoAcceptor(ddsc = new EDefaultDatagramSessionConfig, executor),
		lock(1) {
	this->init();
}

void ENioDatagramAcceptor::init() {
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

	boundHandles = ECollections::synchronizedMap(new EHashMap<sp<EInetSocketAddress>, sp<EDatagramChannel> >());
}

EIoSessionRecycler* ENioDatagramAcceptor::getSessionRecycler() {
	return sessionRecycler;
}

void ENioDatagramAcceptor::setSessionRecycler(EIoSessionRecycler* sessionRecycler) {
	SYNCBLOCK(&bindLock) {
		if (isActive()) {
			throw EIllegalStateException(__FILE__, __LINE__, "sessionRecycler can't be set while the acceptor is bound.");
		}

		if (sessionRecycler == null) {
			sessionRecycler = DEFAULT_RECYCLER;
		}

		this->sessionRecycler = sessionRecycler;
    }}
}

void ENioDatagramAcceptor::add(sp<EIoSession> session) {
	// Nothing to do for UDP
}

sp<ESet<EInetSocketAddress*> > ENioDatagramAcceptor::bindInternal(
		EList<EInetSocketAddress*>& localAddresses) {
	// Create a bind request as a Future operation. When the selector
	// have handled the registration, it will signal this future.
	sp<AcceptorOperationFuture> request = new AcceptorOperationFuture(localAddresses);

	// adds the Registration request to the queue for the Workers
	// to handle
	registerQueue.add(request);

	// creates the Acceptor instance and has the local
	// executor kick it off.
	startupAcceptor();

	// As we just started the acceptor, we have to unblock the select()
	// in order to process the bind request we just have added to the
	// registerQueue.
	SEMAPHORE_SYNCBLOCK(&lock) {
		// Wait a bit to give a chance to the Acceptor thread to do the select()
		EThread::sleep(10);
		wakeup();
    }}

	// Now, we wait until this request is completed.
	request->awaitUninterruptibly();

	/** @see:
	if (request.getException() != null) {
		throw request.getException();
	}
	*/
	request->tryThrowException();

	// Update the local addresses.
	// setLocalAddresses() shouldn't be called from the worker thread
	// because of deadlock.
	sp<ESet<EInetSocketAddress*> > newLocalAddresses = new EHashSet<EInetSocketAddress*>();

	sp<EIterator<sp<EDatagramChannel> > > iter = boundHandles->values()->iterator();
	while (iter->hasNext()) {
		sp<EDatagramChannel> handle = iter->next();
		newLocalAddresses->add(new EInetSocketAddress(*(localAddress(handle).get())));
	}

	return newLocalAddresses;
}

void ENioDatagramAcceptor::flush(sp<EIoSession>& session) {
	if (scheduleFlush(session)) {
		wakeup();
	}
}

void ENioDatagramAcceptor::setDefaultLocalAddress(EInetSocketAddress* localAddress) {
	EAbstractIoAcceptor::setDefaultLocalAddress(localAddress);
}

EInetSocketAddress* ENioDatagramAcceptor::getDefaultLocalAddress() {
	return EAbstractIoAcceptor::getDefaultLocalAddress();
}

EInetSocketAddress* ENioDatagramAcceptor::getLocalAddress() {
	return EAbstractIoAcceptor::getLocalAddress();
}

EDatagramSessionConfig* ENioDatagramAcceptor::getSessionConfig() {
	return dynamic_cast<EDatagramSessionConfig*>(sessionConfig);
}

ETransportMetadata* ENioDatagramAcceptor::getTransportMetadata() {
	return ENioDatagramSession::METADATA();
}

sp<EIoSession> ENioDatagramAcceptor::newSession(EIoProcessor* processor, sp<EDatagramChannel> handle,
	            EInetSocketAddress* remoteAddress) {
	sp<ESelectionKey> key = handle->keyFor(selector);

	if ((key == null) || (!key->isValid())) {
		return null;
	}

	ENioDatagramSession* newSession = new ENioDatagramSession(this, processor, handle, remoteAddress);
	newSession->setSelectionKey(key);

	return sp<EIoSession>(newSession);
}

sp<EIoSession> ENioDatagramAcceptor::newSession(EInetSocketAddress* remoteAddress,
		EInetSocketAddress* localAddress) {
	if (isDisposing()) {
		throw EIllegalStateException(__FILE__, __LINE__, "The Acceptor is being disposed.");
	}

	if (remoteAddress == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "remoteAddress");
	}

	SYNCBLOCK(&bindLock) {
		if (!isActive()) {
			throw EIllegalStateException(__FILE__, __LINE__, "Can't create a session from a unbound service.");
		}

		try {
			return newSessionWithoutLock(remoteAddress, localAddress);
		} catch (ERuntimeException& e) {
			throw e;
		} catch (EException& e) {
			throw ERuntimeIoException(__FILE__, __LINE__, "Failed to create a session.", &e);
		}
    }}
	return null; //not reach here.
}

int ENioDatagramAcceptor::registerHandles() {
	for (;;) {
		sp<AcceptorOperationFuture> req = registerQueue.poll();

		if (req == null) {
			break;
		}

		sp<EHashMap<sp<EInetSocketAddress>, sp<EDatagramChannel> > > newHandles =
				new EHashMap<sp<EInetSocketAddress>, sp<EDatagramChannel> >();
		EList<EInetSocketAddress*>* localAddresses = req->getLocalAddresses();

		{
			ON_SCOPE_EXIT(
				// Roll back if failed to bind all addresses.
				if (req->getException() != null) {
					sp<EIterator<sp<EDatagramChannel> > > iter = newHandles->values()->iterator();
					while (iter->hasNext()) {
						sp<EDatagramChannel> handle = iter->next();
						try {
							this->close(handle);
						} catch (EException& e) {
							EExceptionMonitor::getInstance()->exceptionCaught(e);
						}
					}

					wakeup();
				}
			);

			try {
				sp<EIterator<EInetSocketAddress*> > iter = localAddresses->iterator();
				while (iter->hasNext()) {
					EInetSocketAddress* a = iter->next();
					sp<EDatagramChannel> handle = this->open(a);
					newHandles->put(localAddress(handle), handle);
				}

				//@see: boundHandles.putAll(newHandles);
				{
					sp<EIterator<EMapEntry<sp<EInetSocketAddress>, sp<EDatagramChannel> >*> > iter = newHandles->entrySet()->iterator();
					while(iter->hasNext()) {
						EMapEntry<sp<EInetSocketAddress>, sp<EDatagramChannel> >* me = iter->next();
						boundHandles->put(me->getKey(), me->getValue(), null);
					}
				}

				getListeners()->fireServiceActivated();
				req->setDone();

				return newHandles->size();
			} catch (EException& e) {
				req->setException(new EThrowableObject<EException>(e));
			}
		}
	}

	return 0;
}

int ENioDatagramAcceptor::unregisterHandles() {
	int nHandles = 0;

	for (;;) {
		sp<AcceptorOperationFuture> request = cancelQueue.poll();
		if (request == null) {
			break;
		}

		// close the channels
		sp<EIterator<EInetSocketAddress*> > iter = request->getLocalAddresses()->iterator();
		while (iter->hasNext()) {
			EInetSocketAddress* a = iter->next();
			sp<EDatagramChannel> handle = boundHandles->remove(a);

			if (handle == null) {
				continue;
			}

			{
				ON_SCOPE_EXIT(
					nHandles++;
				);

				try {
					close(handle);
					wakeup(); // wake up again to trigger thread death
				} catch (EException& e) {
					EExceptionMonitor::getInstance()->exceptionCaught(e);
				}
			}
		}

		request->setDone();
	}

	return nHandles;
}

void ENioDatagramAcceptor::processReadySessions(ESet<sp<ESelectionKey> >* handles) {
	sp<EIterator<sp<ESelectionKey> > > iterator = handles->iterator();

	while (iterator->hasNext()) {
		sp<ESelectionKey> key = iterator->next();
		sp<EDatagramChannel> handle = dynamic_pointer_cast<EDatagramChannel>(key->channel());
		iterator->remove();

		try {
			if (key->isValid() && key->isReadable()) {
				readHandle(handle);
			}

			if (key->isValid() && key->isWritable()) {
				sp<EIterator<sp<EIoSession> > > i = getManagedSessions()->values()->iterator();
				while (i->hasNext()) {
					sp<EIoSession> session = i->next();
					scheduleFlush(session);
				}
			}
		} catch (EException& e) {
			EExceptionMonitor::getInstance()->exceptionCaught(e);
		}
	}
}

void ENioDatagramAcceptor::flushSessions(llong currentTime) {
	for (;;) {
		sp<ENioSession> session = flushingSessions.poll();

		if (session == null) {
			break;
		}

		// Reset the Schedule for flush flag for this session,
		// as we are flushing it now
		session->unscheduledForFlush();

		try {
			boolean flushedAll = flush(session, currentTime);

			if (flushedAll && !session->getWriteRequestQueue()->isEmpty(session) && !session->isScheduledForFlush()) {
				sp<EIoSession> s = dynamic_pointer_cast<EIoSession>(session);
				scheduleFlush(s);
			}
		} catch (EException& e) {
			session->getFilterChain()->fireExceptionCaught(new EThrowableObject<EException>(e));
		}
	}
}

void ENioDatagramAcceptor::notifyIdleSessions(llong currentTime) {
	// process idle sessions
	if (currentTime - lastIdleCheckTime >= 1000) {
		lastIdleCheckTime = currentTime;
		//@see: AbstractIoSession.notifyIdleness(getListeners().getManagedSessions().values().iterator(), currentTime);
		sp<EIterator<sp<EIoSession> > > sessions = getListeners()->getManagedSessions()->values()->iterator();
		while (sessions->hasNext()) {
			sp<EAbstractIoSession> s = dynamic_pointer_cast<EAbstractIoSession>(sessions->next());

			if (s != null && !s->getCloseFuture()->isClosed()) {
				EAbstractIoSession::notifyIdleSession(s, currentTime);
			}
		}
	}
}

boolean ENioDatagramAcceptor::flush(sp<ENioSession>& session, llong currentTime) {
	EWriteRequestQueue* writeRequestQueue = session->getWriteRequestQueue();
	int maxWrittenBytes = session->getConfig()->getMaxReadBufferSize()
			+ (session->getConfig()->getMaxReadBufferSize() >> 1);

	int writtenBytes = 0;

	{
		ON_SCOPE_EXIT(
			session->increaseWrittenBytes(writtenBytes, currentTime);
		);

		for (;;) {
			sp<EWriteRequest> req = session->getCurrentWriteRequest();

			if (req == null) {
				req = writeRequestQueue->poll(session);

				if (req == null) {
					setInterestedInWrite(session, false);
					break;
				}

				session->setCurrentWriteRequest(req);
			}

			sp<EIoBuffer> buf = dynamic_pointer_cast<EIoBuffer>(req->getMessage());

			if (buf->remaining() == 0) {
				// Clear and fire event
				session->setCurrentWriteRequest(null);
				buf->reset();
				session->getFilterChain()->fireMessageSent(req);
				continue;
			}

			EInetSocketAddress* destination = req->getDestination();

			if (destination == null) {
				destination = session->getRemoteAddress();
			}

			int localWrittenBytes = this->send(session, buf.get(), destination);

			if ((localWrittenBytes == 0) || (writtenBytes >= maxWrittenBytes)) {
				// Kernel buffer is full or wrote too much
				setInterestedInWrite(session, true);

				return false;
			} else {
				setInterestedInWrite(session, false);

				// Clear and fire event
				session->setCurrentWriteRequest(null);
				writtenBytes += localWrittenBytes;
				buf->reset();
				session->getFilterChain()->fireMessageSent(req);
			}
		}
	}

	return true;
}

void ENioDatagramAcceptor::readHandle(sp<EDatagramChannel> handle) {
	sp<EIoBuffer> readBuf = EIoBuffer::allocate(getSessionConfig()->getReadBufferSize());

	sp<EInetSocketAddress> remoteAddress = receive(handle, readBuf.get());

	if (remoteAddress != null) {
		sp<EIoSession> session = newSessionWithoutLock(remoteAddress.get(), localAddress(handle).get());

		readBuf->flip();

		sp<EObject> buf = readBuf;
		session->getFilterChain()->fireMessageReceived(buf);
	}
}

sp<EIoSession> ENioDatagramAcceptor::newSessionWithoutLock(
		EInetSocketAddress* remoteAddress, EInetSocketAddress* localAddress) {
	sp<EDatagramChannel> handle = boundHandles->get(localAddress);
	if (handle == null) {
		EString msg("Unknown local address: ");
		msg << localAddress->toString();
		throw EIllegalArgumentException(__FILE__, __LINE__, msg.c_str());
	}
	sp < EIoSession > session;
	SYNCHRONIZED(sessionRecycler) {
		session = sessionRecycler->recycle(remoteAddress);

		if (session != null) {
			return session;
		}

		// If a new session needs to be created.
		sp < EIoSession > n = newSession(this, handle, remoteAddress);
		getSessionRecycler()->put(n);
		session = n;
    }}
	sp < EAbstractIoSession > ais = dynamic_pointer_cast < EAbstractIoSession
			> (session);
	initSession(ais, null, null);
	try {
		this->getFilterChainBuilder()->buildFilterChain(
				session->getFilterChain());
		getListeners()->fireSessionCreated(session);
	} catch (EException& e) {
		EExceptionMonitor::getInstance()->exceptionCaught(e);
	}
	return session;
}

void ENioDatagramAcceptor::remove(sp<EIoSession>& session) {
	getSessionRecycler()->remove(session);
	getListeners()->fireSessionDestroyed(session);
}

void ENioDatagramAcceptor::unbind0(EList<EInetSocketAddress*>& localAddresses) {
	sp < AcceptorOperationFuture
			> request(new AcceptorOperationFuture(localAddresses));
	cancelQueue.add(request);
	startupAcceptor();
	wakeup();
	request->awaitUninterruptibly();
	/** @see:
	 if (request.getException() != null) {
	 throw request.getException();
	 }
	 */
	request->tryThrowException();
}

void ENioDatagramAcceptor::updateTrafficControl(sp<EIoSession>& session) {
	throw EUnsupportedOperationException(__FILE__, __LINE__);
}

boolean ENioDatagramAcceptor::isDisposing() {
	return EAbstractIoAcceptor::isDisposing();
}

boolean ENioDatagramAcceptor::isDisposed() {
	return EAbstractIoAcceptor::isDisposed();
}

void ENioDatagramAcceptor::dispose() {
	EAbstractIoAcceptor::dispose();
}

void ENioDatagramAcceptor::dispose(boolean awaitTermination) {
	EAbstractIoAcceptor::dispose(awaitTermination);
}

void ENioDatagramAcceptor::write(sp<EIoSession>& is,
		sp<EWriteRequest>& writeRequest) {
	sp<ENioSession> session = dynamic_pointer_cast<ENioSession>(is);

	// We will try to write the message directly
	llong currentTime = ESystem::currentTimeMillis();
	EWriteRequestQueue* writeRequestQueue = session->getWriteRequestQueue();
	int maxWrittenBytes = session->getConfig()->getMaxReadBufferSize()
			+ (session->getConfig()->getMaxReadBufferSize() >> 1);

	int writtenBytes = 0;

	// Deal with the special case of a Message marker (no bytes in the request)
	// We just have to return after having calle dthe messageSent event
	sp<EIoBuffer> buf = dynamic_pointer_cast<EIoBuffer>(writeRequest->getMessage());

	if (buf->remaining() == 0) {
		// Clear and fire event
		session->setCurrentWriteRequest(null);
		buf->reset();
		session->getFilterChain()->fireMessageSent(writeRequest);
		return;
	}

	// Now, write the data
	ON_FINALLY_NOTHROW(
		session->increaseWrittenBytes(writtenBytes, currentTime);
	) {
		try {
			for (;;) {
				if (writeRequest == null) {
					writeRequest = writeRequestQueue->poll(session);

					if (writeRequest == null) {
						setInterestedInWrite(session, false);
						break;
					}

					session->setCurrentWriteRequest(writeRequest);
				}

				buf = dynamic_pointer_cast<EIoBuffer>(writeRequest->getMessage());

				if (buf->remaining() == 0) {
					// Clear and fire event
					session->setCurrentWriteRequest(null);
					buf->reset();
					session->getFilterChain()->fireMessageSent(writeRequest);
					continue;
				}

				EInetSocketAddress* destination = writeRequest->getDestination();

				if (destination == null) {
					destination = session->getRemoteAddress();
				}

				int localWrittenBytes = this->send(session, buf.get(), destination);

				if ((localWrittenBytes == 0) || (writtenBytes >= maxWrittenBytes)) {
					// Kernel buffer is full or wrote too much
					setInterestedInWrite(session, true);

					session->getWriteRequestQueue()->offer(session, writeRequest);
					scheduleFlush(is);
				} else {
					setInterestedInWrite(session, false);

					// Clear and fire event
					session->setCurrentWriteRequest(null);
					writtenBytes += localWrittenBytes;
					buf->reset();
					session->getFilterChain()->fireMessageSent(writeRequest);

					break;
				}
			}
		} catch (EException& e) {
			session->getFilterChain()->fireExceptionCaught(new EThrowableObject<EException>(e));
		}
    }}
}

sp<EDatagramChannel> ENioDatagramAcceptor::open(EInetSocketAddress* localAddress) {
	sp<EDatagramChannel> ch = EDatagramChannel::open();
	boolean success = false;

	ON_FINALLY_NOTHROW (
		if (!success) {
			this->close(ch);
		}
	) {
		ENioDatagramSessionConfig tmp(ch);
		tmp.setAll(getSessionConfig());
		ch->configureBlocking(false);

		try {
			ch->socket()->bind(localAddress);
		} catch (EIOException& ioe) {
			// Add some info regarding the address we try to bind to the
			// message
			EString newMessage("Error while binding on ");
			newMessage << localAddress->toString() << "\n"  "original message : " << ioe.getMessage();
			EIOException e(__FILE__, __LINE__, newMessage.c_str());
			e.initCause(ioe.getCause());

			// And close the channel
			ch->close();

			throw e;
		}

		ch->register_(selector, ESelectionKey::OP_READ);
		success = true;
    }}

	return ch;
}

void ENioDatagramAcceptor::destroy() {
	if (selector != null) {
		selector->close();
	}
}

void ENioDatagramAcceptor::wakeup() {
	selector->wakeup();
}

void ENioDatagramAcceptor::close(sp<EDatagramChannel> handle) {
	sp<ESelectionKey> key = handle->keyFor(selector);

	if (key != null) {
		key->cancel();
	}

	handle->disconnect();
	handle->close();
}

void ENioDatagramAcceptor::dispose0() {
	unbind();
	startupAcceptor();
	wakeup();
}

int ENioDatagramAcceptor::select(llong timeout) {
	return selector->select(timeout);
}

int ENioDatagramAcceptor::select() {
	return selector->select();
}

boolean ENioDatagramAcceptor::isReadable(sp<EDatagramChannel> handle) {
	sp<ESelectionKey> key = handle->keyFor(selector);

	if ((key == null) || (!key->isValid())) {
		return false;
	}

	return key->isReadable();
}

boolean ENioDatagramAcceptor::isWritable(sp<EDatagramChannel> handle) {
	sp<ESelectionKey> key = handle->keyFor(selector);

	if ((key == null) || (!key->isValid())) {
		return false;
	}

	return key->isWritable();
}

ESet<sp<ESelectionKey> >* ENioDatagramAcceptor::selectedHandles() {
	return selector->selectedKeys();
}

sp<EInetSocketAddress> ENioDatagramAcceptor::receive(sp<EDatagramChannel> handle, EIoBuffer* buffer) {
	return handle->receive(buffer->buf());
}

int ENioDatagramAcceptor::send(sp<ENioSession>& session, EIoBuffer* buffer, EInetSocketAddress* remoteAddress) {
	sp<EDatagramChannel> channel = dynamic_pointer_cast<EDatagramChannel>(session->getChannel());
	return channel->send(buffer->buf(), remoteAddress);
}

void ENioDatagramAcceptor::setInterestedInWrite(sp<ENioSession>& session, boolean isInterested) {
	sp<ESelectionKey> key = session->getSelectionKey();

	if (key == null) {
		return;
	}

	int newInterestOps = key->interestOps();

	if (isInterested) {
		newInterestOps |= ESelectionKey::OP_WRITE;
	} else {
		newInterestOps &= ~ESelectionKey::OP_WRITE;
	}

	key->interestOps(newInterestOps);
}

void ENioDatagramAcceptor::startupAcceptor() {
	if (!selectable) {
		registerQueue.clear();
		cancelQueue.clear();
		flushingSessions.clear();
	}
	lock.acquire();
	if (acceptor == null) {
		acceptor = new Acceptor(this);
		executeWorker (acceptor);
	} else {
		lock.release();
	}
}

void ENioDatagramAcceptor::acceptorProcess() {
	int nHandles = 0;
	lastIdleCheckTime = ESystem::currentTimeMillis();
	// Release the lock
	lock.release();
	while (selectable) {
		try {
			int selected = this->select(SELECT_TIMEOUT);

			nHandles += registerHandles();

			if (nHandles == 0) {
				SEMAPHORE_SYNCBLOCK(&lock) {
					if (registerQueue.isEmpty() && cancelQueue.isEmpty()) {
						acceptor = null;
						break;
					}
                }}
			}

			if (selected > 0) {
				processReadySessions (selectedHandles());}

			llong currentTime = ESystem::currentTimeMillis();
			flushSessions(currentTime);
			nHandles -= unregisterHandles();

			notifyIdleSessions(currentTime);
		} catch (EClosedSelectorException& cse) {
			// If the selector has been closed, we can exit the loop
			EExceptionMonitor::getInstance()->exceptionCaught(cse);
			break;
		} catch (EException& e) {
			EExceptionMonitor::getInstance()->exceptionCaught(e);

			try {
				EThread::sleep(1000);
			} catch (EInterruptedException& e1) {
			}
		}
	}
	if (selectable && isDisposing()) {
		selectable = false;
		try {
			destroy();
			{
				disposalFuture->setValue(new EBoolean(true));
			}
		} catch (EException& e) {
			EExceptionMonitor::getInstance()->exceptionCaught(e);
			{
				disposalFuture->setValue(new EBoolean(true));
			}
		}
	}

}

bool ENioDatagramAcceptor::scheduleFlush(sp<EIoSession>& s) {
	// Set the schedule for flush flag if the session
	// has not already be added to the flushingSessions
	// queue
	sp<ENioSession> session = dynamic_pointer_cast<ENioSession>(s);
	if (session->setScheduledForFlush(true)) {
		flushingSessions.add(session);
		return true;
	} else {
		return false;
	}
}

sp<EInetSocketAddress> ENioDatagramAcceptor::localAddress(sp<EDatagramChannel> handle) {
	return handle->socket()->getLocalSocketAddress();
}

} /* namespace eio */
} /* namespace efc */
