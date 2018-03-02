/*
 * EHttpVersion.hh
 *
 *  Created on: 2017-1-4
 *      Author: cxxjava@163.com
 */

#ifndef EHTTPVERSION_HH_
#define EHTTPVERSION_HH_

#include "Efc.hh"

namespace efc {
namespace eio {

/**
 * An <code>Enumeration</code> of all known HTTP method codes.
 */

class EHttpVersion {
public:
	static EHttpVersion HTTP_1_1;
	static EHttpVersion HTTP_1_0;

public:
	virtual ~EHttpVersion(){}

	/**
	 * Returns the {@link HttpVersion} instance from the specified string.
	 *
	 * @param string The String contaoning the HTTP version
	 * @return The version, or <code>null</code> if no version is found
	 */
	static EHttpVersion* fromString(const char* string) {
		if (HTTP_1_1.value.equalsIgnoreCase(string)) {
			return &HTTP_1_1;
		}

		if (HTTP_1_0.value.equalsIgnoreCase(string)) {
			return &HTTP_1_0;
		}

		return null;
	}

	/**
	 * @return A String representation of this version
	 */
	virtual EString toString() {
		return value;
	}

private:
	EString value;

	EHttpVersion(const char* value) {
		this->value = value;
	}
};

} /* namespace eio */
} /* namespace efc */
#endif /* EHTTPVERSION_HH_ */
