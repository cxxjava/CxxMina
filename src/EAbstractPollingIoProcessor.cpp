/*
 * EAbstractPollingIoProcessor.cpp
 *
 *  Created on: 2016-3-9
 *      Author: cxxjava@163.com
 */

#include "EAbstractPollingIoProcessor.hh"
#include "EIoFilterChain.hh"
#include "EExceptionMonitor.hh"
#include "EIoFilterChainBuilder.hh"
#include "EIoServiceListenerSupport.hh"
#include "EAbstractIoService.hh"
#include "EIllegalStateException.hh"
#include "EWriteFuture.hh"
#include "EWriteToClosedSessionException.hh"

namespace efc {
namespace eio {

sp<ELogger> EAbstractPollingIoProcessor::LOG = ELoggerManager::getLogger("EAbstractPollingIoProcessor");

EAbstractPollingIoProcessor::~EAbstractPollingIoProcessor() {
	delete disposalFuture;
}

EAbstractPollingIoProcessor::EAbstractPollingIoProcessor(EExecutor* exec) :
		wakeupCalled(false),
		executor(exec),
		lastIdleCheckTime(0),
		disposing(false),
		disposed(false) {
	disposalFuture = new EDefaultIoFuture(null);

	if (executor == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "executor");
	}

	this->threadName = nextThreadName();
}

void EAbstractPollingIoProcessor::add(sp<EIoSession> session) {
	if (disposed || disposing) {
		throw EIllegalStateException(__FILE__, __LINE__, "Already disposed.");
	}

	// Adds the session to the newSession queue and starts the worker
	newSessions.add(dynamic_pointer_cast<ENioSession>(session));
	startupProcessor();
}

void EAbstractPollingIoProcessor::remove(sp<EIoSession>& session) {
	sp<ENioSession> s = dynamic_pointer_cast<ENioSession>(session);
	scheduleRemove(s);
	startupProcessor();
}

void EAbstractPollingIoProcessor::scheduleRemove(sp<ENioSession>& session) {
	/* @see:
	if (!removingSessions.contains(session)) {
	*/
	if (!removingSessions.contains(session.get()) && !session->isDestroyed()) {
		removingSessions.add(session);
	}
}

void EAbstractPollingIoProcessor::write(sp<EIoSession>& session,
		sp<EWriteRequest>& writeRequest) {
	sp<ENioSession> s = dynamic_pointer_cast<ENioSession>(session);
	EWriteRequestQueue* writeRequestQueue = s->getWriteRequestQueue();

	writeRequestQueue->offer(s, writeRequest);

	if (!s->isWriteSuspended()) {
		this->flush(session);
	}
}

void EAbstractPollingIoProcessor::flush(sp<EIoSession>& session) {
	// add the session to the queue if it's not already
	// in the queue, then wake up the select()
	sp<ENioSession> s = dynamic_pointer_cast<ENioSession>(session);
	if (s->setScheduledForFlush(true)) {
		flushingSessions.add(s);
		wakeup();
	}
}

void EAbstractPollingIoProcessor::updateTrafficControl(sp<EIoSession>& session) {
	sp<ENioSession> s = dynamic_pointer_cast<ENioSession>(session);
	try {
		setInterestedInRead(s, !s->isReadSuspended());
	} catch (EException& e) {
		EIoFilterChain* filterChain = s->getFilterChain();
		filterChain->fireExceptionCaught(new EThrowableObject<EException>(e));
	}

	try {
		setInterestedInWrite(s,
				!s->getWriteRequestQueue()->isEmpty(session) && !s->isWriteSuspended());
	} catch (EException& e) {
		EIoFilterChain* filterChain = session->getFilterChain();
		filterChain->fireExceptionCaught(new EThrowableObject<EException>(e));
	}
}

void EAbstractPollingIoProcessor::updateTrafficMask(sp<ENioSession>& session) {
	trafficControllingSessions.add(session);
	wakeup();
}

boolean EAbstractPollingIoProcessor::isDisposing() {
	return disposing;
}

boolean EAbstractPollingIoProcessor::isDisposed() {
	return disposed;
}

void EAbstractPollingIoProcessor::dispose() {
	if (disposed || disposing) {
		return;
	}

	SYNCBLOCK(&disposalLock) {
		disposing = true;
		startupProcessor();
    }}

