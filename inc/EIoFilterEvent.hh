/*
 * EIoFilterEvent.hh
 *
 *  Created on: 2016-9-23
 *      Author: cxxjava@163.com
 */

#ifndef EIOFILTEREVENT_HH_
#define EIOFILTEREVENT_HH_

#include "./EIoEvent.hh"
#include "./EIoFilter.hh"
#include "ELog.hh"

namespace efc {
namespace eio {

/**
 * An I/O event or an I/O request that MINA provides for {@link IoFilter}s.
 * Most users won't need to use this class.  It is usually used by internal
 * components to store I/O events.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EIoFilterEvent: public EIoEvent {
public:
	virtual ~EIoFilterEvent();

	EIoFilterEvent(EIoFilter::NextFilter* nextFilter, EIoEventType type, sp<EIoSession>& session, sp<EObject> parameter);

	EIoFilter::NextFilter* getNextFilter();

	virtual void fire();

private:
	EIoFilter::NextFilter* nextFilter;

	/** A logger for this class */
	static sp<ELogger> LOGGER;// = LoggerFactory.getLogger(IoFilterEvent.class);
};

} /* namespace eio */
} /* namespace efc */
#endif /* EIOFILTEREVENT_HH_ */
