/*
 * EDefaultWriteRequest.hh
 *
 *  Created on: 2016-1-6
 *      Author: cxxjava@163.com
 */

#ifndef EDEFAULTWRITEREQUEST_HH_
#define EDEFAULTWRITEREQUEST_HH_

#include "./EWriteRequest.hh"

namespace efc {
namespace eio {

/**
 * The default implementation of {@link WriteRequest}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EDefaultWriteRequest: public EWriteRequest {
public:
	virtual ~EDefaultWriteRequest();

	/**
	 * Creates a new instance without {@link WriteFuture}.  You'll get
	 * an instance of {@link WriteFuture} even if you called this constructor
	 * because {@link #getFuture()} will return a bogus future.
	 *
	 * @param message The message that will be written
	 */
	EDefaultWriteRequest(sp<EObject> message);

	/**
	 * Creates a new instance with {@link WriteFuture}.
	 *
	 * @param message The message that will be written
     * @param future The associated {@link WriteFuture}
	 */
	EDefaultWriteRequest(sp<EObject> message, sp<EWriteFuture> future);

	/**
	 * Creates a new instance.
	 *
	 * @param message a message to write
	 * @param future a future that needs to be notified when an operation is finished
	 * @param destination the destination of the message.  This property will be
	 *                    ignored unless the transport supports it.
	 */
	EDefaultWriteRequest(sp<EObject> message, sp<EWriteFuture> future, EInetSocketAddress* destination);

	virtual sp<EWriteFuture> getFuture();

	virtual sp<EObject> getMessage();

	virtual EWriteRequest* getOriginalRequest();

	virtual EInetSocketAddress* getDestination();

	virtual boolean isEncoded();

	virtual EString toString();

private:
	sp<EObject> message;

	sp<EWriteFuture> future;

	EInetSocketAddress* destination;

	void init(sp<EObject>& message, sp<EWriteFuture> future, EInetSocketAddress* destination);

	/** An empty FUTURE */
	static sp<EWriteFuture> UNUSED_FUTURE;// = new WriteFuture(){...};
};

} /* namespace eio */
} /* namespace efc */
#endif /* EDEFAULTWRITEREQUEST_HH_ */
