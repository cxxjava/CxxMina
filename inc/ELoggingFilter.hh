/*
 * ELoggingFilter.hh
 *
 *  Created on: 2016-5-7
 *      Author: cxxjava@163.com
 */

#ifndef ELOGGINGFILTER_HH_
#define ELOGGINGFILTER_HH_

#include "./EIoFilterAdapter.hh"
#include "ELog.hh"

namespace efc {
namespace eio {

/**
 * Logs all MINA protocol events.  Each event can be
 * tuned to use a different level based on the user's specific requirements.  Methods
 * are in place that allow the user to use either the get or set method for each event
 * and pass in the {@link IoEventType} and the {@link LogLevel}.
 *
 * By default, all events are logged to the {@link LogLevel#INFO} level except
 * {@link IoFilterAdapter#exceptionCaught(IoFilter.NextFilter, IoSession, Throwable)},
 * which is logged to {@link LogLevel#WARN}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @org.apache.xbean.XBean
 */

class ELoggingFilter: public EIoFilterAdapter {
public:
	virtual ~ELoggingFilter();

	/**
	 * Default Constructor.
	 */
	ELoggingFilter();

	/**
	 * Create a new NoopFilter using a name
	 *
	 * @param name the name used to create the logger. If null, will default to "NoopFilter"
	 */
	ELoggingFilter(const char* name);

	/**
	 * @return The logger's name
	 */
	EString getName();

	/**
	 * {@inheritDoc}
	 */
	virtual void sessionCreated(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void sessionOpened(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void sessionClosed(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void sessionIdle(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, EIdleStatus status) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void exceptionCaught(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, sp<EThrowableType>& cause) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void messageReceived(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, sp<EObject>& message) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void messageSent(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) THROWS(EException);

	/**
	 * Set the LogLevel for the ExceptionCaught event.
	 *
	 * @param level The LogLevel to set
	 */
	void setExceptionCaughtLogLevel(ELogger::Level level);

	/**
	 * Get the LogLevel for the ExceptionCaught event.
	 *
	 * @return The LogLevel for the ExceptionCaught eventType
	 */
	ELogger::Level getExceptionCaughtLogLevel();

	/**
	 * Set the LogLevel for the MessageReceived event.
	 *
	 * @param level The LogLevel to set
	 */
	void setMessageReceivedLogLevel(ELogger::Level level);

	/**
	 * Get the LogLevel for the MessageReceived event.
	 *
	 * @return The LogLevel for the MessageReceived eventType
	 */
	ELogger::Level getMessageReceivedLogLevel();

	/**
	 * Set the LogLevel for the MessageSent event.
	 *
	 * @param level The LogLevel to set
	 */
	void setMessageSentLogLevel(ELogger::Level level);

	/**
	 * Get the LogLevel for the MessageSent event.
	 *
	 * @return The LogLevel for the MessageSent eventType
	 */
	ELogger::Level getMessageSentLogLevel();

	/**
	 * Set the LogLevel for the SessionCreated event.
	 *
	 * @param level The LogLevel to set
	 */
	void setSessionCreatedLogLevel(ELogger::Level level);

	/**
	 * Get the LogLevel for the SessionCreated event.
	 *
	 * @return The LogLevel for the SessionCreated eventType
	 */
	ELogger::Level getSessionCreatedLogLevel();

	/**
	 * Set the LogLevel for the SessionOpened event.
	 *
	 * @param level The LogLevel to set
	 */
	void setSessionOpenedLogLevel(ELogger::Level level);

	/**
	 * Get the LogLevel for the SessionOpened event.
	 *
	 * @return The LogLevel for the SessionOpened eventType
	 */
	ELogger::Level getSessionOpenedLogLevel();

	/**
	 * Set the LogLevel for the SessionIdle event.
	 *
	 * @param level The LogLevel to set
	 */
	void setSessionIdleLogLevel(ELogger::Level level);

	/**
	 * Get the LogLevel for the SessionIdle event.
	 *
	 * @return The LogLevel for the SessionIdle eventType
	 */
	ELogger::Level getSessionIdleLogLevel();

	/**
	 * Set the LogLevel for the SessionClosed event.
	 *
	 * @param level The LogLevel to set
	 */
	void setSessionClosedLogLevel(ELogger::Level level);

	/**
	 * Get the LogLevel for the SessionClosed event.
	 *
	 * @return The LogLevel for the SessionClosed eventType
	 */
	ELogger::Level getSessionClosedLogLevel();

private:
	/** The logger name */
	EString name;

	/** The logger */
	sp<ELogger> logger;

	/** The log level for the exceptionCaught event. Default to WARN. */
	ELogger::Level exceptionCaughtLevel;// {ELogger::Level::LEVEL_WARN};

	/** The log level for the messageSent event. Default to INFO. */
	ELogger::Level messageSentLevel;// {ELogger::Level::LEVEL_INFO};

	/** The log level for the messageReceived event. Default to INFO. */
	ELogger::Level messageReceivedLevel;// {ELogger::Level::LEVEL_INFO};

	/** The log level for the sessionCreated event. Default to INFO. */
	ELogger::Level sessionCreatedLevel;// {ELogger::Level::LEVEL_INFO};

	/** The log level for the sessionOpened event. Default to INFO. */
	ELogger::Level sessionOpenedLevel;// {ELogger::Level::LEVEL_INFO};

	/** The log level for the sessionIdle event. Default to INFO. */
	ELogger::Level sessionIdleLevel;// {ELogger::Level::LEVEL_INFO};

	/** The log level for the sessionClosed event. Default to INFO. */
	ELogger::Level sessionClosedLevel;// {ELogger::Level::LEVEL_INFO};

	/**
	 * Log if the logger and the current event log level are compatible. We log
	 * a formated message and its parameters.
	 *
	 * @param eventLevel the event log level as requested by the user
	 * @param message the formated message to log
	 * @param param the parameter injected into the message
	 */
	void log(ELogger::Level eventLevel, const char* message, EObject* param=null);

	/**
	 * Log if the logger and the current event log level are compatible. We log
	 * a message and an exception.
	 *
	 * @param eventLevel the event log level as requested by the user
	 * @param message the message to log
	 * @param cause the exception cause to log
	 */
	void log(ELogger::Level eventLevel, const char* message, EThrowable* cause);
};

} /* namespace eio */
} /* namespace efc */
#endif /* ELOGGINGFILTER_HH_ */
