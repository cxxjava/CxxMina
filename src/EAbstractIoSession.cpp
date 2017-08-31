/*
 * EAbstractIoSession.cpp
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#include "../inc/EAbstractIoSession.hh"
#include "../inc/EDefaultWriteRequest.hh"
#include "../inc/EDefaultReadFuture.hh"
#include "../inc/EDefaultWriteFuture.hh"
#include "../inc/EIoFilterChain.hh"
#include "../inc/EIoBuffer.hh"
#include "../inc/EIoService.hh"
#include "../inc/EIoAcceptor.hh"
#include "../inc/EDefaultCloseFuture.hh"
#include "../inc/EAbstractIoService.hh"
#include "../inc/EDefaultFileRegion.hh"
#include "../inc/EFilenameFileRegion.hh"
#include "../inc/EExceptionMonitor.hh"
#include "../inc/EWriteTimeoutException.hh"
#include "../inc/EWriteToClosedSessionException.hh"

namespace efc {
namespace eio {

class CloseFutureListener : public EIoFutureListener {
public:
	void operationComplete(EIoFuture* future) {
		ECloseFuture* cf = dynamic_cast<ECloseFuture*>(future);
		sp<EAbstractIoSession> session = dynamic_pointer_cast<EAbstractIoSession>(cf->getSession());
		session->scheduledWriteBytes.set(0);
		session->scheduledWriteMessages.set(0);
		session->readBytesThroughput = 0;
		session->readMessagesThroughput = 0;
		session->writtenBytesThroughput = 0;
		session->writtenMessagesThroughput = 0;
	}
};

EAtomicLLong EAbstractIoSession::idGenerator;
sp<EWriteRequest> EAbstractIoSession::CLOSE_REQUEST = new EDefaultWriteRequest(new EObject());
sp<EWriteRequest> EAbstractIoSession::MESSAGE_SENT_REQUEST = new EDefaultWriteRequest(new EObject());
sp<EIoFutureListener> EAbstractIoSession::SCHEDULED_COUNTER_RESETTER = new CloseFutureListener();
sp<EAttributeKey> EAbstractIoSession::READY_READ_FUTURES_KEY = new EAttributeKey(typeid(EAbstractIoSession).name(), "readyReadFutures");
sp<EAttributeKey> EAbstractIoSession::WAITING_READ_FUTURES_KEY = new EAttributeKey(typeid(EAbstractIoSession).name(), "waitingReadFutures");

DEFINE_STATIC_INITZZ_BEGIN(EAbstractIoSession)
ESystem::_initzz_();
DEFINE_STATIC_INITZZ_END

EAbstractIoSession::~EAbstractIoSession() {
	delete writeRequestQueue;
	delete attributes;
}

EAbstractIoSession::EAbstractIoSession(EIoService* is) :
		handler(is->getHandler()),
		service(is),
		attributes(null),
		writeRequestQueue(null),
		currentWriteRequest(null),
		creationTime(0),
		sessionId(0),
		closeFuture(null),
		closing(false),
		readSuspended(false),
		writeSuspended(false),
		readBytes(0),
		writtenBytes(0),
		readMessages(0),
		writtenMessages(0),
		lastReadTime(0),
		lastWriteTime(0),
		lastThroughputCalculationTime(0),
		lastReadBytes(0),
		lastWrittenBytes(0),
		lastReadMessages(0),
		lastWrittenMessages(0),
		readBytesThroughput(0.0),
		writtenBytesThroughput(0.0),
		readMessagesThroughput(0.0),
		writtenMessagesThroughput(0.0),
		lastIdleTimeForBoth(0),
		lastIdleTimeForRead(0),
		lastIdleTimeForWrite(0),
		deferDecreaseReadBuffer(true) {

	//moved to init().
	//closeFuture = new EDefaultCloseFuture(this);

	// Initialize all the Session counters to the current time
	llong currentTime = ESystem::currentTimeMillis();
	creationTime = currentTime;
	lastThroughputCalculationTime = currentTime;
	lastReadTime = currentTime;
	lastWriteTime = currentTime;
	lastIdleTimeForBoth = currentTime;
	lastIdleTimeForRead = currentTime;
	lastIdleTimeForWrite = currentTime;

	// TODO add documentation
	//moved to init().
	//closeFuture->addListener(SCHEDULED_COUNTER_RESETTER);

	// Set a new ID for this session
	sessionId = idGenerator.incrementAndGet();
}

void EAbstractIoSession::init() {
	closeFuture = new EDefaultCloseFuture(shared_from_this());
	closeFuture->addListener(SCHEDULED_COUNTER_RESETTER);
}

sp<EIoSession> EAbstractIoSession::tosp() {
	return shared_from_this();
}

long EAbstractIoSession::getId() {
	return sessionId;
}

boolean EAbstractIoSession::isConnected() {
	return !closeFuture->isClosed();
}

boolean EAbstractIoSession::isActive() {
	// Return true by default
	return true;
}

boolean EAbstractIoSession::isClosing() {
	return closing || closeFuture->isClosed();
}

boolean EAbstractIoSession::isSecured() {
	// Always false...
	return false;
}

sp<ECloseFuture> EAbstractIoSession::getCloseFuture() {
	return closeFuture;
}

boolean EAbstractIoSession::isScheduledForFlush() {
	return scheduledForFlush_.get();
}

void EAbstractIoSession::scheduledForFlush() {
	scheduledForFlush_.set(true);
}

void EAbstractIoSession::unscheduledForFlush() {
	scheduledForFlush_.set(false);
}

boolean EAbstractIoSession::setScheduledForFlush(boolean schedule) {
	if (schedule) {
		// If the current tag is set to false, switch it to true,
		// otherwise, we do nothing but return false : the session
		// is already scheduled for flush
		return scheduledForFlush_.compareAndSet(false, schedule);
	}

	scheduledForFlush_.set(schedule);
	return true;
}

sp<ECloseFuture> EAbstractIoSession::close(boolean rightNow) {
	if (rightNow) {
		return closeNow();
	} else {
		return closeOnFlush();
	}
}

sp<ECloseFuture> EAbstractIoSession::close() {
	return closeNow();
}

sp<ECloseFuture> EAbstractIoSession::closeOnFlush() {
	if (!isClosing()) {
		sp<EIoSession> session = shared_from_this();
		getWriteRequestQueue()->offer(session, CLOSE_REQUEST);
		getProcessor()->flush(session);
	}
	return closeFuture;
}

sp<ECloseFuture> EAbstractIoSession::closeNow() {
	SYNCBLOCK(&lock) {
		if (isClosing()) {
			return closeFuture;
		}

		closing = true;

		//@see: mina-2.0.16/src/mina-core/src/main/java/org/apache/mina/core/session/AbstractIoSession.java#L365
		try {
			if (writeRequestQueue != null) {
				sp<EIoSession> session = shared_from_this();
				while (!writeRequestQueue->isEmpty(session)) {
					sp<EWriteRequest> writeRequest = writeRequestQueue->poll(session);
					if (writeRequest != null) {
						sp<EWriteFuture> writeFuture = writeRequest->getFuture();

						// The WriteRequest may not always have a future : The CLOSE_REQUEST
						// and MESSAGE_SENT_REQUEST don't.
						if (writeFuture != null) {
							writeFuture->setWritten();
						}
					}
				}
			}
		} catch (EException& e) {
			getFilterChain()->fireExceptionCaught(new EThrowableObject<EException>(e));
		}
    }}

	getFilterChain()->fireFilterClose();

	return closeFuture;
}

EIoHandler* EAbstractIoSession::getHandler() {
	return handler;
}

EIoSessionConfig* EAbstractIoSession::getConfig() {
	return config;
}

sp<EReadFuture> EAbstractIoSession::read() {
	if (!getConfig()->isUseReadOperation()) {
		throw EIllegalStateException(__FILE__, __LINE__, "useReadOperation is not enabled.");
	}

	sp<ConcurrentLinkedQueue<EReadFuture> > readyReadFutures = getReadyReadFutures();
	sp<EReadFuture> future;
	SYNCHRONIZED(readyReadFutures) {
		future = readyReadFutures->poll();
		if (future != null) {
			if (future->isClosed()) {
				// Let other readers get notified.
				readyReadFutures->offer(future);
			}
		} else {
			future = new EDefaultReadFuture(shared_from_this());
			getWaitingReadFutures()->offer(future);
		}
    }}

	return future;
}

void EAbstractIoSession::offerReadFuture(EObject* message) {
	newReadFuture()->setRead(message);
}

void EAbstractIoSession::offerFailedReadFuture(EObject* exception) {
	newReadFuture()->setException(exception);
}

void EAbstractIoSession::offerClosedReadFuture() {
	sp<ConcurrentLinkedQueue<EReadFuture> > readyReadFutures = getReadyReadFutures();
	SYNCHRONIZED(readyReadFutures) {
		newReadFuture()->setClosed();
    }}
}

sp<EWriteFuture> EAbstractIoSession::write(sp<EObject> message) {
	return write(message, null);
}

sp<EWriteFuture> EAbstractIoSession::write(sp<EObject> message,
		EInetSocketAddress* remoteAddress) {
	if (message == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "Trying to write a null message : not allowed");
	}

	// We can't send a message to a connected session if we don't have
	// the remote address
	if (!getTransportMetadata()->isConnectionless() && (remoteAddress != null)) {
		throw EUnsupportedOperationException(__FILE__, __LINE__);
	}

	// If the session has been closed or is closing, we can't either
	// send a message to the remote side. We generate a future
	// containing an exception.
	if (isClosing() || !isConnected()) {
		sp<EWriteFuture> future = new EDefaultWriteFuture(shared_from_this());
		EWriteToClosedSessionException writeException(__FILE__, __LINE__,
				new EDefaultWriteRequest(message, future, remoteAddress));
		future->setException(new EThrowableObject<EWriteToClosedSessionException>(writeException));
		return future;
	}

	//EFileChannel* openedFileChannel = null;

	// TODO: remove this code as soon as we use InputStream
	// instead of Object for the message.
	try {
		EIoBuffer* ib = dynamic_cast<EIoBuffer*>(message.get());
		if (ib && !ib->hasRemaining()) {
			// Nothing to write : probably an error in the user code
			throw EIllegalArgumentException(__FILE__, __LINE__, "message is empty. Forgot to call flip()?");
		}
		else {
			EFileChannel* fc = dynamic_cast<EFileChannel*>(message.get());
			if (fc) {
				message = new EDefaultFileRegion(fc, 0, fc->size());
			}
			else {
				EFile* f = dynamic_cast<EFile*>(message.get());
				if (f) {
					EFileChannel* fc = EFileChannel::open(f->getAbsolutePath().c_str(), true, false, false);
					message = new EFilenameFileRegion(f, fc, 0, fc->size());
				}
			}
		}
	} catch (EIOException& e) {
		EExceptionMonitor::getInstance()->exceptionCaught(e);
		return EDefaultWriteFuture::newNotWrittenFuture(shared_from_this(), new EThrowableObject<EIOException>(&e));
	}

	// Now, we can write the message. First, create a future
	sp<EWriteFuture> writeFuture = new EDefaultWriteFuture(shared_from_this());
	sp<EWriteRequest> writeRequest = new EDefaultWriteRequest(message, writeFuture, remoteAddress);

	// Then, get the chain and inject the WriteRequest into it
	EIoFilterChain* filterChain = getFilterChain();
	filterChain->fireFilterWrite(writeRequest);

	/* @removed
	// TODO : This is not our business ! The caller has created a
	// FileChannel,
	// he has to close it !
	if (openedFileChannel != null) {
		class IoFutureListener : public EIoFutureListener  {
		private:
			EFileChannel* fc;
		public:
			IoFutureListener(EFileChannel* fc) {
				this->fc = fc;
			}
			void operationComplete(EIoFuture* future) {
				try {
					fc->close();
				} catch (EIOException& e) {
					EExceptionMonitor::getInstance()->exceptionCaught(e);
				}
			}
		};
		// If we opened a FileChannel, it needs to be closed when the write
		// has completed
		writeFuture->addListener(new IoFutureListener(openedFileChannel));
	}
	*/

	// Return the WriteFuture.
	return writeFuture;
}