	disposalFuture->awaitUninterruptibly();
	disposed = true;
}

EString EAbstractPollingIoProcessor::nextThreadName() {
	//TODO...
	return typeid(*this).name();
}

void EAbstractPollingIoProcessor::startupProcessor() {
	Processor* processor = processorRef.get();

	if (processor == null) {
		processor = new Processor(this);

		if (processorRef.compareAndSet(null, processor)) {
			//@see: executor.execute(new NamePreservingRunnable(processor, threadName));
			executor->execute(processor);
		} else {
			delete processor;
		}
	}

	// Just stop the select() and start it again, so that the processor
	// can be activated immediately.
	wakeup();
}

int EAbstractPollingIoProcessor::handleNewSessions() {
	int addedSessions = 0;

	for (sp<ENioSession> session = newSessions.poll(); session != null; session = newSessions.poll()) {
		if (addNow(session)) {
			// A new session has been created
			addedSessions++;
		}
	}

	return addedSessions;
}

boolean EAbstractPollingIoProcessor::addNow(sp<ENioSession>& session) {
	boolean registered = false;

	try {
		init(session);
		registered = true;

		// Build the filter chain of this session.
		EIoFilterChainBuilder* chainBuilder = session->getService()->getFilterChainBuilder();
		chainBuilder->buildFilterChain(session->getFilterChain());

		// DefaultIoFilterChain.CONNECT_FUTURE is cleared inside here
		// in AbstractIoFilterChain.fireSessionOpened().
		// Propagate the SESSION_CREATED event up to the chain
		EIoServiceListenerSupport* listeners = (dynamic_cast<EAbstractIoService*>(session->getService()))->getListeners();
		listeners->fireSessionCreated(session);
	} catch (EException& e) {
		EExceptionMonitor::getInstance()->exceptionCaught(e);

		ON_FINALLY_NOTHROW(
			registered = false;
		) {
			try {
				destroy(session);
			} catch (EException& e1) {
				EExceptionMonitor::getInstance()->exceptionCaught(e1);
			}
        }}
	}

	return registered;
}

void EAbstractPollingIoProcessor::updateTrafficMask() {
	int queueSize = trafficControllingSessions.size();

	while (queueSize > 0) {
		sp<ENioSession> session = trafficControllingSessions.poll();

		if (session == null) {
			// We are done with this queue.
			return;
		}

		ESessionState state = getState(session);

		switch (state) {
		case OPENED:
		{
			sp<EIoSession> s = dynamic_pointer_cast<EIoSession>(session);
			updateTrafficControl(s);
			break;
		}
		case CLOSING:
			break;

		case OPENING:
			// Retry later if session is not yet fully initialized.
			// (In case that Session.suspend??() or session.resume??() is
			// called before addSession() is processed)
			// We just put back the session at the end of the queue.
			trafficControllingSessions.add(session);
			break;

		default:
			throw EIllegalStateException(__FILE__, __LINE__, EString::valueOf(state).c_str());
		}

		// As we have handled one session, decrement the number of
		// remaining sessions. The OPENING session will be processed
		// with the next select(), as the queue size has been decreased,
		// even
		// if the session has been pushed at the end of the queue
		queueSize--;
	}
}

