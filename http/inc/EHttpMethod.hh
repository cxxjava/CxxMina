/*
 * EHttpMethod.hh
 *
 *  Created on: 2017-1-4
 *      Author: cxxjava@163.com
 */

#ifndef EHTTPMETHOD_HH_
#define EHTTPMETHOD_HH_

#include "Efc.hh"

namespace efc {
namespace eio {

/**
 * An <code>Enumeration</code> of all known HTTP method codes.
 */

#undef DELETE

class EHttpMethod {
public:
	static EHttpMethod GET;
	static EHttpMethod HEAD;
	static EHttpMethod POST;
	static EHttpMethod PUT;
	static EHttpMethod DELETE;
	static EHttpMethod OPTIONS;
	static EHttpMethod TRACE;
	static EHttpMethod CONNECT;

public:
	virtual ~EHttpMethod(){}

	EHttpMethod* valueOf(const char* value) {

	}

	/**
	 * @return A String representation of this version
	 */
	virtual EString toString() {
		return value;
	}

private:
	EString value;

	EHttpMethod(const char* value) {
		this->value = value;
	}
};

} /* namespace eio */
} /* namespace efc */
#endif /* EHTTPMETHOD_HH_ */
