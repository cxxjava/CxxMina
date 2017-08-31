/*
 * ECumulativeProtocolDecoder.cpp
 *
 *  Created on: 2016-5-23
 *      Author: cxxjava@163.com
 */

#include "../inc/ECumulativeProtocolDecoder.hh"

namespace efc {
namespace eio {

void ECumulativeProtocolDecoder::decode(sp<EIoSession>& session, EIoBuffer* in,
		sp<EProtocolDecoderOutput>& out) {
	if (transportMetadataFragmentation && !session->getTransportMetadata()->hasFragmentation()) {
		while (in->hasRemaining()) {
			if (!doDecode(session, in, out)) {
				break;
			}
		}

		return;
	}

	boolean usingSessionBuffer = true;
	sp<EIoBuffer> buf_ = dynamic_pointer_cast<EIoBuffer>(session->getAttribute(BUFFER.get()));
	EIoBuffer* buf = buf_.get();
	// If we have a session buffer, append data to that; otherwise
	// use the buffer read from the network directly.
	if (buf != null) {
		boolean appended = false;
		// Make sure that the buffer is auto-expanded.
		if (buf->isAutoExpand()) {
			try {
				buf->put(in);
				appended = true;
			} catch (EIllegalStateException& e) {
				// A user called derivation method (e.g. slice()),
				// which disables auto-expansion of the parent buffer.
			} catch (EIndexOutOfBoundsException& e) {
				// A user disabled auto-expansion.
			}
		}

		if (appended) {
			buf->flip();
		} else {
			// Reallocate the buffer if append operation failed due to
			// derivation or disabled auto-expansion.
			buf->flip();
			sp<EIoBuffer> newBuf(EIoBuffer::allocate(buf->remaining() + in->remaining())->setAutoExpand(true));
			//newBuf.order(buf.order());
			newBuf->put(buf);
			newBuf->put(in);
			newBuf->flip();
			buf = newBuf.get();

			// Update the session attribute.
			session->setAttribute(BUFFER, newBuf);
		}
	} else {
		buf = in;
		usingSessionBuffer = false;
	}

	for (;;) {
		int oldPos = buf->position();
		boolean decoded = doDecode(session, buf, out);
		if (decoded) {
			if (buf->position() == oldPos) {
				throw EIllegalStateException(__FILE__, __LINE__, "doDecode() can't return true when buffer is not consumed.");
			}

			if (!buf->hasRemaining()) {
				break;
			}
		} else {
			break;
		}
	}

	// if there is any data left that cannot be decoded, we store
	// it in a buffer in the session and next time this decoder is
	// invoked the session buffer gets appended to
	if (buf->hasRemaining()) {
		if (usingSessionBuffer && buf->isAutoExpand()) {
			buf->compact();
		} else {
			storeRemainingInSession(buf, session);
		}
	} else {
		if (usingSessionBuffer) {
			removeSessionBuffer(session);
		}
	}
}

void ECumulativeProtocolDecoder::dispose(sp<EIoSession>& session) {
	removeSessionBuffer(session);
}

ECumulativeProtocolDecoder::ECumulativeProtocolDecoder(): transportMetadataFragmentation(true) {
	BUFFER = new EAttributeKey(typeid(ECumulativeProtocolDecoder).name(), "buffer");
}

sp<EIoBuffer> ECumulativeProtocolDecoder::getRemainingInSession(sp<EIoSession>& session) {
	return dynamic_pointer_cast<EIoBuffer>(session->getAttribute(BUFFER.get()));
}

void ECumulativeProtocolDecoder::setTransportMetadataFragmentation(boolean tmf) {
	transportMetadataFragmentation = tmf;
}

void ECumulativeProtocolDecoder::removeSessionBuffer(sp<EIoSession>& session) {
	session->removeAttribute(BUFFER.get());
}

void ECumulativeProtocolDecoder::storeRemainingInSession(EIoBuffer* buf,
		sp<EIoSession>& session) {
	/* @see:
	 final IoBuffer remainingBuf = IoBuffer.allocate(buf.capacity()).setAutoExpand(true);
	 */
	int capacity = ES_MAX(buf->remaining(), buf->capacity() / 2);
	sp<EIoBuffer> remainingBuf(EIoBuffer::allocate(capacity)->setAutoExpand(true));

	/*
	remainingBuf->order(buf->order());
	*/
	remainingBuf->put(buf);

	session->setAttribute(BUFFER, remainingBuf);
}

} /* namespace eio */
} /* namespace efc */
