/*
 * EIoSessionEvent.hh
 *
 *  Created on: 2016-6-16
 *      Author: cxxjava@lotuseed.com
 */

#ifndef EIOSESSIONEVENT_HH_
#define EIOSESSIONEVENT_HH_

#include "./EIoFilter.hh"
#include "./EIoSession.hh"
#include "./EIdleStatus.hh"
#include "./EIoSessionEventType.hh"
#include "ELog.hh"

namespace efc {
namespace eio {

class EIoSessionEvent: public EObject {
public:
	/**
	 * Creates an instance of this class when event type differs from
	 * {@link IoSessionEventType#IDLE}.
	 *
	 * @param nextFilter the next filter
	 * @param session the session
	 * @param type the event type
	 */
	EIoSessionEvent(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, EIoSessionEventType::TYPE type);

	/**
	 * Creates an instance of this class when event type is
	 * {@link IoSessionEventType#IDLE}.
	 *
	 * @param nextFilter the next filter
	 * @param session the session
	 * @param status the idle status
	 */
	EIoSessionEvent(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, EIdleStatus status);

	/**
	 * Delivers this event to the next filter.
	 */
	void deliverEvent();

	/**
	 * {@inheritDoc}
	 */
	virtual EString toString();

	/**
	 * @return the idle status of the event
	 */
	EIdleStatus getStatus();

	/**
	 * @return the next filter to which the event should be sent.
	 */
	EIoFilter::NextFilter* getNextFilter();

	/**
	 * @return the session on which the event occured.
	 */
	sp<EIoSession> getSession();

	/**
	 * @return the event type that occured.
	 */
	EIoSessionEventType::TYPE getType();

private:
	static sp<ELogger> logger;// = LoggerFactory.getLogger(IoSessionEvent.class);

    /**
     * The next filter in the chain.
     */
	EIoFilter::NextFilter* nextFilter;

    /**
     * The session.
     */
    sp<EIoSession> session;

    /**
     * The event type.
     */
    EIoSessionEventType::TYPE type; //EIoSessionEventType

    /**
     * The idle status if type value is {@link IoSessionEventType#IDLE},
     * null otherwise.
     */
    EIdleStatus status;

    /**
	 * Static method which effectively delivers the specified event to the next filter
	 * <code>nextFilter</code> on the <code>session</code>.
	 *
	 * @param nextFilter the next filter
	 * @param session the session on which the event occured
	 * @param type the event type
	 * @param status the idle status should only be non null only if the event type is
	 * {@link IoSessionEventType#IDLE}
	 */
	static void deliverEvent(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session,
			EIoSessionEventType::TYPE type, EIdleStatus status);
};

} /* namespace eio */
} /* namespace efc */
#endif /* EIOSESSIONEVENT_HH_ */
