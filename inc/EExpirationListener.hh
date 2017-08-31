/*
 * EExpirationListener.hh
 *
 *  Created on: 2016-8-29
 *      Author: cxxjava@163.com
 */

#ifndef EEXPIRATIONLISTENER_HH_
#define EEXPIRATIONLISTENER_HH_

#include "./EIoSession.hh"

namespace efc {
namespace eio {

/**
 * A listener for expired object events.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * TODO Make this a inner interface of ExpiringMap
 */

template<typename E>
interface EExpirationListener: virtual public EObject {
	virtual ~EExpirationListener(){}

	virtual void expired(sp<E> expiredSession) = 0;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EEXPIRATIONLISTENER_HH_ */
