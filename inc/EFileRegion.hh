/*
 * EFileRegion.hh
 *
 *  Created on: 2016-1-19
 *      Author: cxxjava@163.com
 */

#ifndef EFILEREGION_HH_
#define EFILEREGION_HH_

#include "Efc.hh"

namespace efc {
namespace eio {

/**
 * Indicates the region of a file to be sent to the remote host.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

interface EFileRegion : virtual public EObject {
	virtual ~EFileRegion(){
	}

	/**
	 * The open <tt>FileChannel</tt> from which data will be read to send to
	 * remote host.
	 *
	 * @return  An open <tt>FileChannel</tt>.
	 */
	virtual EFileChannel* getFileChannel() = 0;

	/**
	 * The current file position from which data will be read.
	 *
	 * @return  The current file position.
	 */
	virtual llong getPosition() = 0;

	/**
	 * Updates the current file position based on the specified amount. This
	 * increases the value returned by {@link #getPosition()} and
	 * {@link #getWrittenBytes()} by the given amount and decreases the value
	 * returned by {@link #getRemainingBytes()} by the given {@code amount}.
	 *
	 * @param amount
	 *            The new value for the file position.
	 */
	virtual void update(long amount) = 0;

	/**
	 * The number of bytes remaining to be written from the file to the remote
	 * host.
	 *
	 * @return  The number of bytes remaining to be written.
	 */
	virtual llong getRemainingBytes() = 0;

	/**
	 * The total number of bytes already written.
	 *
	 * @return  The total number of bytes already written.
	 */
	virtual llong getWrittenBytes() = 0;

	/**
	 * Provides an absolute filename for the underlying FileChannel.
	 *
	 * @return  the absolute filename, or <tt>null</tt> if the FileRegion
	 *   does not know the filename
	 */
	virtual EString getFilename() = 0;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EFILEREGION_HH_ */