sp<EObject> EAbstractIoSession::getAttachment() {
	EString attachmentKey("");
	return getAttribute(&attachmentKey);
}

sp<EObject> EAbstractIoSession::setAttachment(sp<EObject> attachment) {
	return setAttribute(new EString(""), attachment);
}

sp<EObject> EAbstractIoSession::getAttribute(EObject* key) {
	return getAttribute(key, null);
}

sp<EObject> EAbstractIoSession::getAttribute(EObject* key, EObject* defaultValue) {
	return attributes->getAttribute(this, key, defaultValue);
}

sp<EObject> EAbstractIoSession::setAttribute(sp<EObject> key, sp<EObject> value) {
	return attributes->setAttribute(this, key, value);
}

sp<EObject> EAbstractIoSession::setAttribute(sp<EObject> key) {
	return setAttribute(key, new EBoolean(true));
}

sp<EObject> EAbstractIoSession::setAttributeIfAbsent(sp<EObject> key,
		sp<EObject> value) {
	return attributes->setAttributeIfAbsent(this, key, value);
}

sp<EObject> EAbstractIoSession::setAttributeIfAbsent(sp<EObject> key) {
	return setAttributeIfAbsent(key, new EBoolean(true));
}

sp<EObject> EAbstractIoSession::removeAttribute(EObject* key) {
	return attributes->removeAttribute(this, key);
}

