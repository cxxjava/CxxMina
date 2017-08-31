/*
 * ETextLineDecoder.cpp
 *
 *  Created on: 2016-5-24
 *      Author: cxxjava@163.com
 */

#include "../inc/ETextLineDecoder.hh"

namespace efc {
namespace eio {

ETextLineDecoder::~ETextLineDecoder() {
	eso_mfree(crlf);
}

ETextLineDecoder::ETextLineDecoder(const char* s): crlf(null) {
	crlf = eso_mstrdup(s ? s : "\r\n");
}

boolean ETextLineDecoder::doDecode(sp<EIoSession>& session, EIoBuffer* in,
		sp<EProtocolDecoderOutput>& out) {
	// Remember the initial position.
	int start = in->position();

	int crlfLen = eso_strlen(crlf);

	if (in->hasRemaining()) {
		// Now find the first CRLF in the buffer.
		char* startAddr = (char*)in->current();
		char* foundCRLF = eso_strnstr(startAddr, in->remaining(), crlf);
		if (foundCRLF) {
			// Remember the current position and limit.
			int position = start + (foundCRLF + crlfLen - startAddr);
			int limit = in->limit();

			ON_FINALLY_NOTHROW(
					// Set the position to point right after the
					// detected line and set the limit to the old
					// one.
					in->position(position);
					in->limit(limit);
			) {
				in->position(start);
				in->limit(position);
				// The bytes between in.position() and in.limit()
				// now contain a full CRLF terminated line.
				EIoBuffer* slice = EIoBuffer::allocate(in->remaining() + 1 /*'\0'*/);
				slice->put(in);
				slice->put((byte)'\0'); //!
				slice->flip();
				out->write(slice);
            }}

			// Decoded one line; CumulativeProtocolDecoder will
			// call me again until I return false. So just
			// return true until there are no more lines in the
			// buffer.
			return true;
		}
	}

	// Could not find CRLF in the buffer. Reset the initial
	// position to the one we recorded above.
	in->position(start);

	return false;
}

void ETextLineDecoder::finishDecode(sp<EIoSession>& session, sp<EProtocolDecoderOutput>& out) {
	sp<EIoBuffer> in = getRemainingInSession(session);
	if (in != null && (in->flip(), in->hasRemaining())) {
		EIoBuffer* slice = EIoBuffer::allocate(in->remaining() + 1 /*'\0'*/);
		slice->put(in.get());
		slice->put((byte)'\0'); //!
		slice->flip();
		out->write(slice);
	}
}

} /* namespace eio */
} /* namespace efc */
