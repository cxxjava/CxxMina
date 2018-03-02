/*
 * EAttributeKey.cpp
 *
 *  Created on: 2016-3-3
 *      Author: cxxjava@163.com
 */

#include "../inc/EAttributeKey.hh"

namespace efc {
namespace eio {

EAttributeKey::EAttributeKey(const char* source, const char* name) {
	this->name << source << '.' << name << '@' << EInteger::toHexString(this->hashCode());
}

EString EAttributeKey::toString() {
	return name;
}

int EAttributeKey::hashCode() {
	int h = 17 * 37 + (name.isEmpty() ? 0 : name.hashCode());
	return h;
}

boolean EAttributeKey::equals(EObject* obj) {
	if (!obj) {
		return false;
	}
	if (this == obj) {
		return true;
	}

	EAttributeKey* other = dynamic_cast<EAttributeKey*>(obj);

	if (!other) {
		return false;
	}

	return name.equals(other->name);
}

boolean EAttributeKey::equals(EAttributeKey* obj) {
	if (!obj) {
		return false;
	}
	if (this == obj) {
		return true;
	}

	return name.equals(obj->name);
}

} /* namespace eio */
} /* namespace efc */