void EAbstractPollingIoProcessor::flush(llong currentTime) {
	if (flushingSessions.isEmpty()) {
		return;
	}

	do {
		sp<ENioSession> session = flushingSessions.poll(); // the same one with
											 // firstSession
		if (session == null) {
			// Just in case ... It should not happen.
			break;
		}

		// Reset the Schedule for flush flag for this session,
		// as we are flushing it now
		session->unscheduledForFlush();

		ESessionState state = getState(session);

		switch (state) {
		case OPENED:
			try {
				boolean flushedAll = flushNow(session, currentTime);

				if (flushedAll && !session->getWriteRequestQueue()->isEmpty(session)
						&& !session->isScheduledForFlush()) {
					scheduleFlush(session);
				}
			} catch (EException& e) {
				scheduleRemove(session);
				session->closeNow();
				EIoFilterChain* filterChain = session->getFilterChain();
				filterChain->fireExceptionCaught(new EThrowableObject<EException>(e));
			}

			break;

		case CLOSING:
			// Skip if the channel is already closed.
			break;

		case OPENING:
			// Retry later if session is not yet fully initialized.
			// (In case that Session.write() is called before addSession()
			// is processed)
			scheduleFlush(session);
			return;

		default:
			throw EIllegalStateException(__FILE__, __LINE__, EString::valueOf(state).c_str());
		}
	} while (!flushingSessions.isEmpty());
}

void EAbstractPollingIoProcessor::process(sp<ENioSession> session) {
	// Process Reads
	if (isReadable(session) && !session->isReadSuspended()) {
		read(session);
	}

	// Process writes
	if (isWritable(session) && !session->isWriteSuspended() && session->setScheduledForFlush(true)) {
		// add the session to the queue, if it's not already there
		flushingSessions.add(session);
	}
}

void EAbstractPollingIoProcessor::read(sp<ENioSession>& session) {
	EIoSessionConfig* config = session->getConfig();
	int bufferSize = config->getReadBufferSize();
	sp<EIoBuffer> buf = EIoBuffer::allocate(bufferSize);

	boolean hasFragmentation = session->getTransportMetadata()->hasFragmentation();

	try {
		int readBytes = 0;
		int ret;

		ON_FINALLY_NOTHROW(
			buf->flip();
		) {
			if (hasFragmentation) {
				while ((ret = read(session, buf.get())) > 0) {
					readBytes += ret;

					if (!buf->hasRemaining()) {
						break;
					}
				}
			} else {
				ret = read(session, buf.get());

				if (ret > 0) {
					readBytes = ret;
				}
			}
        }}

		if (readBytes > 0) {
			EIoFilterChain* filterChain = session->getFilterChain();
            sp<EObject> buf_ = buf;
			filterChain->fireMessageReceived(buf_);
			buf = null;

			if (hasFragmentation) {
				if (readBytes << 1 < config->getReadBufferSize()) {
					session->decreaseReadBufferSize();
				} else if (readBytes == config->getReadBufferSize()) {
					session->increaseReadBufferSize();
				}
			}
		}

		if (ret < 0) {
			EIoFilterChain* filterChain = session->getFilterChain();
			filterChain->fireInputClosed();
		}
	} catch (EException& e) {
		EIOException* ioe = dynamic_cast<EIOException*>(&e);
		if (ioe) {
			//TODO:
			/* @see:
			if (!(e instanceof PortUnreachableException)
					|| !AbstractDatagramSessionConfig.class.isAssignableFrom(config.getClass())
					|| ((AbstractDatagramSessionConfig) config).isCloseOnPortUnreachable()) {
				scheduleRemove(session);
			}
			*/
			scheduleRemove(session);
		}

		EIoFilterChain* filterChain = session->getFilterChain();
		filterChain->fireExceptionCaught(new EThrowableObject<EException>(e));
	}
}

void EAbstractPollingIoProcessor::process() {
	for (sp<EIterator<sp<ENioSession> > > i = selectedSessions(); i->hasNext();) {
		process(i->next());
		i->remove();
	}
}

