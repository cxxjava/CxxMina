/*
 * EDefaultExceptionMonitor.hh
 *
 *  Created on: 2015-12-28
 *      Author: cxxjava@163.com
 */

#ifndef EDEFAULTEXCEPTIONMONITOR_HH_
#define EDEFAULTEXCEPTIONMONITOR_HH_

#include "./EExceptionMonitor.hh"
#include "ELog.hh"

namespace efc {
namespace eio {

/**
 * A default {@link ExceptionMonitor} implementation that logs uncaught
 * exceptions using {@link Logger}.
 * <p>
 * All {@link IoService}s have this implementation as a default exception
 * monitor.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EDefaultExceptionMonitor: public EExceptionMonitor {
public:
	void exceptionCaught(EThrowable& cause);

private:
	static sp<ELogger> LOGGER;// = LoggerFactory.getLogger(DefaultExceptionMonitor.class);
};

} /* namespace eio */
} /* namespace efc */
#endif /* EDEFAULTEXCEPTIONMONITOR_HH_ */
