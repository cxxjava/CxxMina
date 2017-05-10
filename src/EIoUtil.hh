/*
 * EIoUtil.hh
 *
 *  Created on: 2016-3-5
 *      Author: cxxjava@163.com
 */

#ifndef EIOUTIL_HH_
#define EIOUTIL_HH_

#include "EIoSession.hh"
#include "EWriteFuture.hh"

namespace efc {
namespace eio {

/**
 * A utility class that provides various convenience methods related with
 * {@link IoSession} and {@link IoFuture}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EIoUtil {
public:
	/**
	 * Writes the specified {@code message} to the specified {@code sessions}.
	 * If the specified {@code message} is an {@link IoBuffer}, the buffer is
	 * automatically duplicated using {@link IoBuffer#duplicate()}.
	 */
	static EArrayList<sp<EWriteFuture> > broadcast(sp<EObject>& message, sp<EConcurrentCollection<EIoSession> > sessions);

private:
	EIoUtil() {
		// Do nothing
	}
};

} /* namespace eio */
} /* namespace efc */
#endif /* EIOUTIL_HH_ */