int EAbstractPollingIoProcessor::removeSessions() {
	int removedSessions = 0;

	for (sp<ENioSession> session = removingSessions.poll(); session != null;session = removingSessions.poll()) {

		ESessionState state = getState(session);

		// Now deal with the removal accordingly to the session's state
		switch (state) {
			case OPENED:
				// Try to remove this session
				if (removeNow(session)) {
					removedSessions++;
				}

				break;

			case CLOSING:
				// Skip if channel is already closed
				// In any case, remove the session from the queue
				removedSessions++;
				break;

			case OPENING:
				// Remove session from the newSessions queue and
				// remove it
				newSessions.remove(session.get());

				if (removeNow(session)) {
					removedSessions++;
				}

				break;

			default:
				throw EIllegalStateException(__FILE__, __LINE__, EString::valueOf(state).c_str());
		}
	}

	return removedSessions;
}

boolean EAbstractPollingIoProcessor::removeNow(sp<ENioSession>& session) {
	clearWriteRequestQueue(session);

	ON_FINALLY_NOTHROW(
		try {
			clearWriteRequestQueue(session); // Why to clear once again ? cxxjava@163.com
			EIoServiceListenerSupport* listeners = (dynamic_cast<EAbstractIoService*>(session->getService()))->getListeners();
			listeners->fireSessionDestroyed(session);
		} catch (EException& e) {
			// The session was either destroyed or not at this point.
			// We do not want any exception thrown from this "cleanup" code to change
			// the return value by bubbling up.
			EIoFilterChain* filterChain = session->getFilterChain();
			filterChain->fireExceptionCaught(new EThrowableObject<EException>(e));
		}
	) {
		try {
			destroy(session);
			return true;
		} catch (EException& e) {
			EIoFilterChain* filterChain = session->getFilterChain();
			filterChain->fireExceptionCaught(new EThrowableObject<EException>(e));
		}
    }}

	return false;
}

void EAbstractPollingIoProcessor::clearWriteRequestQueue(sp<ENioSession>& session) {
	EWriteRequestQueue* writeRequestQueue = session->getWriteRequestQueue();
	sp<EWriteRequest> req;

	eal<EWriteRequest> failedRequests;

	if ((req = writeRequestQueue->poll(session)) != null) {
		sp<EObject> message = req->getMessage();

		EIoBuffer* buf = dynamic_cast<EIoBuffer*>(message.get());
		if (buf) {
			// The first unwritten empty buffer must be
			// forwarded to the filter chain.
			if (buf->hasRemaining()) {
				buf->reset();
				failedRequests.add(req);
			} else {
				EIoFilterChain* filterChain = session->getFilterChain();
				filterChain->fireMessageSent(req);
			}
		} else {
			failedRequests.add(req);
		}

		// Discard others.
		while ((req = writeRequestQueue->poll(session)) != null) {
			failedRequests.add(req);
		}
	}

	// Create an exception and notify.
	if (!failedRequests.isEmpty()) {
		EWriteToClosedSessionException cause(__FILE__, __LINE__, failedRequests.get(0));

		for (int i=0; i<failedRequests.size(); i++) {
			sp<EWriteRequest> r = failedRequests.get(i);
			session->decreaseScheduledBytesAndMessages(r);

			r->getFuture()->setException(new EThrowableObject<EWriteToClosedSessionException>(cause));
		}

		EIoFilterChain* filterChain = session->getFilterChain();
		filterChain->fireExceptionCaught(new EThrowableObject<EWriteToClosedSessionException>(cause));
	}
}

void EAbstractPollingIoProcessor::notifyIdleSessions(llong currentTime) {
	// process idle sessions
	if (currentTime - lastIdleCheckTime >= SELECT_TIMEOUT) {
		lastIdleCheckTime = currentTime;
		//@see: AbstractIoSession.notifyIdleness(allSessions(), currentTime);
		sp<EIterator<sp<ENioSession> > > sessions = allSessions();
		while (sessions->hasNext()) {
			sp<EAbstractIoSession> s = dynamic_pointer_cast<EAbstractIoSession>(sessions->next());

			if (s != null && !s->getCloseFuture()->isClosed()) {
				EAbstractIoSession::notifyIdleSession(s, currentTime);
			}
		}
	}
}

