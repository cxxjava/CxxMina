/*
 * EProtocolCodecFilter.hh
 *
 *  Created on: 2016-5-20
 *      Author: cxxjava@163.com
 */

#ifndef EPROTOCOLCODECFILTER_HH_
#define EPROTOCOLCODECFILTER_HH_

#include "./EIoFilterAdapter.hh"
#include "./EProtocolEncoder.hh"
#include "./EProtocolDecoder.hh"
#include "./EProtocolCodecFactory.hh"
#include "./EProtocolDecoderOutput.hh"
#include "./EProtocolEncoderOutput.hh"
#include "./EAttributeKey.hh"
#include "./EIoFilterChain.hh"
#include "./EDefaultWriteRequest.hh"
#include "./EWriteRequestWrapper.hh"
#include "./EAbstractIoSession.hh"
#include "./EAbstractProtocolDecoderOutput.hh"
#include "./EAbstractProtocolEncoderOutput.hh"
#include "./EDefaultWriteFuture.hh"
#include "./ENothingWrittenException.hh"
#include "ELog.hh"

namespace efc {
namespace eio {

class EProtocolCodecFilter: public EIoFilterAdapter {
public:
	virtual ~EProtocolCodecFilter();

	/**
	 * Creates a new instance of ProtocolCodecFilter, associating a factory
	 * for the creation of the encoder and decoder.
	 *
	 * @param factory The associated factory
	 */
	EProtocolCodecFilter(EProtocolCodecFactory* factory);

	/**
	 * Creates a new instance of ProtocolCodecFilter, without any factory.
	 * The encoder/decoder factory will be created as an inner class, using
	 * the two parameters (encoder and decoder).
	 *
	 * @param encoder The class responsible for encoding the message
	 * @param decoder The class responsible for decoding the message
	 */
	EProtocolCodecFilter(EProtocolEncoder* encoder, EProtocolDecoder* decoder);

	/**
	 * Get the encoder instance from a given session.
	 *
	 * @param session The associated session we will get the encoder from
	 * @return The encoder instance, if any
	 */
//	EProtocolEncoder* getEncoder(sp<EIoSession>& session);

	/**
	 * {@inheritDoc}
	 */
	virtual void onPreAdd(EIoFilterChain* parent, const char* name, EIoFilter::NextFilter* nextFilter) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void onPostRemove(EIoFilterChain* parent, const char* name, EIoFilter::NextFilter* nextFilter) THROWS(EException);

