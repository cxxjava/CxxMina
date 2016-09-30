/*
 * EProtocolCodecFilter.cpp
 *
 *  Created on: 2016-5-20
 *      Author: cxxjava@163.com
 */

#include "EProtocolCodecFilter.hh"
#include "EFileRegion.hh"
#include "EProtocolEncoderException.hh"
#include "EProtocolDecoderException.hh"
#include "ERecoverableProtocolDecoderException.hh"

namespace efc {
namespace eio {

sp<ELogger> EProtocolCodecFilter::LOGGER = ELoggerManager::getLogger("EProtocolCodecFilter");
sp<EAttributeKey> EProtocolCodecFilter::ENCODER = new EAttributeKey(typeid(EProtocolCodecFilter).name(), "encoder");
sp<EAttributeKey> EProtocolCodecFilter::DECODER = new EAttributeKey(typeid(EProtocolCodecFilter).name(), "decoder");
sp<EAttributeKey> EProtocolCodecFilter::DECODER_OUT = new EAttributeKey(typeid(EProtocolCodecFilter).name(), "decoderOut");
sp<EAttributeKey> EProtocolCodecFilter::ENCODER_OUT = new EAttributeKey(typeid(EProtocolCodecFilter).name(), "encoderOut");
static const char* emptyBuffer = "";
EIoBuffer* EProtocolCodecFilter::EMPTY_BUFFER = EIoBuffer::wrap(&emptyBuffer, 0, 0); //!

EProtocolCodecFilter::~EProtocolCodecFilter() {
	if (factoryOwner && factory) {
		delete factory;
	}
}

EProtocolCodecFilter::EProtocolCodecFilter(EProtocolCodecFactory* factory) {
	if (factory == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "factory");
	}

	this->factory = factory;
	this->factoryOwner = false;
}

EProtocolCodecFilter::EProtocolCodecFilter(EProtocolEncoder* encoder,
		EProtocolDecoder* decoder) {
	if (encoder == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "encoder");
	}
	if (decoder == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "decoder");
	}

	class ProtocolCodecFactory: public EProtocolCodecFactory {
	private:
		EProtocolEncoder* encoder;
		EProtocolDecoder* decoder;
	public:
		ProtocolCodecFactory(EProtocolEncoder* e, EProtocolDecoder* d) {
			encoder = e;
			decoder = d;
		}
		virtual EProtocolEncoder* getEncoder(sp<EIoSession>& session) {
			return encoder;
		}

		virtual EProtocolDecoder* getDecoder(sp<EIoSession>& session) {
			return decoder;
		}
	};
	// Create the inner Factory based on the two parameters
	this->factory = new ProtocolCodecFactory(encoder, decoder);
}

//EProtocolEncoder* EProtocolCodecFilter::getEncoder(sp<EIoSession>& session) {
//	return (EProtocolEncoder*) session->getAttribute(ENCODER);
//}

void EProtocolCodecFilter::onPreAdd(EIoFilterChain* parent, const char* name,
		EIoFilter::NextFilter* nextFilter) {
	if (parent->contains(this)) {
		throw EIllegalArgumentException(__FILE__, __LINE__,
			"You can't add the same filter instance more than once.  Create another instance and add it.");
	}
}

void EProtocolCodecFilter::onPostRemove(EIoFilterChain* parent,
		const char* name, EIoFilter::NextFilter* nextFilter) {
	// Clean everything
	sp<EIoSession> session = parent->getSession();
	disposeCodec(session);
}