boolean EAbstractPollingIoProcessor::flushNow(sp<ENioSession>& session, llong currentTime) {
	if (!session->isConnected()) {
		scheduleRemove(session);
		return false;
	}

	boolean hasFragmentation = session->getTransportMetadata()->hasFragmentation();

	EWriteRequestQueue* writeRequestQueue = session->getWriteRequestQueue();

	// Set limitation for the number of written bytes for read-write
	// fairness. I used maxReadBufferSize * 3 / 2, which yields best
	// performance in my experience while not breaking fairness much.
	int maxWrittenBytes = session->getConfig()->getMaxReadBufferSize()
			+ ((unsigned)(session->getConfig()->getMaxReadBufferSize()) >> 1);
	int writtenBytes = 0;
	sp<EWriteRequest> req = null;

	try {
		// Clear OP_WRITE
		setInterestedInWrite(session, false);

		do {
			// Check for pending writes.
			req = session->getCurrentWriteRequest();

			if (req == null) {
				req = writeRequestQueue->poll(session);

				if (req == null) {
					break;
				}

				session->setCurrentWriteRequest(req);
			}

			int localWrittenBytes = 0;
			sp<EObject> message = req->getMessage();

			sp<EIoBuffer> ib = dynamic_pointer_cast<EIoBuffer>(message);
			sp<EFileRegion> fr = dynamic_pointer_cast<EFileRegion>(message);
			if (ib != null) {
				localWrittenBytes = writeBuffer(session, req, hasFragmentation, maxWrittenBytes - writtenBytes,
						currentTime);

				if ((localWrittenBytes > 0) && ib->hasRemaining()) {
					// the buffer isn't empty, we re-interest it in writing
					writtenBytes += localWrittenBytes;
					setInterestedInWrite(session, true);
					return false;
				}
			} else if (fr != null) {
				localWrittenBytes = writeFile(session, req, hasFragmentation, maxWrittenBytes - writtenBytes,
						currentTime);

				// Fix for Java bug on Linux
				// http://bugs.sun.com/bugdatabase/view_bug.do?bug_id=5103988
				// If there's still data to be written in the FileRegion,
				// return 0 indicating that we need
				// to pause until writing may resume.
				if ((localWrittenBytes > 0) && (fr->getRemainingBytes() > 0)) {
					writtenBytes += localWrittenBytes;
					setInterestedInWrite(session, true);
					return false;
				}
			} else {
				EString msg("Don't know how to handle message of type '");
				msg << typeid(*message).name() << "'.  Are you missing a protocol encoder?";
				throw EIllegalStateException(__FILE__, __LINE__, msg.c_str());
			}

			if (localWrittenBytes == 0) {
				// Kernel buffer is full.
				if (!req->equals(EAbstractIoSession::MESSAGE_SENT_REQUEST.get())) {
					setInterestedInWrite(session, true);
					return false;
				}
			} else {
				writtenBytes += localWrittenBytes;

				if (writtenBytes >= maxWrittenBytes) {
					// Wrote too much
					scheduleFlush(session);
					return false;
				}
			}

			if (ib != null) {
				ib->free();
			}
		} while (writtenBytes < maxWrittenBytes);
	} catch (EException& e) {
		if (req != null) {
			req->getFuture()->setException(new EThrowableObject<EException>(e));
		}

		EIoFilterChain* filterChain = session->getFilterChain();
		filterChain->fireExceptionCaught(new EThrowableObject<EException>(e));
		return false;
	}

	return true;
}

void EAbstractPollingIoProcessor::scheduleFlush(sp<ENioSession>& session) {
	// add the session to the queue if it's not already
	// in the queue
	if (session->setScheduledForFlush(true)) {
		flushingSessions.add(session);
	}
}

