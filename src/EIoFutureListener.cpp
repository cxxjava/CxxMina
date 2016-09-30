/*
 * EIoFutureListener.cpp
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#include "EIoFutureListener.hh"

namespace efc {
namespace eio {

EIoFutureListener* EIoFutureListener::CLOSE;

DEFINE_STATIC_INITZZ_BEGIN(EIoFutureListener)
ESystem::_initzz_();
CLOSE = new CloseListener();
printf("EIoFutureListener::initzz()\n");
DEFINE_STATIC_INITZZ_END

} /* namespace eio */
} /* namespace efc */