void EProtocolCodecFilter::messageReceived(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, sp<EObject>& message) {
	LOGGER->debug__(__FILE__, __LINE__, "Processing a MESSAGE_RECEIVED for session %ld", session->getId());

	EIoBuffer* in = dynamic_cast<EIoBuffer*>(message.get());
	if (!in) {
		nextFilter->messageReceived(session, message);
		return;
	}

	EProtocolDecoder* decoder = factory->getDecoder(session);
	sp<EProtocolDecoderOutput> decoderOut = getDecoderOut(session, nextFilter);

	// Loop until we don't have anymore byte in the buffer,
	// or until the decoder throws an unrecoverable exception or
	// can't decoder a message, because there are not enough
	// data in the buffer
	while (in->hasRemaining()) {
		int oldPos = in->position();
		try {
//			ELock* lock = session->getLock();
//			ES_ASSERT(lock);
//			SYNCBLOCK(lock) { // here need lock ? cxxjava@163.com
				// Call the decoder with the read bytes
				decoder->decode(session, in, decoderOut);
//			}}
			// Finish decoding if no exception was thrown.
			decoderOut->flush(nextFilter, session);
		} catch (EException& e) {
			EProtocolDecoderException pde(e);
			if (pde.getHexdump() == null) {
				// Generate a message hex dump
				int curPos = in->position();
				in->position(oldPos);
				pde.setHexdump(in->getHexdump().c_str());
				in->position(curPos);
			}
			// Fire the exceptionCaught event.
			decoderOut->flush(nextFilter, session);
			sp<EThrowableType> tt(new EThrowableObject<EProtocolDecoderException>(pde));
			nextFilter->exceptionCaught(session, tt);
			// Retry only if the type of the caught exception is
			// recoverable and the buffer position has changed.
			// We check buffer position additionally to prevent an
			// infinite loop.
			//@see: if (!(e instanceof RecoverableProtocolDecoderException) || (in.position() == oldPos)) {
			if (instanceof<ERecoverableProtocolDecoderException>(e) || (in->position() == oldPos)) {
				break;
			}
		}
	}
}

void EProtocolCodecFilter::messageSent(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) {
	if (instanceof<EncodedWriteRequest>(writeRequest.get())) {
		return;
	}

	sp<MessageWriteRequest> wrappedRequest = dynamic_pointer_cast<MessageWriteRequest>(writeRequest);
	if (wrappedRequest != null) {
		sp<EWriteRequest> wr = wrappedRequest->getParentRequest();
		nextFilter->messageSent(session, wr);
	} else {
		nextFilter->messageSent(session, writeRequest);
	}
}

void EProtocolCodecFilter::filterWrite(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) {
	sp<EObject> message = writeRequest->getMessage();

	// Bypass the encoding if the message is contained in a IoBuffer,
	// as it has already been encoded before
	if (instanceof<EIoBuffer>(message.get()) || instanceof<EFileRegion>(message.get())) {
		nextFilter->filterWrite(session, writeRequest);
		return;
	}

	// Get the encoder in the session
	EProtocolEncoder* encoder = factory->getEncoder(session);

	sp<EProtocolEncoderOutput> encoderOut = getEncoderOut(session, nextFilter, writeRequest);

	if (encoder == null) {
		throw EProtocolEncoderException(__FILE__, __LINE__,
				EString::formatOf("The encoder is null for the session %s", session->toString().c_str()).c_str());
	}

	try {
		// Now we can try to encode the response
		encoder->encode(session, message, encoderOut);

		/* @see:
		// Send it directly
		Queue<Object> bufferQueue = ((AbstractProtocolEncoderOutput) encoderOut).getMessageQueue();

		// Write all the encoded messages now
		while (!bufferQueue.isEmpty()) {
			Object encodedMessage = bufferQueue.poll();

			if (encodedMessage == null) {
				break;
			}

			// Flush only when the buffer has remaining.
			if (!(encodedMessage instanceof IoBuffer) || ((IoBuffer) encodedMessage).hasRemaining()) {
				SocketAddress destination = writeRequest.getDestination();
				WriteRequest encodedWriteRequest = new EncodedWriteRequest(encodedMessage, null, destination);

				nextFilter.filterWrite(session, encodedWriteRequest);
			}
		}
		*/
		// Write all the encoded messages now
		EAbstractProtocolEncoderOutput* peo = dynamic_cast<EAbstractProtocolEncoderOutput*>(encoderOut.get());
		sp<EObject>  encodedMessage;
		while ((encodedMessage = peo->poll()) != null) {
			// Flush only when the buffer has remaining.
			sp<EIoBuffer> buf = dynamic_pointer_cast<EIoBuffer>(encodedMessage);
			if (buf == null || buf->hasRemaining()) {
				EInetSocketAddress* destination = writeRequest->getDestination();
				sp<EWriteRequest> encodedWriteRequest = new EncodedWriteRequest(encodedMessage, null, destination);

				nextFilter->filterWrite(session, encodedWriteRequest);
			}
		}

		// Call the next filter
		sp<EWriteRequest> wr = new MessageWriteRequest(writeRequest);
		nextFilter->filterWrite(session, wr);
	} catch (EException& e) {
		throw EProtocolEncoderException(e.getSourceFile(), e.getSourceLine(), e.getMessage(), e.getErrno());
	}
}