boolean EAbstractIoSession::removeAttribute(EObject* key, EObject* value) {
	return attributes->removeAttribute(this, key, value);
}

boolean EAbstractIoSession::replaceAttribute(EObject* key, EObject* oldValue,
		sp<EObject> newValue) {
	return attributes->replaceAttribute(this, key, oldValue, newValue);
}

boolean EAbstractIoSession::containsAttribute(EObject* key) {
	return attributes->containsAttribute(this, key);
}

sp<EConcurrentSet<EObject> > EAbstractIoSession::getAttributeKeys() {
	return attributes->getAttributeKeys(this);
}

EIoSessionAttributeMap* EAbstractIoSession::getAttributeMap() {
	return attributes;
}

void EAbstractIoSession::setAttributeMap(EIoSessionAttributeMap* attributes) {
	this->attributes = attributes;
}

void EAbstractIoSession::setWriteRequestQueue(
		EWriteRequestQueue* writeRequestQueue) {
	ES_ASSERT(!this->writeRequestQueue);
	this->writeRequestQueue = writeRequestQueue;
}

void EAbstractIoSession::suspendRead() {
	readSuspended = true;
	if (isClosing() || !isConnected()) {
		return;
	}
	sp<EIoSession> session = shared_from_this();
	getProcessor()->updateTrafficControl(session);
}

