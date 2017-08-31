/*
 * ETextLineDecoder.hh
 *
 *  Created on: 2016-5-24
 *      Author: cxxjava@163.com
 */

#ifndef ETEXTLINEDECODER_HH_
#define ETEXTLINEDECODER_HH_

#include "./ECumulativeProtocolDecoder.hh"
#include "./EAttributeKey.hh"

namespace efc {
namespace eio {

/**
 * A {@link ProtocolDecoder} which decodes a text line into a string.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class ETextLineDecoder: public ECumulativeProtocolDecoder {
public:
	virtual ~ETextLineDecoder();
	ETextLineDecoder(const char* crlf);

	/**
	 * {@inheritDoc}
	 */
	virtual boolean doDecode(sp<EIoSession>& session, EIoBuffer* in, sp<EProtocolDecoderOutput>& out) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void finishDecode(sp<EIoSession>& session, sp<EProtocolDecoderOutput>& out) THROWS(EException);

private:
	es_string_t* crlf;
};

} /* namespace eio */
} /* namespace efc */
#endif /* ETEXTLINEDECODER_HH_ */