	/**
	 * Process the incoming message, calling the session decoder. As the incoming
	 * buffer might contains more than one messages, we have to loop until the decoder
	 * throws an exception.
	 *
	 *  while ( buffer not empty )
	 *    try
	 *      decode ( buffer )
	 *    catch
	 *      break;
	 *
	 */
	virtual void messageReceived(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, sp<EObject>& message) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void messageSent(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void filterWrite(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void sessionClosed(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session) THROWS(EException);

protected:

private:
	class EncodedWriteRequest: public EDefaultWriteRequest {
	public:
		EncodedWriteRequest(sp<EObject> encodedMessage, sp<EWriteFuture> future, EInetSocketAddress* destination) :
			EDefaultWriteRequest(encodedMessage, future, destination) {
		}

		virtual boolean isEncoded() {
			return true;
		}
	};

	class MessageWriteRequest: public EWriteRequestWrapper {
	public:
		MessageWriteRequest(sp<EWriteRequest>& writeRequest) :
			EWriteRequestWrapper(writeRequest) {
		}

		virtual sp<EObject> getMessage() {
			return EMPTY_BUFFER;
		}

		virtual EString toString() {
			return EString("MessageWriteRequest, parent : ") + EWriteRequestWrapper::toString();
		}
	};

	class ProtocolDecoderOutputImpl: public EAbstractProtocolDecoderOutput {
	public:
		ProtocolDecoderOutputImpl() {
			// Do nothing
		}

		virtual void flush(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session) {
			/* @see:
			Queue<Object> messageQueue = getMessageQueue();

			while (!messageQueue.isEmpty()) {
				nextFilter.messageReceived(session, messageQueue.poll());
			}
			*/
			sp<EObject> message;
			while ((message = poll()) != null) {
				nextFilter->messageReceived(session, message);
			}
		}
	};

	class ProtocolEncoderOutputImpl: public EAbstractProtocolEncoderOutput {
	private:
		sp<EIoSession> session;

		EIoFilter::NextFilter* nextFilter;

		/** The WriteRequest destination */
		EInetSocketAddress destination;

	public:
		ProtocolEncoderOutputImpl(sp<EIoSession> session, EIoFilter::NextFilter* nextFilter, sp<EWriteRequest>& writeRequest) :
			destination(*(writeRequest->getDestination())) {
			this->session = session;
			this->nextFilter = nextFilter;

			// Only store the destination, not the full WriteRequest.
			//destination = writeRequest.getDestination();
		}

		virtual sp<EWriteFuture> flush() {
			/* @see:
			Queue<Object> bufferQueue = getMessageQueue();
			WriteFuture future = null;

			while (!bufferQueue.isEmpty()) {
				Object encodedMessage = bufferQueue.poll();

				if (encodedMessage == null) {
					break;
				}

				// Flush only when the buffer has remaining.
				if (!(encodedMessage instanceof IoBuffer) || ((IoBuffer) encodedMessage).hasRemaining()) {
					future = new DefaultWriteFuture(session);
					nextFilter.filterWrite(session, new EncodedWriteRequest(encodedMessage, future, destination));
				}
			}
			*/
			sp<EWriteFuture> future;
			sp<EObject> encodedMessage;
			while ((encodedMessage = poll()) != null) {
				// Flush only when the buffer has remaining.
				sp<EIoBuffer> buf = dynamic_pointer_cast<EIoBuffer>(encodedMessage);
				if (buf == null || buf->hasRemaining()) {
					future = new EDefaultWriteFuture(session);
					sp<EWriteRequest> wr = new EncodedWriteRequest(encodedMessage, future, &destination);
					nextFilter->filterWrite(session, wr);
				}
			}

			if (future == null) {
				// Creates an empty writeRequest containing the destination
				//@see: future = DefaultWriteFuture.newNotWrittenFuture(session, new NothingWrittenException(AbstractIoSession.MESSAGE_SENT_REQUEST));
				{
					sp<EWriteRequest> writeRequest = new EDefaultWriteRequest(
							EAbstractIoSession::MESSAGE_SENT_REQUEST, null, &destination);
					ENothingWrittenException e(__FILE__, __LINE__, writeRequest);
					future = EDefaultWriteFuture::newNotWrittenFuture(session, new EThrowableObject<ENothingWrittenException>(e));
				}
			}

			return future;
		}
	};

private:
	static sp<ELogger> LOGGER;// = LoggerFactory.getLogger(ProtocolCodecFilter.class);

	static sp<EAttributeKey> ENCODER;// = new AttributeKey(ProtocolCodecFilter.class, "encoder");

    static sp<EAttributeKey> DECODER;// = new AttributeKey(ProtocolCodecFilter.class, "decoder");

    static sp<EAttributeKey> DECODER_OUT;// = new AttributeKey(ProtocolCodecFilter.class, "decoderOut");

    static sp<EAttributeKey> ENCODER_OUT;// = new AttributeKey(ProtocolCodecFilter.class, "encoderOut");

    static EIoBuffer* EMPTY_BUFFER;// = IoBuffer.wrap(new byte[0]);

    /** The factory responsible for creating the encoder and decoder */
    EProtocolCodecFactory* factory;
    boolean factoryOwner;

    //----------- Helper methods ---------------------------------------------
	/**
	 * Dispose the encoder, decoder, and the callback for the decoded
	 * messages.
	 */
	void disposeCodec(sp<EIoSession>& session);

	/**
	 * Dispose the encoder, removing its instance from the
	 * session's attributes, and calling the associated
	 * dispose method.
	 */
	void disposeEncoder(sp<EIoSession>& session);

	/**
	 * Dispose the decoder, removing its instance from the
	 * session's attributes, and calling the associated
	 * dispose method.
	 */
	void disposeDecoder(sp<EIoSession>& session);

	/**
	 * Return a reference to the decoder callback. If it's not already created
	 * and stored into the session, we create a new instance.
	 */
	sp<EProtocolDecoderOutput> getDecoderOut(sp<EIoSession>& session, EIoFilter::NextFilter* nextFilter);
	sp<EProtocolEncoderOutput> getEncoderOut(sp<EIoSession>& session, EIoFilter::NextFilter* nextFilter, sp<EWriteRequest>& writeRequest);

	/**
	 * Remove the decoder callback from the session's attributes.
	 */
	void disposeDecoderOut(sp<EIoSession>& session);
};

} /* namespace eio */
} /* namespace efc */
#endif /* EPROTOCOLCODECFILTER_HH_ */
