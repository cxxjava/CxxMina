/*
 * EDefaultFileRegion.cpp
 *
 *  Created on: 2016-1-19
 *      Author: cxxjava@163.com
 */

#include "../inc/EDefaultFileRegion.hh"

namespace efc {
namespace eio {

EDefaultFileRegion::~EDefaultFileRegion() {
	delete channel; //!
}

EDefaultFileRegion::EDefaultFileRegion(EFileChannel* channel) {
	if (channel == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "channel can not be null");
	}
	this->channel = channel;
	this->originalPosition = 0;
	this->position = 0;
	this->remainingBytes = channel->size();
}

EDefaultFileRegion::EDefaultFileRegion(EFileChannel* channel, llong position,
		llong remainingBytes) {
	if (channel == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "channel can not be null");
	}
	if (position < 0) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "position may not be less than 0");
	}
	if (remainingBytes < 0) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "remainingBytes may not be less than 0");
	}
	this->channel = channel;
	this->originalPosition = position;
	this->position = position;
	this->remainingBytes = remainingBytes;
}

llong EDefaultFileRegion::getWrittenBytes() {
	return position - originalPosition;
}

llong EDefaultFileRegion::getRemainingBytes() {
	return remainingBytes;
}

EFileChannel* EDefaultFileRegion::getFileChannel() {
	return channel;
}

llong EDefaultFileRegion::getPosition() {
	return position;
}

void EDefaultFileRegion::update(long value) {
	position += value;
	remainingBytes -= value;
}

EString EDefaultFileRegion::getFilename() {
	return null;
}

} /* namespace eio */
} /* namespace efc */
