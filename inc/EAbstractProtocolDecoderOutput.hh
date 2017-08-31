/*
 * EAbstractProtocolDecoderOutput.hh
 *
 *  Created on: 2016-5-21
 *      Author: cxxjava@163.com
 */

#ifndef EABSTRACTPROTOCOLDECODEROUTPUT_HH_
#define EABSTRACTPROTOCOLDECODEROUTPUT_HH_

#include "./EProtocolDecoderOutput.hh"

namespace efc {
namespace eio {

/**
 * A {@link ProtocolDecoderOutput} based on queue.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EAbstractProtocolDecoderOutput: virtual public EProtocolDecoderOutput {
public:
	EAbstractProtocolDecoderOutput() {
		// Do nothing
    }

	/* @see:
	public Queue<Object> getMessageQueue() {
        return messageQueue;
    }
    */
	sp<EObject> poll() {
		return messageQueue.poll();
	}

    virtual void write(sp<EObject> message) {
        if (message == null) {
            throw EIllegalArgumentException(__FILE__, __LINE__, "message");
        }

        messageQueue.add(message);
    }

private:
    ELinkedList<sp<EObject> > messageQueue;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EABSTRACTPROTOCOLDECODEROUTPUT_HH_ */
