/*
 * EProtocolDecoderOutput.hh
 *
 *  Created on: 2016-5-20
 *      Author: cxxjava@163.com
 */

#ifndef EPROTOCOLDECODEROUTPUT_HH_
#define EPROTOCOLDECODEROUTPUT_HH_

#include "./EIoSession.hh"
#include "./EIoFilter.hh"

namespace efc {
namespace eio {

/**
 * Callback for {@link ProtocolDecoder} to generate decoded messages.
 * {@link ProtocolDecoder} must call {@link #write(Object)} for each decoded
 * messages.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

interface EProtocolDecoderOutput: virtual public EObject {
	virtual ~EProtocolDecoderOutput(){}

	/**
	 * Callback for {@link ProtocolDecoder} to generate decoded messages.
	 * {@link ProtocolDecoder} must call {@link #write(Object)} for each
	 * decoded messages.
	 *
	 * @param message the decoded message
	 */
	virtual void write(sp<EObject> message) = 0;

	/**
	 * Flushes all messages you wrote via {@link #write(Object)} to
	 * the next filter.
	 *
	 * @param nextFilter the next Filter
	 * @param session The current Session
	 */
	virtual void flush(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session) = 0;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EPROTOCOLDECODEROUTPUT_HH_ */