void EAbstractIoSession::suspendWrite() {
	writeSuspended = true;
	if (isClosing() || !isConnected()) {
		return;
	}
	sp<EIoSession> session = shared_from_this();
	getProcessor()->updateTrafficControl(session);
}

void EAbstractIoSession::resumeRead() {
	readSuspended = false;
	if (isClosing() || !isConnected()) {
		return;
	}
	sp<EIoSession> session = shared_from_this();
	getProcessor()->updateTrafficControl(session);
}

void EAbstractIoSession::resumeWrite() {
	writeSuspended = false;
	if (isClosing() || !isConnected()) {
		return;
	}
	sp<EIoSession> session = shared_from_this();
	getProcessor()->updateTrafficControl(session);
}

boolean EAbstractIoSession::isReadSuspended() {
	return readSuspended;
}

boolean EAbstractIoSession::isWriteSuspended() {
	return writeSuspended;
}

long EAbstractIoSession::getReadBytes() {
	return readBytes;
}

long EAbstractIoSession::getWrittenBytes() {
	return writtenBytes;
}

long EAbstractIoSession::getReadMessages() {
	return readMessages;
}

long EAbstractIoSession::getWrittenMessages() {
	return writtenMessages;
}

double EAbstractIoSession::getReadBytesThroughput() {
	return readBytesThroughput;
}

double EAbstractIoSession::getWrittenBytesThroughput() {
	return writtenBytesThroughput;
}

double EAbstractIoSession::getReadMessagesThroughput() {
	return readMessagesThroughput;
}

double EAbstractIoSession::getWrittenMessagesThroughput() {
	return writtenMessagesThroughput;
}

void EAbstractIoSession::updateThroughput(llong currentTime, boolean force) {
	int interval = (int) (currentTime - lastThroughputCalculationTime);

	long minInterval = getConfig()->getThroughputCalculationIntervalInMillis();

	if (((minInterval == 0) || (interval < minInterval)) && !force) {
		return;
	}

	readBytesThroughput = (readBytes - lastReadBytes) * 1000.0 / interval;
	writtenBytesThroughput = (writtenBytes - lastWrittenBytes) * 1000.0 / interval;
	readMessagesThroughput = (readMessages - lastReadMessages) * 1000.0 / interval;
	writtenMessagesThroughput = (writtenMessages - lastWrittenMessages) * 1000.0 / interval;

	lastReadBytes = readBytes;
	lastWrittenBytes = writtenBytes;
	lastReadMessages = readMessages;
	lastWrittenMessages = writtenMessages;

	lastThroughputCalculationTime = currentTime;
}

