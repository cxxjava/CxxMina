/*
 * EFilenameFileRegion.hh
 *
 *  Created on: 2016-1-19
 *      Author: cxxjava@163.com
 */

#ifndef EFILENAMEFILEREGION_HH_
#define EFILENAMEFILEREGION_HH_

#include "./EDefaultFileRegion.hh"

namespace efc {
namespace eio {

class EFilenameFileRegion: public EDefaultFileRegion {
public:
	virtual ~EFilenameFileRegion();

	EFilenameFileRegion(EFile* file, EFileChannel* channel) THROWS(EIOException);

	EFilenameFileRegion(EFile* file, EFileChannel* channel, llong position, llong remainingBytes) THROWS(EIOException);

	virtual EString getFilename();

private:
	EFile* file;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EFILENAMEFILEREGION_HH_ */
