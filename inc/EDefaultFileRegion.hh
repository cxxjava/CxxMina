/*
 * EDefaultFileRegion.hh
 *
 *  Created on: 2016-1-19
 *      Author: cxxjava@163.com
 */

#ifndef EDEFAULTFILEREGION_HH_
#define EDEFAULTFILEREGION_HH_

#include "./EFileRegion.hh"

namespace efc {
namespace eio {

/**
 * TODO Add documentation
 *
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EDefaultFileRegion : public EFileRegion {
public:
	virtual ~EDefaultFileRegion();

	EDefaultFileRegion(EFileChannel* channel) THROWS(EIOException);

	EDefaultFileRegion(EFileChannel* channel, llong position, llong remainingBytes) THROWS(EIOException);

	virtual llong getWrittenBytes();

	virtual llong getRemainingBytes();

	virtual EFileChannel* getFileChannel();

	virtual llong getPosition();

	virtual void update(long value) ;

	virtual EString getFilename();

private:
	EFileChannel* channel;

    long originalPosition;

    long position;

    long remainingBytes;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EDEFAULTFILEREGION_HH_ */