long EAbstractIoSession::getScheduledWriteBytes() {
	return scheduledWriteBytes.get();
}

int EAbstractIoSession::getScheduledWriteMessages() {
	return scheduledWriteMessages.get();
}

void EAbstractIoSession::setScheduledWriteBytes(int byteCount) {
	scheduledWriteBytes.set(byteCount);
}

void EAbstractIoSession::setScheduledWriteMessages(int messages) {
	scheduledWriteMessages.set(messages);
}

void EAbstractIoSession::increaseReadBytes(long increment, llong currentTime) {
	if (increment <= 0) {
		return;
	}

	readBytes += increment;
	lastReadTime = currentTime;
	idleCountForBoth.set(0);
	idleCountForRead.set(0);

	EIoService* service = getService();
	EAbstractIoService* ais = dynamic_cast<EAbstractIoService*>(service);
	if (ais) {
		ais->getStatistics()->increaseReadBytes(increment, currentTime);
	}
}

void EAbstractIoSession::increaseReadMessages(llong currentTime) {
	readMessages++;
	lastReadTime = currentTime;
	idleCountForBoth.set(0);
	idleCountForRead.set(0);

	EIoService* service = getService();
	EAbstractIoService* ais = dynamic_cast<EAbstractIoService*>(service);
	if (ais) {
		ais->getStatistics()->increaseReadMessages(currentTime);
	}
}

void EAbstractIoSession::increaseWrittenBytes(int increment, llong currentTime) {
	if (increment <= 0) {
		return;
	}

	writtenBytes += increment;
	lastWriteTime = currentTime;
	idleCountForBoth.set(0);
	idleCountForWrite.set(0);

	EIoService* service = getService();
	EAbstractIoService* ais = dynamic_cast<EAbstractIoService*>(service);
	if (ais) {
		ais->getStatistics()->increaseWrittenBytes(increment, currentTime);
	}

	increaseScheduledWriteBytes(-increment);
}

void EAbstractIoSession::increaseWrittenMessages(sp<EWriteRequest>& request,
		llong currentTime) {
	sp<EObject> message = request->getMessage();

	EIoBuffer* b = dynamic_cast<EIoBuffer*>(message.get());
	if (b) {
		if (b->hasRemaining()) {
			return;
		}
	}

	writtenMessages++;
	lastWriteTime = currentTime;

	EIoService* service = getService();
	EAbstractIoService* ais = dynamic_cast<EAbstractIoService*>(service);
	if (ais) {
		ais->getStatistics()->increaseWrittenMessages(currentTime);
	}

	decreaseScheduledWriteMessages();
}

void EAbstractIoSession::increaseScheduledWriteBytes(int increment) {
	scheduledWriteBytes.addAndGet(increment);
	EIoService* service = getService();
	EAbstractIoService* ais = dynamic_cast<EAbstractIoService*>(service);
	if (ais) {
		ais->getStatistics()->increaseScheduledWriteBytes(increment);
	}
}

void EAbstractIoSession::increaseScheduledWriteMessages() {
	scheduledWriteMessages.incrementAndGet();
	EIoService* service = getService();
	EAbstractIoService* ais = dynamic_cast<EAbstractIoService*>(service);
	if (ais) {
		ais->getStatistics()->increaseScheduledWriteMessages();
	}
}

void EAbstractIoSession::decreaseScheduledBytesAndMessages(
		sp<EWriteRequest>& request) {
	sp<EObject> message = request->getMessage();

	EIoBuffer* b = dynamic_cast<EIoBuffer*>(message.get());
	if (b) {
		if (b->hasRemaining()) {
			increaseScheduledWriteBytes(-b->remaining());
		} else {
			decreaseScheduledWriteMessages();
		}
	} else {
		decreaseScheduledWriteMessages();
	}
}

EWriteRequestQueue* EAbstractIoSession::getWriteRequestQueue() {
	if (writeRequestQueue == null) {
		throw EIllegalStateException(__FILE__, __LINE__);
	}
	return writeRequestQueue;
}

sp<EWriteRequest> EAbstractIoSession::getCurrentWriteRequest() {
	return currentWriteRequest;
}

sp<EObject> EAbstractIoSession::getCurrentWriteMessage() {
	sp<EWriteRequest> req = getCurrentWriteRequest();
	if (req == null) {
		return null;
	}
	return req->getMessage();
}

