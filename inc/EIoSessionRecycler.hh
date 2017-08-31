/*
 * EIoSessionRecycler.hh
 *
 *  Created on: 2016-8-29
 *      Author: cxxjava@163.com
 */

#ifndef EIOSESSIONRECYCLER_HH_
#define EIOSESSIONRECYCLER_HH_

#include "./EIoSession.hh"

namespace efc {
namespace eio {

/**
 * A connectionless transport can recycle existing sessions by assigning an
 * {@link IoSessionRecycler} to an {@link IoService}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * TODO More documentation
 */

interface EIoSessionRecycler: virtual public ESynchronizeable {
	virtual ~EIoSessionRecycler() {
	}

	/**
	 * Called when the underlying transport creates or writes a new {@link IoSession}.
	 *
	 * @param session the new {@link IoSession}.
	 */
	virtual void put(sp<EIoSession>& session) = 0;

	/**
	 * Attempts to retrieve a recycled {@link IoSession}.
	 *
	 * @param remoteAddress the remote socket address of the {@link IoSession} the transport wants to recycle.
	 * @return a recycled {@link IoSession}, or null if one cannot be found.
	 */
	virtual sp<EIoSession> recycle(EInetSocketAddress* remoteAddress) = 0;

	/**
	 * Called when an {@link IoSession} is explicitly closed.
	 *
	 * @param session the new {@link IoSession}.
	 */
	virtual void remove(sp<EIoSession>& session) = 0;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EIOSESSIONRECYCLER_HH_ */
