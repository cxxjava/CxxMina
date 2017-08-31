/*
 * EProfilerTimerFilter.cpp
 *
 *  Created on: 2016-5-17
 *      Author: cxxjava@163.com
 */

#include "../inc/EProfilerTimerFilter.hh"
#include "../inc/EIoFilterChain.hh"

namespace efc {
namespace eio {

EProfilerTimerFilter::~EProfilerTimerFilter() {
	delete messageReceivedTimerWorker;
	delete messageSentTimerWorker;
	delete sessionCreatedTimerWorker;
	delete sessionOpenedTimerWorker;
	delete sessionIdleTimerWorker;
	delete sessionClosedTimerWorker;
}

EProfilerTimerFilter::EProfilerTimerFilter():
		messageReceivedTimerWorker(null),
		profileMessageReceived(false),
		messageSentTimerWorker(null),
		profileMessageSent(false),
		sessionCreatedTimerWorker(null),
		profileSessionCreated(false),
		sessionOpenedTimerWorker(null),
		profileSessionOpened(false),
		sessionIdleTimerWorker(null),
		profileSessionIdle(false),
		sessionClosedTimerWorker(null),
		profileSessionClosed(false) {
	this->timeUnit = ETimeUnit::MILLISECONDS;

	setProfilers(EIoEventType::MESSAGE_RECEIVED | EIoEventType::MESSAGE_SENT);
}

EProfilerTimerFilter::EProfilerTimerFilter(ETimeUnit* timeUnit):
		messageReceivedTimerWorker(null),
		profileMessageReceived(false),
		messageSentTimerWorker(null),
		profileMessageSent(false),
		sessionCreatedTimerWorker(null),
		profileSessionCreated(false),
		sessionOpenedTimerWorker(null),
		profileSessionOpened(false),
		sessionIdleTimerWorker(null),
		profileSessionIdle(false),
		sessionClosedTimerWorker(null),
		profileSessionClosed(false) {
	this->timeUnit = timeUnit;

	setProfilers(EIoEventType::MESSAGE_RECEIVED | EIoEventType::MESSAGE_SENT);
}

EProfilerTimerFilter::EProfilerTimerFilter(ETimeUnit* timeUnit,
		int eventTypes):
		messageReceivedTimerWorker(null),
		profileMessageReceived(false),
		messageSentTimerWorker(null),
		profileMessageSent(false),
		sessionCreatedTimerWorker(null),
		profileSessionCreated(false),
		sessionOpenedTimerWorker(null),
		profileSessionOpened(false),
		sessionIdleTimerWorker(null),
		profileSessionIdle(false),
		sessionClosedTimerWorker(null),
		profileSessionClosed(false) {
	this->timeUnit = timeUnit;

	setProfilers(eventTypes);
}

void EProfilerTimerFilter::setTimeUnit(ETimeUnit* timeUnit) {
	this->timeUnit = timeUnit;
}

void EProfilerTimerFilter::profile(EIoEventType type) {
	switch (type) {
		case MESSAGE_RECEIVED:
			profileMessageReceived = true;

			if (messageReceivedTimerWorker == null) {
				messageReceivedTimerWorker = new TimerWorker();
			}

			return;

		case MESSAGE_SENT:
			profileMessageSent = true;

			if (messageSentTimerWorker == null) {
				messageSentTimerWorker = new TimerWorker();
			}

			return;

		case SESSION_CREATED:
			profileSessionCreated = true;

			if (sessionCreatedTimerWorker == null) {
				sessionCreatedTimerWorker = new TimerWorker();
			}

			return;

		case SESSION_OPENED:
			profileSessionOpened = true;

			if (sessionOpenedTimerWorker == null) {
				sessionOpenedTimerWorker = new TimerWorker();
			}

			return;

		case SESSION_IDLE:
			profileSessionIdle = true;

			if (sessionIdleTimerWorker == null) {
				sessionIdleTimerWorker = new TimerWorker();
			}

			return;

		case SESSION_CLOSED:
			profileSessionClosed = true;

			if (sessionClosedTimerWorker == null) {
				sessionClosedTimerWorker = new TimerWorker();
			}

			return;

		default:
				break;
	}
}

void EProfilerTimerFilter::stopProfile(EIoEventType type) {
	switch (type) {
		case MESSAGE_RECEIVED:
			profileMessageReceived = false;
			return;

		case MESSAGE_SENT:
			profileMessageSent = false;
			return;

		case SESSION_CREATED:
			profileSessionCreated = false;
			return;

		case SESSION_OPENED:
			profileSessionOpened = false;
			return;

		case SESSION_IDLE:
			profileSessionIdle = false;
			return;

		case SESSION_CLOSED:
			profileSessionClosed = false;
			return;

		default:
			return;
	}
}

int EProfilerTimerFilter::getEventsToProfile() {
	int set = 0x00;

	if (profileMessageReceived) {
		set |= EIoEventType::MESSAGE_RECEIVED;
	}

	if (profileMessageSent) {
		set |= EIoEventType::MESSAGE_SENT;
	}

	if (profileSessionCreated) {
		set |= EIoEventType::SESSION_CREATED;
	}

	if (profileSessionOpened) {
		set |= EIoEventType::SESSION_OPENED;
	}

	if (profileSessionIdle) {
		set |= EIoEventType::SESSION_IDLE;
	}

	if (profileSessionClosed) {
		set |= EIoEventType::SESSION_CLOSED;
	}

	return set;
}

void EProfilerTimerFilter::setEventsToProfile(int eventTypes) {
	setProfilers(eventTypes);
}

double EProfilerTimerFilter::getAverageTime(EIoEventType type) {
	switch (type) {
		case MESSAGE_RECEIVED:
			if (profileMessageReceived) {
				return messageReceivedTimerWorker->getAverage();
			}

			break;

		case MESSAGE_SENT:
			if (profileMessageSent) {
				return messageSentTimerWorker->getAverage();
			}

			break;

		case SESSION_CREATED:
			if (profileSessionCreated) {
				return sessionCreatedTimerWorker->getAverage();
			}

			break;

		case SESSION_OPENED:
			if (profileSessionOpened) {
				return sessionOpenedTimerWorker->getAverage();
			}

			break;

		case SESSION_IDLE:
			if (profileSessionIdle) {
				return sessionIdleTimerWorker->getAverage();
			}

			break;

		case SESSION_CLOSED:
			if (profileSessionClosed) {
				return sessionClosedTimerWorker->getAverage();
			}

			break;

		default:
			break;
	}

	throw EIllegalArgumentException(__FILE__, __LINE__, "You are not monitoring this event.  Please add this event first.");
}

llong EProfilerTimerFilter::getTotalCalls(EIoEventType type) {
	switch (type) {
		case MESSAGE_RECEIVED:
			if (profileMessageReceived) {
				return messageReceivedTimerWorker->getCallsNumber();
			}

			break;

		case MESSAGE_SENT:
			if (profileMessageSent) {
				return messageSentTimerWorker->getCallsNumber();
			}

			break;

		case SESSION_CREATED:
			if (profileSessionCreated) {
				return sessionCreatedTimerWorker->getCallsNumber();
			}

			break;

		case SESSION_OPENED:
			if (profileSessionOpened) {
				return sessionOpenedTimerWorker->getCallsNumber();
			}

			break;

		case SESSION_IDLE:
			if (profileSessionIdle) {
				return sessionIdleTimerWorker->getCallsNumber();
			}

			break;

		case SESSION_CLOSED:
			if (profileSessionClosed) {
				return sessionClosedTimerWorker->getCallsNumber();
			}

			break;

		default:
			break;
	}

	throw EIllegalArgumentException(__FILE__, __LINE__, "You are not monitoring this event.  Please add this event first.");
}

llong EProfilerTimerFilter::getTotalTime(EIoEventType type) {
	switch (type) {
		case MESSAGE_RECEIVED:
			if (profileMessageReceived) {
				return messageReceivedTimerWorker->getTotal();
			}

			break;

		case MESSAGE_SENT:
			if (profileMessageSent) {
				return messageSentTimerWorker->getTotal();
			}

			break;

		case SESSION_CREATED:
			if (profileSessionCreated) {
				return sessionCreatedTimerWorker->getTotal();
			}

			break;

		case SESSION_OPENED:
			if (profileSessionOpened) {
				return sessionOpenedTimerWorker->getTotal();
			}

			break;

		case SESSION_IDLE:
			if (profileSessionIdle) {
				return sessionIdleTimerWorker->getTotal();
			}

			break;

		case SESSION_CLOSED:
			if (profileSessionClosed) {
				return sessionClosedTimerWorker->getTotal();
			}

			break;

		default:
			break;
	}

	throw EIllegalArgumentException(__FILE__, __LINE__, "You are not monitoring this event.  Please add this event first.");
}

llong EProfilerTimerFilter::getMinimumTime(EIoEventType type) {
	switch (type) {
		case MESSAGE_RECEIVED:
			if (profileMessageReceived) {
				return messageReceivedTimerWorker->getMinimum();
			}

			break;

		case MESSAGE_SENT:
			if (profileMessageSent) {
				return messageSentTimerWorker->getMinimum();
			}

			break;

		case SESSION_CREATED:
			if (profileSessionCreated) {
				return sessionCreatedTimerWorker->getMinimum();
			}

			break;

		case SESSION_OPENED:
			if (profileSessionOpened) {
				return sessionOpenedTimerWorker->getMinimum();
			}

			break;

		case SESSION_IDLE:
			if (profileSessionIdle) {
				return sessionIdleTimerWorker->getMinimum();
			}

			break;

		case SESSION_CLOSED:
			if (profileSessionClosed) {
				return sessionClosedTimerWorker->getMinimum();
			}

			break;

		default:
			break;
	}

	throw EIllegalArgumentException(__FILE__, __LINE__, "You are not monitoring this event.  Please add this event first.");
}

llong EProfilerTimerFilter::getMaximumTime(EIoEventType type) {
	switch (type) {
		case MESSAGE_RECEIVED:
			if (profileMessageReceived) {
				return messageReceivedTimerWorker->getMaximum();
			}

			break;

		case MESSAGE_SENT:
			if (profileMessageSent) {
				return messageSentTimerWorker->getMaximum();
			}

			break;

		case SESSION_CREATED:
			if (profileSessionCreated) {
				return sessionCreatedTimerWorker->getMaximum();
			}

			break;

		case SESSION_OPENED:
			if (profileSessionOpened) {
				return sessionOpenedTimerWorker->getMaximum();
			}

			break;

		case SESSION_IDLE:
			if (profileSessionIdle) {
				return sessionIdleTimerWorker->getMaximum();
			}

			break;

		case SESSION_CLOSED:
			if (profileSessionClosed) {
				return sessionClosedTimerWorker->getMaximum();
			}

			break;

		default:
			break;
	}

	throw EIllegalArgumentException(__FILE__, __LINE__, "You are not monitoring this event.  Please add this event first.");
}

void EProfilerTimerFilter::messageReceived(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, sp<EObject>& message) {
	if (profileMessageReceived) {
		llong start = timeNow();
		nextFilter->messageReceived(session, message);
		llong end = timeNow();
		messageReceivedTimerWorker->addNewDuration(end - start);
	} else {
		nextFilter->messageReceived(session, message);
	}
}

void EProfilerTimerFilter::messageSent(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) {
	if (profileMessageSent) {
		llong start = timeNow();
		nextFilter->messageSent(session, writeRequest);
		llong end = timeNow();
		messageSentTimerWorker->addNewDuration(end - start);
	} else {
		nextFilter->messageSent(session, writeRequest);
	}
}

void EProfilerTimerFilter::sessionCreated(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session) {
	if (profileSessionCreated) {
		llong start = timeNow();
		nextFilter->sessionCreated(session);
		llong end = timeNow();
		sessionCreatedTimerWorker->addNewDuration(end - start);
	} else {
		nextFilter->sessionCreated(session);
	}
}

void EProfilerTimerFilter::sessionOpened(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session) {
	if (profileSessionOpened) {
		llong start = timeNow();
		nextFilter->sessionOpened(session);
		llong end = timeNow();
		sessionOpenedTimerWorker->addNewDuration(end - start);
	} else {
		nextFilter->sessionOpened(session);
	}
}

void EProfilerTimerFilter::sessionIdle(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, EIdleStatus status) {
	if (profileSessionIdle) {
		llong start = timeNow();
		nextFilter->sessionIdle(session, status);
		llong end = timeNow();
		sessionIdleTimerWorker->addNewDuration(end - start);
	} else {
		nextFilter->sessionIdle(session, status);
	}
}

void EProfilerTimerFilter::sessionClosed(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session) {
	if (profileSessionClosed) {
		llong start = timeNow();
		nextFilter->sessionClosed(session);
		llong end = timeNow();
		sessionClosedTimerWorker->addNewDuration(end - start);
	} else {
		nextFilter->sessionClosed(session);
	}
}

void EProfilerTimerFilter::setProfilers(int eventTypes) {
	if (eventTypes & EIoEventType::MESSAGE_RECEIVED) {
		messageReceivedTimerWorker = new TimerWorker();
		profileMessageReceived = true;
	}

	if (eventTypes & EIoEventType::MESSAGE_SENT) {
		messageSentTimerWorker = new TimerWorker();
		profileMessageSent = true;
	}

	if (eventTypes & EIoEventType::SESSION_CREATED) {
		sessionCreatedTimerWorker = new TimerWorker();
		profileSessionCreated = true;
	}

	if (eventTypes & EIoEventType::SESSION_OPENED) {
		sessionOpenedTimerWorker = new TimerWorker();
		profileSessionOpened = true;
	}

	if (eventTypes & EIoEventType::SESSION_IDLE) {
		sessionIdleTimerWorker = new TimerWorker();
		profileSessionIdle = true;
	}

	if (eventTypes & EIoEventType::SESSION_CLOSED) {
		sessionClosedTimerWorker = new TimerWorker();
		profileSessionClosed = true;
	}
}

llong EProfilerTimerFilter::timeNow() {
	if (timeUnit == ETimeUnit::SECONDS) {
		return ESystem::currentTimeMillis() / 1000;
	}
	else if (timeUnit == ETimeUnit::MICROSECONDS) {
		return ESystem::nanoTime() / 1000;
	}
	else if (timeUnit == ETimeUnit::NANOSECONDS) {
		return ESystem::nanoTime();
	}
	else {
		return ESystem::currentTimeMillis();
	}
}

} /* namespace eio */
} /* namespace efc */