void EAbstractIoSession::setCurrentWriteRequest(
		sp<EWriteRequest> currentWriteRequest) {
	this->currentWriteRequest = currentWriteRequest;
}

void EAbstractIoSession::increaseReadBufferSize() {
	EIoSessionConfig* config = getConfig();
	int newReadBufferSize = config->getReadBufferSize() << 1;
	if (newReadBufferSize <= config->getMaxReadBufferSize()) {
		config->setReadBufferSize(newReadBufferSize);
	} else {
		config->setReadBufferSize(config->getMaxReadBufferSize());
	}

	deferDecreaseReadBuffer = true;
}

void EAbstractIoSession::decreaseReadBufferSize() {
	EIoSessionConfig* config = getConfig();

	if (deferDecreaseReadBuffer) {
		deferDecreaseReadBuffer = false;
		return;
	}

	if (config->getReadBufferSize() > config->getMinReadBufferSize()) {
		config->setReadBufferSize(config->getReadBufferSize() >> 1);
	}

	deferDecreaseReadBuffer = true;
}

llong EAbstractIoSession::getCreationTime() {
	return creationTime;
}

llong EAbstractIoSession::getLastIoTime() {
	return ES_MAX(lastReadTime, lastWriteTime);
}

llong EAbstractIoSession::getLastReadTime() {
	return lastReadTime;
}

llong EAbstractIoSession::getLastWriteTime() {
	return lastWriteTime;
}

boolean EAbstractIoSession::isIdle(EIdleStatus status) {
	if (status == EIdleStatus::BOTH_IDLE) {
		return idleCountForBoth.get() > 0;
	}

	if (status == EIdleStatus::READER_IDLE) {
		return idleCountForRead.get() > 0;
	}

	if (status == EIdleStatus::WRITER_IDLE) {
		return idleCountForWrite.get() > 0;
	}

	EString msg("Unknown idle status: ");
	msg += status;
	throw EIllegalArgumentException(__FILE__, __LINE__, msg.c_str());
}

boolean EAbstractIoSession::isBothIdle() {
	return isIdle(EIdleStatus::BOTH_IDLE);
}

boolean EAbstractIoSession::isReaderIdle() {
	return isIdle(EIdleStatus::READER_IDLE);
}

boolean EAbstractIoSession::isWriterIdle() {
	return isIdle(EIdleStatus::WRITER_IDLE);
}

int EAbstractIoSession::getIdleCount(EIdleStatus status) {
	if (getConfig()->getIdleTime(status) == 0) {
		if (status == EIdleStatus::BOTH_IDLE) {
			idleCountForBoth.set(0);
		}

		if (status == EIdleStatus::READER_IDLE) {
			idleCountForRead.set(0);
		}

		if (status == EIdleStatus::WRITER_IDLE) {
			idleCountForWrite.set(0);
		}
	}

	if (status == EIdleStatus::BOTH_IDLE) {
		return idleCountForBoth.get();
	}

	if (status == EIdleStatus::READER_IDLE) {
		return idleCountForRead.get();
	}

	if (status == EIdleStatus::WRITER_IDLE) {
		return idleCountForWrite.get();
	}

	EString msg("Unknown idle status: ");
	msg += status;
	throw EIllegalArgumentException(__FILE__, __LINE__, msg.c_str());
}

llong EAbstractIoSession::getLastIdleTime(EIdleStatus status) {
	if (status == EIdleStatus::BOTH_IDLE) {
		return lastIdleTimeForBoth;
	}

	if (status == EIdleStatus::READER_IDLE) {
		return lastIdleTimeForRead;
	}

	if (status == EIdleStatus::WRITER_IDLE) {
		return lastIdleTimeForWrite;
	}

	EString msg("Unknown idle status: ");
	msg += status;
	throw EIllegalArgumentException(__FILE__, __LINE__, msg.c_str());
}

void EAbstractIoSession::increaseIdleCount(EIdleStatus status,
		llong currentTime) {
	if (status == EIdleStatus::BOTH_IDLE) {
		idleCountForBoth.incrementAndGet();
		lastIdleTimeForBoth = currentTime;
	} else if (status == EIdleStatus::READER_IDLE) {
		idleCountForRead.incrementAndGet();
		lastIdleTimeForRead = currentTime;
	} else if (status == EIdleStatus::WRITER_IDLE) {
		idleCountForWrite.incrementAndGet();
		lastIdleTimeForWrite = currentTime;
	} else {
		EString msg("Unknown idle status: ");
		msg += status;
		throw EIllegalArgumentException(__FILE__, __LINE__, msg.c_str());
	}
}

