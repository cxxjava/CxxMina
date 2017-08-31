/*
 * ETextLineCodecFactory.cpp
 *
 *  Created on: 2016-5-24
 *      Author: cxxjava@163.com
 */

#include "../inc/ETextLineCodecFactory.hh"

namespace efc {
namespace eio {

ETextLineCodecFactory::~ETextLineCodecFactory() {
	delete encoder;
	delete decoder;
}

ETextLineCodecFactory::ETextLineCodecFactory(const char* delimiter) {
	encoder = new ETextLineEncoder(delimiter);
	decoder = new ETextLineDecoder(delimiter);
}

ETextLineCodecFactory::ETextLineCodecFactory(const char* encodingDelimiter, const char* decodingDelimiter) {
	encoder = new ETextLineEncoder(encodingDelimiter);
	decoder = new ETextLineDecoder(decodingDelimiter);
}

EProtocolEncoder* ETextLineCodecFactory::getEncoder(sp<EIoSession>& session) {
	return encoder;
}

EProtocolDecoder* ETextLineCodecFactory::getDecoder(sp<EIoSession>& session) {
	return decoder;
}

} /* namespace eio */
} /* namespace efc */
