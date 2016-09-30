/*
 * EIoEventType.hh
 *
 *  Created on: 2016-5-17
 *      Author: cxxjava@163.com
 */

#ifndef EIOEVENTTYPE_HH_
#define EIOEVENTTYPE_HH_

#include "Efc.hh"

namespace efc {
namespace eio {

/**
 * An {@link Enum} that represents the type of I/O events and requests.
 * Most users won't need to use this class.  It is usually used by internal
 * components to store I/O events.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

enum EIoEventType {
    SESSION_CREATED  = 0x0001,
    SESSION_OPENED   = 0x0002,
    SESSION_CLOSED   = 0x0004,
    MESSAGE_RECEIVED = 0x0008,
    MESSAGE_SENT     = 0x0010,
    SESSION_IDLE     = 0x0020,
    EXCEPTION_CAUGHT = 0x0040,
    WRITE            = 0x0080,
    CLOSE            = 0x0100
};

/**
 * Get event type name.
 */
const char* GetIoEventTypeName(int eventType);

} /* namespace eio */
} /* namespace efc */

#endif /* EIOEVENTTYPE_HH_ */
