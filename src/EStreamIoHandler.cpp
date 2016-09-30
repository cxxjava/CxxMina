/*
 * EStreamIoHandler.cpp
 *
 *  Created on: 2016-6-1
 *      Author: cxxjava@163.com
 */

#include "EStreamIoHandler.hh"
#include "EIoSessionInputStream.hh"
#include "EIoSessionOutputStream.hh"

namespace efc {
namespace eio {

sp<ELogger> EStreamIoHandler::LOGGER = ELoggerManager::getLogger("EStreamIoHandler");

sp<EAttributeKey> EStreamIoHandler::KEY_IN = new EAttributeKey(typeid(EStreamIoHandler).name(), "in");
sp<EAttributeKey> EStreamIoHandler::KEY_OUT = new EAttributeKey(typeid(EStreamIoHandler).name(), "out");

EStreamIoHandler::~EStreamIoHandler() {
	//
}

EStreamIoHandler::EStreamIoHandler(): readTimeout(0), writeTimeout(0) {
	// Do nothing
}

int EStreamIoHandler::getReadTimeout() {
	return readTimeout;
}

void EStreamIoHandler::setReadTimeout(int readTimeout) {
	this->readTimeout = readTimeout;
}

int EStreamIoHandler::getWriteTimeout() {
	return writeTimeout;
}

void EStreamIoHandler::setWriteTimeout(int writeTimeout) {
	this->writeTimeout = writeTimeout;
}

void EStreamIoHandler::sessionOpened(sp<EIoSession>& session) {
	// Set timeouts
	session->getConfig()->setWriteTimeout(writeTimeout);
	session->getConfig()->setIdleTime(EIdleStatus::READER_IDLE, readTimeout);

	// Create streams
	sp<EInputStream> in = new EIoSessionInputStream();
	sp<EOutputStream> out = new EIoSessionOutputStream(session);
	session->setAttribute(KEY_IN, in);
	session->setAttribute(KEY_OUT, out);
	processStreamIo(session, in, out);
}

void EStreamIoHandler::sessionClosed(sp<EIoSession>& session) {
	sp<EInputStream> in = dynamic_pointer_cast<EInputStream>(session->getAttribute(KEY_IN.get()));
	sp<EOutputStream> out = dynamic_pointer_cast<EOutputStream>(session->getAttribute(KEY_OUT.get()));
	try {
		in->close();
	} catch (...) {
		out->close();
		throw; //!
	}
	out->close();
}

void EStreamIoHandler::sessionIdle(sp<EIoSession>& session,
		EIdleStatus status) {
	if (status == EIdleStatus::READER_IDLE) {
		//@see: throw new StreamIoException(new SocketTimeoutException("Read timeout"));
		throw StreamIoException(__FILE__, __LINE__, "Read timeout");
	}
}

void EStreamIoHandler::messageReceived(sp<EIoSession>& session,
		sp<EObject>& message) {
	sp<EIoSessionInputStream> in = dynamic_pointer_cast<EIoSessionInputStream>(session->getAttribute(KEY_IN.get()));
	in->write(dynamic_cast<EIoBuffer*>(message.get()));
}

void EStreamIoHandler::exceptionCaught(sp<EIoSession>& session, sp<EThrowableType>& cause) {
	sp<EIoSessionInputStream> in = dynamic_pointer_cast<EIoSessionInputStream>(session->getAttribute(KEY_IN.get()));

	/* @see
	IOException e = null;
	if (cause instanceof StreamIoException) {
		e = (IOException) cause.getCause();
	} else if (cause instanceof IOException) {
		e = (IOException) cause;
	}

	if (e != null && in != null) {
		in.throwException(e);
	} else {
		LOGGER.warn("Unexpected exception.", cause);
		session.close(true);
	}
	*/
	EThrowable* e = cause->getThrowable();
	if (in != null && instanceof<StreamIoException>(e)) {
		ESocketTimeoutException ste(e->getSourceFile(), e->getSourceLine(), e->getMessage());
		in->throwException(ste);
	} else if (in != null && instanceof<EIOException>(e)) {
		EIOException ioe(e->getSourceFile(), e->getSourceLine(), e->getMessage());
		in->throwException(ioe);
	} else {
		LOGGER->warn__(__FILE__, __LINE__, "Unexpected exception: %s", (e ? e->toString().c_str() : "?"));
		session->closeNow();
	}
}

} /* namespace eio */
} /* namespace efc */
