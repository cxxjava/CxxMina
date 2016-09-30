/*
 * EIoSessionOutputStream.cpp
 *
 *  Created on: 2016-6-1
 *      Author: cxxjava@163.com
 */

#include "EIoSessionOutputStream.hh"
#include "ECloseFuture.hh"

namespace efc {
namespace eio {

EIoSessionOutputStream::EIoSessionOutputStream(sp<EIoSession>& session) {
	this->session = session;
}

void EIoSessionOutputStream::write(const void* b, int len) {
	ES_ASSERT(b && len>=0);
	//@see: write(IoBuffer.wrap(b.clone(), off, len));
	sp<EIoBuffer> buf = EIoBuffer::wrap((void*)b, len, 0);
	this->write(buf->duplicate());
}

void EIoSessionOutputStream::write(int b) {
	EIoBuffer* buf = EIoBuffer::allocate(1);
	buf->put((byte) b);
	buf->flip();
	this->write(buf);
}

void EIoSessionOutputStream::flush() {
	SYNCHRONIZED(this) {
		if (lastWriteFuture == null) {
			return;
		}

		lastWriteFuture->awaitUninterruptibly();
		if (!lastWriteFuture->isWritten()) {
			throw EIOException(__FILE__, __LINE__, "The bytes could not be written to the session");
		}
    }}
}

void EIoSessionOutputStream::close() {
	ON_FINALLY_NOTHROW(
		session.lock()->closeNow()->awaitUninterruptibly();
	) {
		flush();
    }}
}

void EIoSessionOutputStream::write(sp<EIoBuffer> buf) {
	SYNCHRONIZED(this) {
		if (!session.lock()->isConnected()) {
			throw EIOException(__FILE__, __LINE__, "The session has been closed.");
		}
		lastWriteFuture = session.lock()->write(buf);
    }}
}

} /* namespace eio */
} /* namespace efc */
