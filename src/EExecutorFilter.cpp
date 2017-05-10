/*
 * EExecutorFilter.cpp
 *
 *  Created on: 2016-9-18
 *      Author: cxxjava@163.com
 */

#include "EExecutorFilter.hh"
#include "EIoFilterChain.hh"
#include "EOrderedThreadPoolExecutor.hh"

namespace efc {
namespace eio {

EExecutorFilter::~EExecutorFilter() throw() {
	if (manageableExecutor && executor) {
		executor->shutdownNow();
		executor->awaitTermination();
		delete executor;
	}
}

EExecutorFilter::EExecutorFilter() {
	// Create a new default Executor
	EExecutorService* executor = createDefaultExecutor(BASE_THREAD_NUMBER, DEFAULT_MAX_POOL_SIZE, DEFAULT_KEEPALIVE_TIME,
			ETimeUnit::SECONDS, EExecutors::defaultThreadFactory(), null);

	// Initialize the filter
	init(executor, MANAGEABLE_EXECUTOR, DEFAULT_EVENT_SET);
}

EExecutorFilter::EExecutorFilter(int maximumPoolSize) {
	// Create a new default Executor
	EExecutorService* executor = createDefaultExecutor(BASE_THREAD_NUMBER, maximumPoolSize, DEFAULT_KEEPALIVE_TIME,
			ETimeUnit::SECONDS, EExecutors::defaultThreadFactory(), null);

	// Initialize the filter
	init(executor, MANAGEABLE_EXECUTOR, DEFAULT_EVENT_SET);
}

EExecutorFilter::EExecutorFilter(int corePoolSize, int maximumPoolSize) {
	// Create a new default Executor
	EExecutorService* executor = createDefaultExecutor(corePoolSize, maximumPoolSize, DEFAULT_KEEPALIVE_TIME,
			ETimeUnit::SECONDS, EExecutors::defaultThreadFactory(), null);

	// Initialize the filter
	init(executor, MANAGEABLE_EXECUTOR, DEFAULT_EVENT_SET);
}

EExecutorFilter::EExecutorFilter(int corePoolSize, int maximumPoolSize,
		long keepAliveTime, ETimeUnit* unit) {
	// Create a new default Executor
	EExecutorService* executor = createDefaultExecutor(corePoolSize, maximumPoolSize, keepAliveTime,
			ETimeUnit::SECONDS, EExecutors::defaultThreadFactory(), null);

	// Initialize the filter
	init(executor, MANAGEABLE_EXECUTOR, DEFAULT_EVENT_SET);
}

EExecutorFilter::EExecutorFilter(int corePoolSize, int maximumPoolSize,
		long keepAliveTime, ETimeUnit* unit,
		EIoEventQueueHandler* queueHandler) {
	// Create a new default Executor
	EExecutorService* executor = createDefaultExecutor(corePoolSize, maximumPoolSize, keepAliveTime,
			unit, EExecutors::defaultThreadFactory(), queueHandler);

	// Initialize the filter
	init(executor, MANAGEABLE_EXECUTOR, DEFAULT_EVENT_SET);
}

EExecutorFilter::EExecutorFilter(int corePoolSize, int maximumPoolSize,
		long keepAliveTime, ETimeUnit* unit, sp<EThreadFactory> threadFactory) {
	// Create a new default Executor
	EExecutorService* executor = createDefaultExecutor(corePoolSize, maximumPoolSize, keepAliveTime,
			unit, threadFactory, null);

	// Initialize the filter
	init(executor, MANAGEABLE_EXECUTOR, DEFAULT_EVENT_SET);
}

EExecutorFilter::EExecutorFilter(int corePoolSize, int maximumPoolSize,
		long keepAliveTime, ETimeUnit* unit, sp<EThreadFactory> threadFactory,
		EIoEventQueueHandler* queueHandler) {
	// Create a new default Executor
	EExecutorService* executor = createDefaultExecutor(corePoolSize, maximumPoolSize, keepAliveTime,
			unit, threadFactory, queueHandler);

	// Initialize the filter
	init(executor, MANAGEABLE_EXECUTOR, DEFAULT_EVENT_SET);
}

EExecutorFilter::EExecutorFilter(long IoEventTypeSet) {
	// Create a new default Executor
	EExecutorService* executor = createDefaultExecutor(BASE_THREAD_NUMBER, DEFAULT_MAX_POOL_SIZE, DEFAULT_KEEPALIVE_TIME,
			ETimeUnit::SECONDS, EExecutors::defaultThreadFactory(), null);

	// Initialize the filter
	init(executor, MANAGEABLE_EXECUTOR, IoEventTypeSet);
}

EExecutorFilter::EExecutorFilter(int maximumPoolSize, long IoEventTypeSet) {
	// Create a new default Executor
	EExecutorService* executor = createDefaultExecutor(BASE_THREAD_NUMBER, maximumPoolSize, DEFAULT_KEEPALIVE_TIME,
			ETimeUnit::SECONDS, EExecutors::defaultThreadFactory(), null);

	// Initialize the filter
	init(executor, MANAGEABLE_EXECUTOR, IoEventTypeSet);
}

EExecutorFilter::EExecutorFilter(int corePoolSize, int maximumPoolSize,
		long IoEventTypeSet) {
	// Create a new default Executor
	EExecutorService* executor = createDefaultExecutor(corePoolSize, maximumPoolSize, DEFAULT_KEEPALIVE_TIME,
			ETimeUnit::SECONDS, EExecutors::defaultThreadFactory(), null);

	// Initialize the filter
	init(executor, MANAGEABLE_EXECUTOR, IoEventTypeSet);
}

EExecutorFilter::EExecutorFilter(int corePoolSize, int maximumPoolSize,
		long keepAliveTime, ETimeUnit* unit, long IoEventTypeSet) {
	// Create a new default Executor
	EExecutorService* executor = createDefaultExecutor(corePoolSize, maximumPoolSize, keepAliveTime,
			unit, EExecutors::defaultThreadFactory(), null);

	// Initialize the filter
	init(executor, MANAGEABLE_EXECUTOR, IoEventTypeSet);
}

EExecutorFilter::EExecutorFilter(int corePoolSize, int maximumPoolSize,
		long keepAliveTime, ETimeUnit* unit, EIoEventQueueHandler* queueHandler,
		long IoEventTypeSet) {
	// Create a new default Executor
	EExecutorService* executor = createDefaultExecutor(corePoolSize, maximumPoolSize, keepAliveTime,
			unit, EExecutors::defaultThreadFactory(), queueHandler);

	// Initialize the filter
	init(executor, MANAGEABLE_EXECUTOR, IoEventTypeSet);
}

EExecutorFilter::EExecutorFilter(int corePoolSize, int maximumPoolSize,
		long keepAliveTime, ETimeUnit* unit, sp<EThreadFactory> threadFactory,
		long IoEventTypeSet) {
	// Create a new default Executor
	EExecutorService* executor = createDefaultExecutor(corePoolSize, maximumPoolSize, keepAliveTime,
			unit, threadFactory, null);

	// Initialize the filter
	init(executor, MANAGEABLE_EXECUTOR, IoEventTypeSet);
}

EExecutorFilter::EExecutorFilter(int corePoolSize, int maximumPoolSize,
		long keepAliveTime, ETimeUnit* unit, sp<EThreadFactory> threadFactory,
		EIoEventQueueHandler* queueHandler, long IoEventTypeSet) {
	// Create a new default Executor
	EExecutorService* executor = createDefaultExecutor(corePoolSize, maximumPoolSize, keepAliveTime,
			unit, threadFactory, queueHandler);

	// Initialize the filter
	init(executor, MANAGEABLE_EXECUTOR, IoEventTypeSet);
}

EExecutorFilter::EExecutorFilter(EExecutorService* executor) {
	// Initialize the filter
	init(executor, NOT_MANAGEABLE_EXECUTOR, DEFAULT_EVENT_SET);
}

EExecutorFilter::EExecutorFilter(EExecutorService* executor,
		long IoEventTypeSet) {
	// Initialize the filter
	init(executor, NOT_MANAGEABLE_EXECUTOR, IoEventTypeSet);
}

void EExecutorFilter::init(EExecutorService* executor, boolean manageableExecutor, long IoEventTypeSet) {
	if (executor == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "executor");
	}

