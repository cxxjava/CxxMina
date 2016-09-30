/*
 * EDefaultExceptionMonitor.cpp
 *
 *  Created on: 2015-12-28
 *      Author: cxxjava@163.com
 */

#include "EDefaultExceptionMonitor.hh"

namespace efc {
namespace eio {

sp<ELogger> EDefaultExceptionMonitor::LOGGER = ELoggerManager::getLogger("DefaultExceptionMonitor");

void EDefaultExceptionMonitor::exceptionCaught(EThrowable& cause) {
	/*
	if (cause instanceof Error) {
		throw (Error) cause;
	}
	*/
	LOGGER->warn("Unexpected exception.", cause);
}

} /* namespace eio */
}

 /* namespace efc */
