/*
 * ELoggingFilter.cpp
 *
 *  Created on: 2016-5-7
 *      Author: cxxjava@163.com
 */

#include "ELoggingFilter.hh"
#include "EIoFilterChain.hh"

namespace efc {
namespace eio {

ELoggingFilter::~ELoggingFilter() {
    //
}

ELoggingFilter::ELoggingFilter():
	exceptionCaughtLevel(ELogger::Level::LEVEL_WARN),
	messageSentLevel(ELogger::Level::LEVEL_INFO),
	messageReceivedLevel(ELogger::Level::LEVEL_INFO),
	sessionCreatedLevel(ELogger::Level::LEVEL_INFO),
	sessionOpenedLevel(ELogger::Level::LEVEL_INFO),
	sessionIdleLevel(ELogger::Level::LEVEL_INFO),
	sessionClosedLevel(ELogger::Level::LEVEL_INFO)
{
	this->name = typeid(ELoggingFilter).name();
	logger = ELoggerManager::getLogger(this->name.c_str());
}

ELoggingFilter::ELoggingFilter(const char* name) {
	if (name == null) {
		this->name = typeid(ELoggingFilter).name();
	} else {
		this->name = name;
	}

	logger = ELoggerManager::getLogger(this->name.c_str());
}

EString ELoggingFilter::getName() {
	return name;
}

void ELoggingFilter::sessionCreated(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session) {
	log(sessionCreatedLevel, "CREATED");
	nextFilter->sessionCreated(session);
}

void ELoggingFilter::sessionOpened(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session) {
	log(sessionOpenedLevel, "OPENED");
	nextFilter->sessionOpened(session);
}

void ELoggingFilter::sessionClosed(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session) {
	log(sessionClosedLevel, "CLOSED");
	nextFilter->sessionClosed(session);
}

void ELoggingFilter::sessionIdle(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, EIdleStatus status) {
	log(sessionIdleLevel, "IDLE");
	nextFilter->sessionIdle(session, status);
}

void ELoggingFilter::exceptionCaught(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, sp<EThrowableType>& cause) {
	log(exceptionCaughtLevel, "EXCEPTION :", cause->getThrowable());
	nextFilter->exceptionCaught(session, cause);
}

void ELoggingFilter::messageReceived(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, sp<EObject>& message) {
	log(messageReceivedLevel, "RECEIVED: ", message.get());
	nextFilter->messageReceived(session, message);
}

void ELoggingFilter::messageSent(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) {
	log(messageSentLevel, "SENT: ", writeRequest->getOriginalRequest()->getMessage().get());
	nextFilter->messageSent(session, writeRequest);
}

void ELoggingFilter::setExceptionCaughtLogLevel(ELogger::Level level) {
	exceptionCaughtLevel = level;
}

ELogger::Level ELoggingFilter::getExceptionCaughtLogLevel() {
	return exceptionCaughtLevel;
}

void ELoggingFilter::setMessageReceivedLogLevel(ELogger::Level level) {
	messageReceivedLevel = level;
}

ELogger::Level ELoggingFilter::getMessageReceivedLogLevel() {
	return messageReceivedLevel;
}

void ELoggingFilter::setMessageSentLogLevel(ELogger::Level level) {
	messageSentLevel = level;
}

ELogger::Level ELoggingFilter::getMessageSentLogLevel() {
	return messageSentLevel;
}

void ELoggingFilter::setSessionCreatedLogLevel(ELogger::Level level) {
	sessionCreatedLevel = level;
}

ELogger::Level ELoggingFilter::getSessionCreatedLogLevel() {
	return sessionCreatedLevel;
}

void ELoggingFilter::setSessionOpenedLogLevel(ELogger::Level level) {
	sessionOpenedLevel = level;
}

ELogger::Level ELoggingFilter::getSessionOpenedLogLevel() {
	return sessionOpenedLevel;
}

void ELoggingFilter::setSessionIdleLogLevel(ELogger::Level level) {
	sessionIdleLevel = level;
}

ELogger::Level ELoggingFilter::getSessionIdleLogLevel() {
	return sessionIdleLevel;
}

void ELoggingFilter::setSessionClosedLogLevel(ELogger::Level level) {
	sessionClosedLevel = level;
}

ELogger::Level ELoggingFilter::getSessionClosedLogLevel() {
	return sessionClosedLevel;
}

void ELoggingFilter::log(ELogger::Level eventLevel, const char* message,
		EObject* param) {
	if (message && param) {
		EString msg(message);
		msg << param->toString();
		switch (eventLevel) {
		case ELogger::Level::LEVEL_TRACE:
			logger->trace(msg.c_str());
			return;
		case ELogger::Level::LEVEL_DEBUG:
			logger->debug(msg.c_str());
			return;
		case ELogger::Level::LEVEL_INFO:
			logger->info(msg.c_str());
			return;
		case ELogger::Level::LEVEL_WARN:
			logger->warn(msg.c_str());
			return;
		case ELogger::Level::LEVEL_ERROR:
			logger->error(msg.c_str());
			return;
		default:
			return;
		}
	} else if (message) {
		switch (eventLevel) {
		case ELogger::Level::LEVEL_TRACE:
			logger->trace(message);
			return;
		case ELogger::Level::LEVEL_DEBUG:
			logger->debug(message);
			return;
		case ELogger::Level::LEVEL_INFO:
			logger->info(message);
			return;
		case ELogger::Level::LEVEL_WARN:
			logger->warn(message);
			return;
		case ELogger::Level::LEVEL_ERROR:
			logger->error(message);
			return;
		default:
			return;
		}
	} else if (param) {
		switch (eventLevel) {
		case ELogger::Level::LEVEL_TRACE:
			logger->trace(param->toString().c_str());
			return;
		case ELogger::Level::LEVEL_DEBUG:
			logger->debug(param->toString().c_str());
			return;
		case ELogger::Level::LEVEL_INFO:
			logger->info(param->toString().c_str());
			return;
		case ELogger::Level::LEVEL_WARN:
			logger->warn(param->toString().c_str());
			return;
		case ELogger::Level::LEVEL_ERROR:
			logger->error(param->toString().c_str());
			return;
		default:
			return;
		}
	}
}

void ELoggingFilter::log(ELogger::Level eventLevel, const char* message,
		EThrowable* cause) {
	if (message && cause) {
		EString msg(message);
		msg << cause->toString();
		switch (eventLevel) {
		case ELogger::Level::LEVEL_TRACE:
			logger->trace(msg.c_str());
			return;
		case ELogger::Level::LEVEL_DEBUG:
			logger->debug(msg.c_str());
			return;
		case ELogger::Level::LEVEL_INFO:
			logger->info(msg.c_str());
			return;
		case ELogger::Level::LEVEL_WARN:
			logger->warn(msg.c_str());
			return;
		case ELogger::Level::LEVEL_ERROR:
			logger->error(msg.c_str());
			return;
		default:
			return;
		}
	} else if (message) {
		switch (eventLevel) {
		case ELogger::Level::LEVEL_TRACE:
			logger->trace(message);
			return;
		case ELogger::Level::LEVEL_DEBUG:
			logger->debug(message);
			return;
		case ELogger::Level::LEVEL_INFO:
			logger->info(message);
			return;
		case ELogger::Level::LEVEL_WARN:
			logger->warn(message);
			return;
		case ELogger::Level::LEVEL_ERROR:
			logger->error(message);
			return;
		default:
			return;
		}
	} else if (cause) {
		switch (eventLevel) {
		case ELogger::Level::LEVEL_TRACE:
			logger->trace(cause->toString().c_str());
			return;
		case ELogger::Level::LEVEL_DEBUG:
			logger->debug(cause->toString().c_str());
			return;
		case ELogger::Level::LEVEL_INFO:
			logger->info(cause->toString().c_str());
			return;
		case ELogger::Level::LEVEL_WARN:
			logger->warn(cause->toString().c_str());
			return;
		case ELogger::Level::LEVEL_ERROR:
			logger->error(cause->toString().c_str());
			return;
		default:
			return;
		}
	}
}

} /* namespace eio */
} /* namespace efc */
