/*
 * EIoBufferDecoder.hh
 *
 *  Created on: 2016-6-8
 *      Author: cxxjava@163.com
 */

#ifndef EIOBUFFERDECODER_HH_
#define EIOBUFFERDECODER_HH_

#include "Efc.hh"
#include "./EIoBuffer.hh"

namespace efc {
namespace eio {

/**
 * IoBufferDecoder.java - Handles an {@link IoBuffer} decoder which supports
 * two methods :
 * - dynamic delimiter decoding
 * - fixed length content reading
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since MINA 2.0.0-M3
 */

class EIoBufferDecoder: public EObject {
public:
	/**
	 * The class holding the decoding context.
	 */
	class DecodingContext: public EObject {
	private:
		/**
		 * The buffered data.
		 */
		sp<EIoBuffer> decodedBuffer;

		/**
		 * The delimiter in use. Set if delimiter mode is in use.
		 */
		sp<EIoBuffer> delimiter;

		/**
		 * The currently matched bytes of the delimiter.
		 */
		int matchCount;// = 0;

		/**
		 * Holds the current content length of decoded data if in
		 * content-length mode.
		 */
		int contentLength;// = -1;

	public:
		DecodingContext(): matchCount(0), contentLength(-1) {
		}

		/**
		 * Resets the decoding state.
		 */
		void reset() {
			contentLength = -1;
			matchCount = 0;
		}

		int getContentLength() {
			return contentLength;
		}

		void setContentLength(int contentLength) {
			this->contentLength = contentLength;
		}

		int getMatchCount() {
			return matchCount;
		}

		void setMatchCount(int matchCount) {
			this->matchCount = matchCount;
		}

		sp<EIoBuffer> getDecodedBuffer() {
			return decodedBuffer;
		}

		void setDecodedBuffer(sp<EIoBuffer>& decodedBuffer) {
			this->decodedBuffer = decodedBuffer;
		}

		sp<EIoBuffer> getDelimiter() {
			return delimiter;
		}

		void setDelimiter(sp<EIoBuffer>& delimiter) {
			this->delimiter = delimiter;
		}
	};

public:
	/**
	 * Creates a new instance that uses specified <tt>delimiter</tt> byte array as a
	 * message delimiter.
	 *
	 * @param delimiter an array of characters which delimits messages
	 */
	EIoBufferDecoder(EA<byte>* delimiter);
	EIoBufferDecoder(const char* delimiter);
	EIoBufferDecoder(const void* delimiter, int length);

	/**
	 * Creates a new instance that will read messages of <tt>contentLength</tt> bytes.
	 *
	 * @param contentLength the exact length to read
	 */
	EIoBufferDecoder(int contentLength);

	/**
	 * Sets the the length of the content line to be decoded.
	 * When set, it overrides the dynamic delimiter setting and content length
	 * method will be used for decoding on the next decodeOnce call.
	 * The default value is <tt>-1</tt>.
	 *
	 * @param contentLength the content length to match
	 * @param resetMatchCount delimiter matching is reset if true
	 */
	void setContentLength(int contentLength, boolean resetMatchCount);

	/**
	 * Dynamically sets a new delimiter. Next time
	 * {@link #decodeFully(IoBuffer)} will be called it will use the new
	 * delimiter. Delimiter matching is reset only if <tt>resetMatchCount</tt> is true but
	 * decoding will continue from current position.
	 *
	 * NB : Delimiter {@link LineDelimiter#AUTO} is not allowed.
	 *
	 * @param delim the new delimiter as a byte array
	 * @param resetMatchCount delimiter matching is reset if true
	 */
	void setDelimiter(EA<byte>* delim, boolean resetMatchCount);
	void setDelimiter(const char* delimiter, boolean resetMatchCount);
	void setDelimiter(const void* delimiter, int length, boolean resetMatchCount);

	/**
	 * Will return null unless it has enough data to decode. If <code>contentLength</code>
	 * is set then it tries to retrieve <code>contentLength</code> bytes from the buffer
	 * otherwise it will scan the buffer to find the data <code>delimiter</code> and return
	 * all the data and the trailing delimiter.
	 *
	 * @param in the data to decode
	 * @return The decoded buffer
	 */
	sp<EIoBuffer> decodeFully(sp<EIoBuffer>& in);

private:
	/**
	 * The decoding context.
	 */
	sp<DecodingContext> ctx;// = new DecodingContext();
};

} /* namespace eio */
} /* namespace efc */
#endif /* EIOBUFFERDECODER_HH_ */
