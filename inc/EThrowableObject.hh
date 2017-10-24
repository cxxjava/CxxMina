/*
 * EThrowableObject.hh
 *
 *  Created on: 2016-1-26
 *      Author: cxxjava@163.com
 */

#ifndef ETHROWABLEOBJECT_HH_
#define ETHROWABLEOBJECT_HH_

#include "Efc.hh"

namespace efc {
namespace eio {

/**
 * EThrowable wrapped for based EObject.
 */

class EThrowableType : public virtual EObject {
public:
	virtual EThrowable* getThrowable() = 0;
	virtual void throwException() = 0;
};

template<typename E>
class EThrowableObject: public EThrowableType {
public:
	EThrowableObject(const E& t) : throwable(t) {
	}

	EThrowableObject(E& t) : throwable(t) {
	}

	EThrowableObject(E* t) : throwable(*t) {
	}

	/**
	 * Get the binded EThrowable object.
	 */
	virtual EThrowable* getThrowable() {
		return &throwable;
	}

	/**
	 * Throw a copy of the binded EThrowable object.
	 */
	virtual void throwException() {
		throw throwable;
	}

private:
	E throwable;
};

} /* namespace eio */
} /* namespace efc */
#endif /* ETHROWABLEOBJECT_HH_ */