int EAbstractIoSession::getBothIdleCount() {
	return getIdleCount(EIdleStatus::BOTH_IDLE);
}

llong EAbstractIoSession::getLastBothIdleTime() {
	return getLastIdleTime(EIdleStatus::BOTH_IDLE);
}

llong EAbstractIoSession::getLastReaderIdleTime() {
	return getLastIdleTime(EIdleStatus::READER_IDLE);
}

llong EAbstractIoSession::getLastWriterIdleTime() {
	return getLastIdleTime(EIdleStatus::WRITER_IDLE);
}

int EAbstractIoSession::getReaderIdleCount() {
	return getIdleCount(EIdleStatus::READER_IDLE);
}

int EAbstractIoSession::getWriterIdleCount() {
	return getIdleCount(EIdleStatus::WRITER_IDLE);
}

EInetSocketAddress* EAbstractIoSession::getServiceAddress() {
	EIoService* service = getService();
	EIoAcceptor* ia = dynamic_cast<EIoAcceptor*>(service);
	if (ia) {
		return ia->getLocalAddress();
	}

	return getRemoteAddress();
}

EIoService* EAbstractIoSession::getService() {
	return service;
}

ELock* EAbstractIoSession::getLock() {
	return &lock;
}

EStringBase EAbstractIoSession::toString() {
	if (isConnected() || isClosing()) {
		EString remote;
		EString local;

		try {
			remote = getRemoteAddress()->toString();
		} catch (EException& e) {
			remote = "Cannot get the remote address informations: ";
			remote += e.getMessage();
		}

		try {
			local = getLocalAddress()->toString();
		} catch (EException& e) {
		}

		EIoAcceptor* ia = dynamic_cast<EIoAcceptor*>(getService());
		if (ia) {
			return EStringBase::formatOf("(%s: %s, server, %s => %s)",
					getIdAsString().c_str(), getServiceName().c_str(),
					remote.c_str(), local.c_str());
		}

		return EStringBase::formatOf("(%s: %s, client, %s => %s)",
				getIdAsString().c_str(), getServiceName().c_str(),
				local.c_str(), remote.c_str());
	}

	return EStringBase::formatOf("(%s) Session disconnected ...", getIdAsString().c_str());
}

void EAbstractIoSession::notifyIdleness(sp<EIterator<sp<EAbstractIoSession> > >& sessions, llong currentTime) {
	while (sessions->hasNext()) {
		sp<EAbstractIoSession> s = sessions->next();

		if (!s->getCloseFuture()->isClosed()) {
			notifyIdleSession(s, currentTime);
		}
	}
}

void EAbstractIoSession::notifyIdleSession(sp<EAbstractIoSession>& session, llong currentTime) {
	notifyIdleSession0(session, currentTime, session->getConfig()->getIdleTimeInMillis(EIdleStatus::BOTH_IDLE),
			EIdleStatus::BOTH_IDLE, ES_MAX(session->getLastIoTime(), session->getLastIdleTime(EIdleStatus::BOTH_IDLE)));

	notifyIdleSession0(session, currentTime, session->getConfig()->getIdleTimeInMillis(EIdleStatus::READER_IDLE),
			EIdleStatus::READER_IDLE,
			ES_MAX(session->getLastReadTime(), session->getLastIdleTime(EIdleStatus::READER_IDLE)));

	notifyIdleSession0(session, currentTime, session->getConfig()->getIdleTimeInMillis(EIdleStatus::WRITER_IDLE),
			EIdleStatus::WRITER_IDLE,
			ES_MAX(session->getLastWriteTime(), session->getLastIdleTime(EIdleStatus::WRITER_IDLE)));

	notifyWriteTimeout(session, currentTime);
}

sp<EReadFuture> EAbstractIoSession::newReadFuture() {
	sp<ConcurrentLinkedQueue<EReadFuture> > readyReadFutures = getReadyReadFutures();
	sp<ConcurrentLinkedQueue<EReadFuture> > waitingReadFutures = getWaitingReadFutures();
	sp<EReadFuture> future;
	SYNCHRONIZED(readyReadFutures) {
		future = waitingReadFutures->poll();
		if (future == null) {
			future = new EDefaultReadFuture(shared_from_this());
			readyReadFutures->offer(future);
		}
    }}
	return future;
}

