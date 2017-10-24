/*
 * EIoFilterLifeCycleException.hh
 *
 *  Created on: 2016-1-19
 *      Author: cxxjava@163.com
 */

#ifndef EIOFILTERLIFECYCLEEXCEPTION_HH_
#define EIOFILTERLIFECYCLEEXCEPTION_HH_

#include "Efc.hh"

namespace efc {
namespace eio {

#define EIOFILTERLIFECYCLEEXCEPTION        EIoFilterLifeCycleException(__FILE__, __LINE__, errno)
#define EIOFILTERLIFECYCLEEXCEPTIONS(msg)  EIoFilterLifeCycleException(__FILE__, __LINE__, msg)

/**
 * A {@link RuntimeException} which is thrown when {@link IoFilter#init()}
 * or {@link IoFilter#onPostAdd(IoFilterChain, String, org.apache.mina.core.filterchain.IoFilter.NextFilter)}
 * failed.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EIoFilterLifeCycleException: public ERuntimeException {
public:
	/**
	 * Constructs an <code>EIoFilterLifeCycleException</code> with no
	 * detail message.
	 *
	 * @param   _file_   __FILE__
	 * @param   _line_   __LINE__
	 * @param   errn     errno
	 */
	EIoFilterLifeCycleException(const char *_file_, int _line_, int errn = 0) :
		ERuntimeException(_file_, _line_, errn) {
	}

	/**
	 * Constructs an <code>EIoFilterLifeCycleException</code> with the
	 * specified detail message.
	 *
	 * @param   _file_   __FILE__.
	 * @param   _line_   __LINE__.
	 * @param   s   the detail message.
	 */
	EIoFilterLifeCycleException(const char *_file_, int _line_, const char *s, int errn = 0) :
		ERuntimeException(_file_, _line_, s, errn) {
	}

	/**
	 * Constructs an <code>EIoFilterLifeCycleException</code> with the specified detail message.
	 *
	 * @param   _file_   __FILE__
	 * @param   _line_   __LINE__
	 * @param   cause    the cause (which is saved for later retrieval by the
	 *         {@link #getCause()} method).  (A {@code null} value is
	 *         permitted, and indicates that the cause is nonexistent or
	 *         unknown.)
	 */
	EIoFilterLifeCycleException(const char *_file_, int _line_, EThrowable* cause) :
		ERuntimeException(_file_, _line_, cause) {
	}

	/**
	 * Constructs a new exception with the specified detail message and
	 * cause.
	 *
	 * @param   _file_   __FILE__
	 * @param   _line_   __LINE__
	 * @param   s   the detail message.
	 * @param   cause    the cause (which is saved for later retrieval by the
	 *         {@link #getCause()} method).  (A {@code null} value is
	 *         permitted, and indicates that the cause is nonexistent or
	 *         unknown.)
	 */
	EIoFilterLifeCycleException(const char *_file_, int _line_, const char *s, EThrowable* cause) :
		ERuntimeException(_file_, _line_, s, cause) {
	}
};

} /* namespace eio */
} /* namespace efc */
#endif /* EIOFILTERLIFECYCLEEXCEPTION_HH_ */
