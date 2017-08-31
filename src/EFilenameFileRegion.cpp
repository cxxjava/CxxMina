/*
 * EFilenameFileRegion.cpp
 *
 *  Created on: 2016-1-19
 *      Author: cxxjava@163.com
 */

#include "../inc/EFilenameFileRegion.hh"

namespace efc {
namespace eio {

EFilenameFileRegion::~EFilenameFileRegion() {
	delete file; //!
}

EFilenameFileRegion::EFilenameFileRegion(EFile* file, EFileChannel* channel) :
				EDefaultFileRegion(channel) {
	if (file == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "file can not be null");
	}
	this->file = file;
}

EFilenameFileRegion::EFilenameFileRegion(EFile* file, EFileChannel* channel,
		llong position, llong remainingBytes) :
		EDefaultFileRegion(channel, position, remainingBytes) {
	if (file == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "file can not be null");
	}
	this->file = file;
}

EString EFilenameFileRegion::getFilename() {
	return file->getAbsolutePath();
}

} /* namespace eio */
} /* namespace efc */
