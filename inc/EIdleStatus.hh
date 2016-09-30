/*
 * EIdleStatus.hh
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#ifndef EIDLESTATUS_HH_
#define EIDLESTATUS_HH_

namespace efc {
namespace eio {


/**
 * Represents the type of idleness of {@link IoSession} or
 * {@link IoSession}.  There are three types of idleness:
 * <ul>
 *   <li>{@link #READER_IDLE} - No data is coming from the remote peer.</li>
 *   <li>{@link #WRITER_IDLE} - Session is not writing any data.</li>
 *   <li>{@link #BOTH_IDLE} - Both {@link #READER_IDLE} and {@link #WRITER_IDLE}.</li>
 * </ul>
 * <p>
 * Idle time settings are all disabled by default.  You can enable them
 * using {@link IoSessionConfig#setIdleTime(IdleStatus,int)}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */
enum EIdleStatus {
	/**
	 * Represents the session status that no data is coming from the remote
	 * peer.
	 */
	READER_IDLE = 0,

	/**
	 * Represents the session status that the session is not writing any data.
	 */
	WRITER_IDLE = 1,

	/**
	 * Represents both {@link #READER_IDLE} and {@link #WRITER_IDLE}.
	 */
	BOTH_IDLE = 2
};

} /* namespace eio */
} /* namespace efc */

#endif /* EIDLESTATUS_HH_ */
