/*
 * EIoBufferDecoder.cpp
 *
 *  Created on: 2016-6-8
 *      Author: cxxjava@163.com
 */

#include "EIoBufferDecoder.hh"

namespace efc {
namespace eio {

EIoBufferDecoder::EIoBufferDecoder(EA<byte>* delimiter) {
	ctx = new DecodingContext();
	setDelimiter(delimiter, true);
}

EIoBufferDecoder::EIoBufferDecoder(const char* delimiter) {
	ctx = new DecodingContext();
	setDelimiter(delimiter, true);
}

EIoBufferDecoder::EIoBufferDecoder(const void* delimiter, int length) {
	ctx = new DecodingContext();
	setDelimiter(delimiter, length, true);
}

EIoBufferDecoder::EIoBufferDecoder(int contentLength) {
	ctx = new DecodingContext();
	setContentLength(contentLength, false);
}

void EIoBufferDecoder::setContentLength(int contentLength,
		boolean resetMatchCount) {
	if (contentLength <= 0) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "contentLength <= 0");
	}

	ctx->setContentLength(contentLength);
	if (resetMatchCount) {
		ctx->setMatchCount(0);
	}
}

void EIoBufferDecoder::setDelimiter(EA<byte>* delim, boolean resetMatchCount) {
	if (delim == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "Null delimiter not allowed");
	}

	this->setDelimiter(delim->address(), delim->length(), resetMatchCount);
}

void EIoBufferDecoder::setDelimiter(const char* delim, boolean resetMatchCount) {
	if (delim == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "Null delimiter not allowed");
	}

	this->setDelimiter(delim, eso_strlen(delim), resetMatchCount);
}

void EIoBufferDecoder::setDelimiter(const void* delim, int length,
		boolean resetMatchCount) {
	if (delim == null || length <= 0) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "Null delimiter not allowed");
	}

	// Convert delimiter to IoBuffer.
	sp<EIoBuffer> delimiter = EIoBuffer::allocate(length);
	delimiter->put(delim, length);
	delimiter->flip();

	ctx->setDelimiter(delimiter);
	ctx->setContentLength(-1);
	if (resetMatchCount) {
		ctx->setMatchCount(0);
	}
}

sp<EIoBuffer> EIoBufferDecoder::decodeFully(sp<EIoBuffer>& in) {
	int contentLength = ctx->getContentLength();
	sp<EIoBuffer> decodedBuffer = ctx->getDecodedBuffer();

	int oldLimit = in->limit();

	// Retrieve fixed length content
	if (contentLength > -1) {
		if (decodedBuffer == null) {
			decodedBuffer = EIoBuffer::allocate(contentLength)->setAutoExpand(true);
		}

		// If not enough data to complete the decoding
		if (in->remaining() < contentLength) {
			int readBytes = in->remaining();
			decodedBuffer->put(in.get());
			ctx->setDecodedBuffer(decodedBuffer);
			ctx->setContentLength(contentLength - readBytes);
			return null;

		}

		int newLimit = in->position() + contentLength;
		in->limit(newLimit);
		decodedBuffer->put(in.get());
		decodedBuffer->flip();
		in->limit(oldLimit);
		ctx->reset();

		return decodedBuffer;
	}

	// Not a fixed length matching so try to find a delimiter match
	int oldPos = in->position();
	int matchCount = ctx->getMatchCount();
	sp<EIoBuffer> delimiter = ctx->getDelimiter();

	while (in->hasRemaining()) {
		byte b = in->get();
		if (delimiter->get(matchCount) == b) {
			matchCount++;
			if (matchCount == delimiter->limit()) {
				// Found a match.
				int pos = in->position();
				in->position(oldPos);

				in->limit(pos);

				if (decodedBuffer == null) {
					decodedBuffer = EIoBuffer::allocate(in->remaining())->setAutoExpand(true);
				}

				decodedBuffer->put(in.get());
				decodedBuffer->flip();

				in->limit(oldLimit);
				ctx->reset();

				return decodedBuffer;
			}
		} else {
			in->position(ES_MAX(0, in->position() - matchCount));
			matchCount = 0;
		}
	}

	// Copy remainder from buf.
	if (in->remaining() > 0) {
		in->position(oldPos);
		decodedBuffer->put(in.get());
		in->position(in->limit());
	}

	// Save decoding state
	ctx->setMatchCount(matchCount);
	ctx->setDecodedBuffer(decodedBuffer);

	return decodedBuffer;
}

} /* namespace eio */
} /* namespace efc */
