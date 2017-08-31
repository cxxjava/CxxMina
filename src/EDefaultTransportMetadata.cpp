/*
 * EDefaultTransportMetadata.cpp
 *
 *  Created on: 2016-3-14
 *      Author: cxxjava@163.com
 */

#include "../inc/EDefaultTransportMetadata.hh"

namespace efc {
namespace eio {

EDefaultTransportMetadata::EDefaultTransportMetadata(const char* p,
		const char* n, boolean c, boolean f) :
		providerName(p), name(n), connectionless(c), fragmentation(f) {
	providerName.toLowerCase();
	name.toLowerCase();
}

const char* EDefaultTransportMetadata::getProviderName() {
	return providerName.c_str();
}

const char* EDefaultTransportMetadata::getName() {
	return  name.c_str();
}

boolean EDefaultTransportMetadata::isConnectionless() {
	return connectionless;
}

boolean EDefaultTransportMetadata::hasFragmentation() {
	return fragmentation;
}

} /* namespace eio */
} /* namespace efc */