void EAbstractPollingIoProcessor::fireMessageSent(sp<ENioSession>& session, sp<EWriteRequest>& req) {
	session->setCurrentWriteRequest(null);
	EIoFilterChain* filterChain = session->getFilterChain();
	filterChain->fireMessageSent(req);
}

int EAbstractPollingIoProcessor::writeBuffer(sp<ENioSession>& session,
		sp<EWriteRequest>& req, boolean hasFragmentation, int maxLength,
		llong currentTime) {
	EIoBuffer* buf = dynamic_cast<EIoBuffer*>(req->getMessage().get());
	int localWrittenBytes = 0;

	if (buf->hasRemaining()) {
		int length;

		if (hasFragmentation) {
			length = EMath::min(buf->remaining(), maxLength);
		} else {
			length = buf->remaining();
		}

		try {
			localWrittenBytes = write(session, buf, length);
		} catch (EIOException& ioe) {
			// We have had an issue while trying to send data to the
			// peer : let's close the session.
			buf->free();
			session->closeNow();
			removeNow(session);

			return 0;
		}
	}

	session->increaseWrittenBytes(localWrittenBytes, currentTime);

	// Now, forward the original message
	if (!buf->hasRemaining() || (!hasFragmentation && (localWrittenBytes != 0))) {
		// Buffer has been sent, clear the current request.
		sp<EObject> originalMessage = req->getOriginalRequest()->getMessage();
		sp<EIoBuffer> buf = dynamic_pointer_cast<EIoBuffer>(originalMessage);
		if (buf != null) {
			int pos = buf->position();
			buf->reset();

			fireMessageSent(session, req);

			// And set it back to its position
			buf->position(pos);
		} else {
			fireMessageSent(session, req);
		}
	}

	return localWrittenBytes;
}

int EAbstractPollingIoProcessor::writeFile(sp<ENioSession>& session,
		sp<EWriteRequest>& req, boolean hasFragmentation, int maxLength,
		llong currentTime) {
	int localWrittenBytes;
	EFileRegion* region = dynamic_cast<EFileRegion*>(req->getMessage().get());

	if (region->getRemainingBytes() > 0) {
		int length;

		if (hasFragmentation) {
			length = (int) EMath::min((int)region->getRemainingBytes(), maxLength);
		} else {
			length = (int) EMath::min(EInteger::MAX_VALUE, (int)region->getRemainingBytes());
		}

		localWrittenBytes = transferFile(session, region, length);
		region->update(localWrittenBytes);
	} else {
		localWrittenBytes = 0;
	}

	session->increaseWrittenBytes(localWrittenBytes, currentTime);

	if ((region->getRemainingBytes() <= 0) || (!hasFragmentation && (localWrittenBytes != 0))) {
		fireMessageSent(session, req);
	}

	return localWrittenBytes;
}

EAbstractPollingIoProcessor::Processor::Processor(EAbstractPollingIoProcessor* p) : owner(p) {
}

