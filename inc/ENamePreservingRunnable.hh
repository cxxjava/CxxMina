/*
 * ENamePreservingRunnable.hh
 *
 *  Created on: 2016-1-29
 *      Author: cxxjava@163.com
 */

#ifndef ENAMEPRESERVINGRUNNABLE_HH_
#define ENAMEPRESERVINGRUNNABLE_HH_

#include "Efc.hh"

namespace efc {
namespace eio {

/**
 * A {@link Runnable} wrapper that preserves the name of the thread after the runnable is
 * complete (for {@link Runnable}s that change the name of the Thread they use.)
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class ENamePreservingRunnable: public virtual ERunnable {
public:
	virtual ~ENamePreservingRunnable();

	/**
     * Creates a new instance of NamePreservingRunnable.
     *
     * @param runnable The underlying runnable
     * @param newName The runnable's name
     */
	ENamePreservingRunnable(sp<ERunnable> runnable, EString& newName);

	/**
	 * Run the runnable after having renamed the current thread's name
	 * to the new name. When the runnable has completed, set back the
	 * current thread name back to its origin.
	 */
	virtual void run();

private:
	/** The runnable name */
	EString newName;

	/** The runnable task */
	sp<ERunnable> runnable;

	/**
	 * Wraps {@link Thread#setName(String)} to catch a possible {@link Exception}s such as
	 * {@link SecurityException} in sandbox environments, such as applets
	 */
	void setName(EThread* thread, const char* name);
};

} /* namespace eio */
} /* namespace efc */
#endif /* ENAMEPRESERVINGRUNNABLE_HH_ */
