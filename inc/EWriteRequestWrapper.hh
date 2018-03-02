/*
 * EWriteRequestWrapper.hh
 *
 *  Created on: 2016-5-21
 *      Author: cxxjava@163.com
 */

#ifndef EWRITEREQUESTWRAPPER_HH_
#define EWRITEREQUESTWRAPPER_HH_

#include "./EWriteRequest.hh"

namespace efc {
namespace eio {

/**
 * A wrapper for an existing {@link WriteRequest}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EWriteRequestWrapper: public EWriteRequest {
public:
	virtual ~EWriteRequestWrapper();

	/**
	 * Creates a new instance that wraps the specified request.
	 *
	 * @param parentRequest The parent's request
	 */
	EWriteRequestWrapper(sp<EWriteRequest>& parentRequest);

	/**
	 * {@inheritDoc}
	 */
	virtual EInetSocketAddress* getDestination();

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EWriteFuture> getFuture();

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EObject> getMessage();

	/**
	 * {@inheritDoc}
	 */
	virtual EWriteRequest* getOriginalRequest();

	/**
	 * @return the wrapped request object.
	 */
	virtual sp<EWriteRequest> getParentRequest();

	/**
	 * {@inheritDoc}
	 */
	virtual EString toString();

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isEncoded();

private:
	sp<EWriteRequest> parentRequest;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EWRITEREQUESTWRAPPER_HH_ */