sp<EAbstractIoSession::ConcurrentLinkedQueue<EReadFuture> > EAbstractIoSession::getReadyReadFutures() {
	sp<ConcurrentLinkedQueue<EReadFuture> > readyReadFutures =
			dynamic_pointer_cast<ConcurrentLinkedQueue<EReadFuture> >(
					getAttribute(READY_READ_FUTURES_KEY.get()));
	if (readyReadFutures == null) {
		readyReadFutures = new ConcurrentLinkedQueue<EReadFuture>();

		sp<ConcurrentLinkedQueue<EReadFuture> > oldReadyReadFutures =
				dynamic_pointer_cast<ConcurrentLinkedQueue<EReadFuture> >(
						setAttributeIfAbsent(READY_READ_FUTURES_KEY,
								readyReadFutures));
		if (oldReadyReadFutures != null) {
			readyReadFutures = oldReadyReadFutures;
		}
	}
	return readyReadFutures;
}

sp<EAbstractIoSession::ConcurrentLinkedQueue<EReadFuture> > EAbstractIoSession::getWaitingReadFutures() {
	sp<ConcurrentLinkedQueue<EReadFuture> > waitingReadyReadFutures =
			dynamic_pointer_cast<ConcurrentLinkedQueue<EReadFuture> >(
					getAttribute(WAITING_READ_FUTURES_KEY.get()));
	if (waitingReadyReadFutures == null) {
		waitingReadyReadFutures = new ConcurrentLinkedQueue<EReadFuture>();

		sp<ConcurrentLinkedQueue<EReadFuture> > oldWaitingReadyReadFutures =
				dynamic_pointer_cast<ConcurrentLinkedQueue<EReadFuture> >(
						setAttributeIfAbsent(WAITING_READ_FUTURES_KEY,
								waitingReadyReadFutures));
		if (oldWaitingReadyReadFutures != null) {
			waitingReadyReadFutures = oldWaitingReadyReadFutures;
		}
	}
	return waitingReadyReadFutures;
}

void EAbstractIoSession::decreaseScheduledWriteMessages() {
	scheduledWriteMessages.decrementAndGet();
	EIoService* service = getService();
	EAbstractIoService* ais = dynamic_cast<EAbstractIoService*>(service);
	if (ais) {
		ais->getStatistics()->decreaseScheduledWriteMessages();
	}
}

EString EAbstractIoSession::getIdAsString() {
	EString id = ELLong::toHexString(getId());
	id.toUpperCase();

	// Somewhat inefficient, but it won't happen that often
	// because an ID is often a big integer.

	while (id.length() < 8) {
		id.insert(0, "0", 1); // padding
	}
	id.insert(0, "0x", 2);

	return id;
}

EString EAbstractIoSession::getServiceName() {
	ETransportMetadata* tm = getTransportMetadata();
	if (tm == null) {
		return "null";
	}

	return EString::formatOf("%s %s", tm->getProviderName(), tm->getName());
}

void EAbstractIoSession::notifyIdleSession0(sp<EAbstractIoSession>& session, llong currentTime, llong idleTime, EIdleStatus status,
	            llong lastIoTime) {
	if ((idleTime > 0) && (lastIoTime != 0) && (currentTime - lastIoTime >= idleTime)) {
		session->getFilterChain()->fireSessionIdle(status);
	}
}

void EAbstractIoSession::notifyWriteTimeout(sp<EAbstractIoSession>& session, llong currentTime) {
	llong writeTimeout = session->getConfig()->getWriteTimeoutInMillis();
	if ((writeTimeout > 0) && (currentTime - session->getLastWriteTime() >= writeTimeout)
			&& !session->getWriteRequestQueue()->isEmpty(session)) {
		sp<EWriteRequest> request = session->getCurrentWriteRequest();
		if (request != null) {
			session->setCurrentWriteRequest(null);
			EWriteTimeoutException cause(__FILE__, __LINE__, request);
			request->getFuture()->setException(new EThrowableObject<EWriteTimeoutException>(cause));
			session->getFilterChain()->fireExceptionCaught(new EThrowableObject<EWriteTimeoutException>(cause));
			// WriteException is an IOException, so we close the session.
			session->closeNow();
		}
	}
}

} /* namespace eio */
} /* namespace efc */