void EAbstractPollingIoProcessor::Processor::run() {
	ES_ASSERT (owner->processorRef.get() == this);

	int nSessions = 0;
	owner->lastIdleCheckTime = ESystem::currentTimeMillis();
	#define MAX_TRIES 10
	int nbTries = MAX_TRIES;

	for (;;) {
		try {
			// This select has a timeout so that we can manage
			// idle session when we get out of the select every
			// second. (note : this is a hack to avoid creating
			// a dedicated thread).
			llong t0 = ESystem::currentTimeMillis();
			int selected = owner->select(SELECT_TIMEOUT);
			llong t1 = ESystem::currentTimeMillis();
			llong delta = (t1 - t0);

			if (!owner->wakeupCalled.getAndSet(false) && (selected == 0) && (delta < 100)) {
				// Last chance : the select() may have been
				// interrupted because we have had an closed channel.
				if (owner->isBrokenConnection()) {
					LOG->warn(__FILE__, __LINE__, "Broken connection");
				} else {
					// Ok, we are hit by the nasty epoll
					// spinning.
					// Basically, there is a race condition
					// which causes a closing file descriptor not to be
					// considered as available as a selected channel,
					// but
					// it stopped the select. The next time we will
					// call select(), it will exit immediately for the
					// same
					// reason, and do so forever, consuming 100%
					// CPU.
					// We have to destroy the selector, and
					// register all the socket on a new one.
					if (nbTries == 0) {
						EString msg("Create a new selector. Selected is 0, delta = ");
						msg << delta;
						LOG->warn(__FILE__, __LINE__,  msg.c_str());
						owner->registerNewSelector();
						nbTries = MAX_TRIES;
					} else {
						nbTries--;
					}
				}
			} else {
				nbTries = MAX_TRIES;
			}

			// Manage newly created session first
			nSessions += owner->handleNewSessions();

			owner->updateTrafficMask();

			// Now, if we have had some incoming or outgoing events,
			// deal with them
			if (selected > 0) {
				// LOG.debug("Processing ..."); // This log hurts one of
				// the MDCFilter test...
				owner->process();
			}

			// Write the pending requests
			llong currentTime = ESystem::currentTimeMillis();
			owner->flush(currentTime);

			// Last, not least, send Idle events to the idle sessions
			owner->notifyIdleSessions(currentTime); //moved to here: cxxjava@163.com

			// And manage removed sessions
			nSessions -= owner->removeSessions(); //session freed from here: cxxjava@163.com

			/* @see:
			// Last, not least, send Idle events to the idle sessions
			owner->notifyIdleSessions(currentTime);
			*/

			// Get a chance to exit the infinite loop if there are no
			// more sessions on this Processor
			/*
			if (nSessions == 0) {
			*/
			if (nSessions <= 0) {
				owner->processorRef.set(null);

				if (owner->newSessions.isEmpty() && owner->isSelectorEmpty()) {
					// newSessions.add() precedes startupProcessor
					ES_ASSERT (owner->processorRef.get() != this);
					break;
				}

				ES_ASSERT (owner->processorRef.get() != this);

				if (!owner->processorRef.compareAndSet(null, this)) {
					// startupProcessor won race, so must exit processor
					ES_ASSERT (owner->processorRef.get() != this);
					break;
				}

				ES_ASSERT (owner->processorRef.get() == this);
			}

			// Disconnect all sessions immediately if disposal has been
			// requested so that we exit this loop eventually.
			if (owner->isDisposing()) {
				boolean hasKeys = false;

				for (sp<EIterator<sp<ENioSession> > > i = owner->allSessions(); i->hasNext();) {
					sp<ENioSession> session = i->next();

					if (session != null && session->isActive()) {
						owner->scheduleRemove(session);
						hasKeys = true;
					}
				}

				if (hasKeys) {
					owner->wakeup();
				}
			}
		} catch (EClosedSelectorException& cse) {
			// If the selector has been closed, we can exit the loop
			// But first, dump a stack trace
			EExceptionMonitor::getInstance()->exceptionCaught(cse);
			break;
		} catch (EException& e) {
			EExceptionMonitor::getInstance()->exceptionCaught(e);

			try {
				EThread::sleep(1000);
			} catch (EInterruptedException& e1) {
				EExceptionMonitor::getInstance()->exceptionCaught(e1);
			}
		}
	}

	ON_FINALLY_NOTHROW(
		owner->disposalFuture->setValue(new EBoolean(true));
	) {
		try {
			SYNCBLOCK(&owner->disposalLock) {
				if (owner->disposing) {
					owner->doDispose();
				}
            }}
		} catch (EException& e) {
			EExceptionMonitor::getInstance()->exceptionCaught(e);
		}
    }}
}

} /* namespace eio */
} /* namespace efc */
