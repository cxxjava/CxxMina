/*
 * EIoSessionEventType.hh
 *
 *  Created on: 2016-6-16
 *      Author: cxxjava@lotuseed.com
 */

#ifndef EIOSESSIONEVENTTYPE_HH_
#define EIOSESSIONEVENTTYPE_HH_

#include "Efc.hh"

namespace efc {
namespace eio {

/**
 * IoSessionEventType.java - Enumerates session event types.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since MINA 2.0.0-M3
 */

class EIoSessionEventType {
public:
	enum TYPE {
		CREATED = 1,
		OPENED = 2,
		IDLE = 3,
		CLOSED = 4
	};

public:
	static EString toString(int id) {
		switch (id) {
			case CREATED:
				return "- CREATED event -";
			case OPENED:
				return "- OPENED event -";
			case IDLE:
				return "- IDLE event -";
			case CLOSED:
				return "- CLOSED event -";
			default: {
				EString s("- Event Id=");
				s << id << " -";
				return s;
			}
		}
	}

private:
	EIoSessionEventType(); //private
};

} /* namespace eio */
} /* namespace efc */
#endif /* EIOSESSIONEVENTTYPE_HH_ */
