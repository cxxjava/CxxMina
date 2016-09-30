/*
 * EIoSessionInputStream.cpp
 *
 *  Created on: 2016-6-1
 *      Author: cxxjava@163.com
 */

#include "EIoSessionInputStream.hh"

namespace efc {
namespace eio {

EIoSessionInputStream::~EIoSessionInputStream() {
	delete buf;
	delete exception;
}

EIoSessionInputStream::EIoSessionInputStream(): closed(false), released(false), exception(null) {
	buf = EIoBuffer::allocate(16);
	buf->setAutoExpand(true);
	buf->limit(0);
}

void EIoSessionInputStream::write(EIoBuffer* src) {
	ES_ASSERT(src);
	SYNCHRONIZED(this) {
		if (closed) {
			return;
		}

		if (buf->hasRemaining()) {
			this->buf->compact();
			this->buf->put(src);
			this->buf->flip();
		} else {
			this->buf->clear();
			this->buf->put(src);
			this->buf->flip();
			this->notifyAll();
		}
    }}
}

void EIoSessionInputStream::throwException(EIOException& e) {
	SYNCHRONIZED(this) {
		if (exception == null) {
			exception = new EIOException(e);

			this->notifyAll();
		}
    }}
}

int EIoSessionInputStream::read(void* b, int len) {
	SYNCHRONIZED(this) {
		if (!waitForData()) {
			return -1;
		}

		int readBytes;

		if (len > buf->remaining()) {
			readBytes = buf->remaining();
		} else {
			readBytes = len;
		}

		buf->get(b, len, readBytes);

		return readBytes;
    }}
}

int EIoSessionInputStream::read() {
	SYNCHRONIZED(this) {
		if (!waitForData()) {
			return -1;
		}

		return buf->get() & 0xff;
    }}
}

long EIoSessionInputStream::available() {
	if (released) {
		return 0;
	}

	SYNCHRONIZED(this) {
		return buf->remaining();
    }}
}

void EIoSessionInputStream::close() {
	if (closed) {
		return;
	}

	SYNCHRONIZED(this) {
		closed = true;
		releaseBuffer();

		this->notifyAll();
    }}
}

boolean EIoSessionInputStream::waitForData() {
	if (released) {
		return false;
	}

	SYNCHRONIZED(this) {
		while (!released && buf->remaining() == 0 && exception == null) {
			try {
				this->wait();
			} catch (EInterruptedException& e) {
				EIOException ioe(__FILE__, __LINE__, "Interrupted while waiting for more data");
				throw ioe;
			}
		}
    }}

	if (exception != null) {
		releaseBuffer();
		throw (*exception);
	}

	if (closed && buf->remaining() == 0) {
		releaseBuffer();

		return false;
	}

	return true;
}

void EIoSessionInputStream::releaseBuffer() {
	if (released) {
		return;
	}

	released = true;
}

} /* namespace eio */
} /* namespace efc */
