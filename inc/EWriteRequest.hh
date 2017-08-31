/*
 * EWriteRequest.hh
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#ifndef EWRITEREQUEST_HH_
#define EWRITEREQUEST_HH_

#include "./EIoSession.hh"

namespace efc {
namespace eio {
    
class EWriteFuture;

/**
 * Represents write request fired by {@link IoSession#write(Object)}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */
interface EWriteRequest : virtual public EObject {
	virtual ~EWriteRequest() {
	}

	/**
	 * @return the {@link WriteRequest} which was requested originally,
	 * which is not transformed by any {@link IoFilter}.
	 */
	virtual EWriteRequest* getOriginalRequest() = 0;

	/**
	 * @return {@link WriteFuture} that is associated with this write request.
	 */
	virtual sp<EWriteFuture> getFuture() = 0;

	/**
	 * @return a message object to be written.
	 */
	virtual sp<EObject> getMessage() = 0;

	/**
	 * Returns the destination of this write request.
	 *
	 * @return <tt>null</tt> for the default destination
	 */
	virtual EInetSocketAddress* getDestination() = 0;

	/**
	 * Tells if the current message has been encoded
	 *
	 * @return true if the message has already been encoded
	 */
	virtual boolean isEncoded() = 0;
};

} /* namespace eio */
} /* namespace efc */

#endif /* EWRITEREQUEST_HH_ */
