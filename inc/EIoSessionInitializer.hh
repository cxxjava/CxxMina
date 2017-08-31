/*
 * EIoSessionInitializer.hh
 *
 *  Created on: 2013-8-23
 *      Author: cxxjava@163.com
 */

#ifndef EIOSESSIONINITIALIZER_HH_
#define EIOSESSIONINITIALIZER_HH_

#include "./EIoSession.hh"
#include "./EIoFuture.hh"

namespace efc {
namespace eio {

/**
 * Defines a callback for obtaining the {@link IoSession} during
 * session initialization.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */
interface EIoSessionInitializer {
	virtual ~EIoSessionInitializer() {
	}

	virtual void initializeSession(sp<EIoSession> session, sp<EIoFuture> future) = 0;
};

} /* namespace eio */
} /* namespace efc */

#endif /* EIOSESSIONINITIALIZER_HH_ */
