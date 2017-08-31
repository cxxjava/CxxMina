/*
 * EAbstractProtocolEncoderOutput.hh
 *
 *  Created on: 2016-5-21
 *      Author: cxxjava@163.com
 */

#ifndef EABSTRACTPROTOCOLENCODEROUTPUT_HH_
#define EABSTRACTPROTOCOLENCODEROUTPUT_HH_

#include "./EProtocolEncoderOutput.hh"

namespace efc {
namespace eio {

/**
 * A {@link ProtocolEncoderOutput} based on queue.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EAbstractProtocolEncoderOutput: virtual public EProtocolEncoderOutput {
public:
	EAbstractProtocolEncoderOutput() {
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

    virtual void write(sp<EObject> encodedMessage) {
    	sp<EIoBuffer> buf = dynamic_pointer_cast<EIoBuffer>(encodedMessage);
        if (buf != null) {
            if (buf->hasRemaining()) {
                messageQueue.offer(buf);
            } else {
                throw EIllegalArgumentException(__FILE__, __LINE__, "buf is empty. Forgot to call flip()?");
            }
        } else {
            messageQueue.offer(encodedMessage);
        }
    }

private:
    EConcurrentLiteQueue<EObject> messageQueue;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EABSTRACTPROTOCOLENCODEROUTPUT_HH_ */
