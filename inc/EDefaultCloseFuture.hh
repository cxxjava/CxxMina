/*
 * EDefaultCloseFuture.hh
 *
 *  Created on: 2016-1-21
 *      Author: cxxjava@163.com
 */

#ifndef EDEFAULTCLOSEFUTURE_HH_
#define EDEFAULTCLOSEFUTURE_HH_

#include "./EDefaultIoFuture.hh"
#include "./ECloseFuture.hh"

namespace efc {
namespace eio {

/**
 * A default implementation of {@link CloseFuture}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EDefaultCloseFuture: public EDefaultIoFuture,
		public virtual ECloseFuture {
public:
	virtual ~EDefaultCloseFuture();

	/**
	 * Creates a new instance.
	 *
	 * @param session The associated session
	 */
	EDefaultCloseFuture(sp<EIoSession> session);

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isClosed();

	/**
	 * {@inheritDoc}
	 */
	virtual void setClosed();
};

} /* namespace eio */
} /* namespace efc */
#endif /* EDEFAULTCLOSEFUTURE_HH_ */
