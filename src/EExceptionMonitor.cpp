/*
 * EExceptionMonitor.cpp
 *
 *  Created on: 2015-12-28
 *      Author: cxxjava@163.com
 */

#include "../inc/EExceptionMonitor.hh"
#include "../inc/EDefaultExceptionMonitor.hh"

namespace efc {
namespace eio {

EExceptionMonitor* EExceptionMonitor::instance = null;

DEFINE_STATIC_INITZZ_BEGIN(EExceptionMonitor)
ESystem::_initzz_();
instance = new EDefaultExceptionMonitor();
DEFINE_STATIC_INITZZ_END

EExceptionMonitor::~EExceptionMonitor() {
}

EExceptionMonitor* EExceptionMonitor::getInstance() {
	return instance;
}

void EExceptionMonitor::setInstance(EExceptionMonitor* monitor) {
	if (monitor == null) {
		monitor = new EDefaultExceptionMonitor();
	}

	delete instance;
	instance = monitor;
}

EExceptionMonitor::EExceptionMonitor() {
}

} /* namespace eio */
} /* namespace efc */