void EProtocolCodecFilter::sessionClosed(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session) {
	// Call finishDecode() first when a connection is closed.
	EProtocolDecoder* decoder = factory->getDecoder(session);
	sp<EProtocolDecoderOutput> decoderOut = getDecoderOut(session, nextFilter);

	ON_FINALLY_NOTHROW(
		// Dispose everything
		disposeCodec(session);
		decoderOut->flush(nextFilter, session);
	) {
		try {
			decoder->finishDecode(session, decoderOut);
		} catch (EException& e) {
			EProtocolDecoderException pde(e);
			throw pde;
		}
    }}

	// Call the next filter
	nextFilter->sessionClosed(session);
}

void EProtocolCodecFilter::disposeCodec(sp<EIoSession>& session) {
	// We just remove the two instances of encoder/decoder to release resources
	// from the session
	disposeEncoder(session);
	disposeDecoder(session);

	// We also remove the callback
	disposeDecoderOut(session);
}

void EProtocolCodecFilter::disposeEncoder(sp<EIoSession>& session) {
	sp<EProtocolEncoder> encoder = dynamic_pointer_cast<EProtocolEncoder>(session->removeAttribute(ENCODER.get())); //sp?
	if (encoder == null) {
		return;
	}

	try {
		encoder->dispose(session);
	} catch (EException& e) {
		LOGGER->warn__(__FILE__, __LINE__,
				EString::formatOf("Failed to dispose: %s (%s)", typeid(encoder).name(), encoder->toString().c_str()).c_str());
	}
}

void EProtocolCodecFilter::disposeDecoder(sp<EIoSession>& session) {
	sp<EProtocolDecoder> decoder = dynamic_pointer_cast<EProtocolDecoder>(session->removeAttribute(DECODER.get())); //sp?
	if (decoder == null) {
		return;
	}

	try {
		decoder->dispose(session);
	} catch (EException& e) {
		LOGGER->warn__(__FILE__, __LINE__,
				EString::formatOf("Failed to dispose: %s (%s)", typeid(decoder).name(), decoder->toString().c_str()).c_str());
	}
}

sp<EProtocolDecoderOutput> EProtocolCodecFilter::getDecoderOut(sp<EIoSession>& session, EIoFilter::NextFilter* nextFilter) {
	sp<EProtocolDecoderOutput> out = dynamic_pointer_cast<EProtocolDecoderOutput>(session->getAttribute(DECODER_OUT.get()));

	if (out == null) {
		// Create a new instance, and stores it into the session
		out = new ProtocolDecoderOutputImpl();
		session->setAttribute(DECODER_OUT, out);
	}

	return out;
}

sp<EProtocolEncoderOutput> EProtocolCodecFilter::getEncoderOut(sp<EIoSession>& session, EIoFilter::NextFilter* nextFilter, sp<EWriteRequest>& writeRequest) {
	sp<EProtocolEncoderOutput> out = dynamic_pointer_cast<EProtocolEncoderOutput>(session->getAttribute(ENCODER_OUT.get()));

	if (out == null) {
		// Create a new instance, and stores it into the session
		out = new ProtocolEncoderOutputImpl(session, nextFilter, writeRequest);
		session->setAttribute(ENCODER_OUT, out);
	}

	return out;
}

void EProtocolCodecFilter::disposeDecoderOut(sp<EIoSession>& session) {
	session->removeAttribute(DECODER_OUT.get());
}

} /* namespace eio */
} /* namespace efc */
