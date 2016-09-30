/*
 * EExceptionMonitor.hh
 *
 *  Created on: 2015-12-28
 *      Author: cxxjava@163.com
 */

#ifndef EEXCEPTIONMONITOR_HH_
#define EEXCEPTIONMONITOR_HH_

#include "Efc.hh"

namespace efc {
namespace eio {

/**
 * Monitors uncaught exceptions.  {@link #exceptionCaught(Throwable)} is
 * invoked when there are any uncaught exceptions.
 * <p>
 * You can monitor any uncaught exceptions by setting {@link ExceptionMonitor}
 * by calling {@link #setInstance(ExceptionMonitor)}.  The default
 * monitor logs all caught exceptions in <tt>WARN</tt> level using
 * SLF4J.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 *
 * @see DefaultExceptionMonitor
 */

abstract class EExceptionMonitor {
public:
	DECLARE_STATIC_INITZZ

public:
	virtual ~EExceptionMonitor();

	/**
	 * Returns the current exception monitor.
	 */
	static EExceptionMonitor* getInstance();

	/**
	 * Sets the uncaught exception monitor.  If <code>null</code> is specified,
	 * the default monitor will be set.
	 *
	 * @param monitor A new instance of {@link DefaultExceptionMonitor} is set
	 *                if <tt>null</tt> is specified.
	 */
	static void setInstance(EExceptionMonitor* monitor);

	/**
	 * Invoked when there are any uncaught exceptions.
	 *
	 * @param cause The caught exception
	 */
	virtual void exceptionCaught(EThrowable& cause) = 0;

protected:
	EExceptionMonitor();

private:
	static EExceptionMonitor* instance;// = new DefaultExceptionMonitor();
};

} /* namespace eio */
} /* namespace efc */
#endif /* EEXCEPTIONMONITOR_HH_ */