	this->eventTypeSet = IoEventTypeSet;
	this->executor = executor;
	this->manageableExecutor = manageableExecutor;
}

EExecutorService* EExecutorFilter::createDefaultExecutor(int corePoolSize, int maximumPoolSize, long keepAliveTime, ETimeUnit* unit,
		sp<EThreadFactory> threadFactory, EIoEventQueueHandler* queueHandler) {
	// Create a new Executor
	return new EOrderedThreadPoolExecutor(corePoolSize, maximumPoolSize, keepAliveTime, unit,
				threadFactory, queueHandler);
}

EExecutor* EExecutorFilter::getExecutor() {
	return executor;
}

void EExecutorFilter::destroy() {
	if (manageableExecutor && executor) {
		executor->shutdown();
	}
}

void EExecutorFilter::fireEvent(sp<EIoFilterEvent> event) {
	executor->execute(event);
}

void EExecutorFilter::onPreAdd(EIoFilterChain* parent, const char* name,
		EIoFilter::NextFilter* nextFilter) {
	if (parent->contains(this)) {
		throw EIllegalArgumentException(__FILE__, __LINE__,
				"You can't add the same filter instance more than once.  Create another instance and add it.");
	}
}

void EExecutorFilter::sessionOpened(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session) {
	if ((eventTypeSet & EIoEventType::SESSION_OPENED) == EIoEventType::SESSION_OPENED) {
		EIoFilterEvent* event = new EIoFilterEvent(nextFilter, EIoEventType::SESSION_OPENED, session, null);
		fireEvent(event);
	} else {
		nextFilter->sessionOpened(session);
	}
}

void EExecutorFilter::sessionClosed(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session) {
	if ((eventTypeSet & EIoEventType::SESSION_CLOSED) == EIoEventType::SESSION_CLOSED) {
		EIoFilterEvent* event = new EIoFilterEvent(nextFilter, EIoEventType::SESSION_CLOSED, session, null);
		fireEvent(event);
	} else {
		nextFilter->sessionClosed(session);
	}
}

void EExecutorFilter::sessionIdle(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, EIdleStatus status) {
	if ((eventTypeSet & EIoEventType::SESSION_IDLE) == EIoEventType::SESSION_IDLE) {
		sp<EInteger> parameter = new EInteger(status);
		EIoFilterEvent* event = new EIoFilterEvent(nextFilter, EIoEventType::SESSION_IDLE, session, parameter);
		fireEvent(event);
	} else {
		nextFilter->sessionIdle(session, status);
	}
}

void EExecutorFilter::exceptionCaught(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, sp<EThrowableType>& cause) {
	if ((eventTypeSet & EIoEventType::EXCEPTION_CAUGHT) == EIoEventType::EXCEPTION_CAUGHT) {
		EIoFilterEvent* event = new EIoFilterEvent(nextFilter, EIoEventType::EXCEPTION_CAUGHT, session, cause);
		fireEvent(event);
	} else {
		nextFilter->exceptionCaught(session, cause);
	}
}

void EExecutorFilter::messageReceived(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, sp<EObject>& message) {
	if ((eventTypeSet & EIoEventType::MESSAGE_RECEIVED) == EIoEventType::MESSAGE_RECEIVED) {
		EIoFilterEvent* event = new EIoFilterEvent(nextFilter, EIoEventType::MESSAGE_RECEIVED, session, message);
		fireEvent(event);
	} else {
		nextFilter->messageReceived(session, message);
	}
}

void EExecutorFilter::messageSent(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) {
	if ((eventTypeSet & EIoEventType::MESSAGE_SENT) == EIoEventType::MESSAGE_SENT) {
		EIoFilterEvent* event = new EIoFilterEvent(nextFilter, EIoEventType::MESSAGE_SENT, session, writeRequest);
		fireEvent(event);
	} else {
		nextFilter->messageSent(session, writeRequest);
	}
}

void EExecutorFilter::filterClose(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session) {
	if ((eventTypeSet & EIoEventType::CLOSE) == EIoEventType::CLOSE) {
		EIoFilterEvent* event = new EIoFilterEvent(nextFilter, EIoEventType::CLOSE, session, null);
		fireEvent(event);
	} else {
		nextFilter->filterClose(session);
	}
}

void EExecutorFilter::filterWrite(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) {
	if ((eventTypeSet & EIoEventType::WRITE) == EIoEventType::WRITE) {
		EIoFilterEvent* event = new EIoFilterEvent(nextFilter, EIoEventType::WRITE, session, writeRequest);
		fireEvent(event);
	} else {
		nextFilter->filterWrite(session, writeRequest);
	}
}

} /* namespace eio */
} /* namespace efc */
