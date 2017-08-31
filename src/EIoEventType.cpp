/*
 * EIoEventType.cpp
 *
 *  Created on: 2016-9-23
 *      Author: cxxjava@163.com
 */

#include "../inc/EIoEventType.hh"

namespace efc {
namespace eio {

const char* GetIoEventTypeName(int eventType) {
	switch (eventType) {
	case SESSION_CREATED:
		return "SESSION_CREATED";
	case SESSION_OPENED:
		return "SESSION_OPENED";
	case SESSION_CLOSED:
		return "SESSION_CLOSED";
	case MESSAGE_RECEIVED:
		return "MESSAGE_RECEIVED";
	case MESSAGE_SENT:
		return "MESSAGE_SENT";
	case SESSION_IDLE:
		return "SESSION_IDLE";
	case EXCEPTION_CAUGHT:
		return "EXCEPTION_CAUGHT";
	case WRITE:
		return "WRITE";
	case CLOSE:
		return "CLOSE";
	}
	return "?";
}

} /* namespace eio */
} /* namespace efc */
