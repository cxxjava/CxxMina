/*
 * EStreamIoHandler.hh
 *
 *  Created on: 2016-6-1
 *      Author: cxxjava@163.com
 */

#ifndef ESTREAMIOHANDLER_HH_
#define ESTREAMIOHANDLER_HH_

#include "./EIoHandlerAdapter.hh"
#include "./EAttributeKey.hh"
#include "ELog.hh"

namespace efc {
namespace eio {

/**
 * A {@link IoHandler} that adapts asynchronous MINA events to stream I/O.
 * <p>
 * Please extend this class and implement
 * {@link #processStreamIo(IoSession, InputStream, OutputStream)} to
 * execute your stream I/O logic; <b>please note that you must forward
 * the process request to other thread or thread pool.</b>
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

abstract class EStreamIoHandler: public EIoHandlerAdapter {
public:
	virtual ~EStreamIoHandler();

	/**
	 * @return read timeout in seconds.
	 * The default value is <tt>0</tt> (disabled).
	 */
	virtual int getReadTimeout();

	/**
	 * Sets read timeout in seconds.
	 * The default value is <tt>0</tt> (disabled).
	 * @param readTimeout The Read timeout
	 */
	virtual void setReadTimeout(int readTimeout);

	/**
	 * @return write timeout in seconds.
	 * The default value is <tt>0</tt> (disabled).
	 */
	virtual int getWriteTimeout();

	/**
	 * Sets write timeout in seconds.
	 * The default value is <tt>0</tt> (disabled).
	 *
	 * @param writeTimeout The Write timeout
	 */
	virtual void setWriteTimeout(int writeTimeout);

	virtual void sessionOpened(sp<EIoSession>& session) THROWS(EException);
	virtual void sessionClosed(sp<EIoSession>& session) THROWS(EException);
	virtual void sessionIdle(sp<EIoSession>& session, EIdleStatus status) THROWS(EException);
	virtual void messageReceived(sp<EIoSession>& session, sp<EObject>& message) THROWS(EException);
	virtual void exceptionCaught(sp<EIoSession>& session, sp<EThrowableType>& cause) THROWS(EException);

protected:
	EStreamIoHandler();

	/**
	 * Implement this method to execute your stream I/O logic;
	 * <b>please note that you must forward the process request to other
	 * thread or thread pool.</b>
	 *
	 * @param session The current session
	 * @param in The input stream
	 * @param out The output stream
	 */
	virtual void processStreamIo(sp<EIoSession>& session, sp<EInputStream>& in, sp<EOutputStream>& out) = 0;

private:
	static sp<ELogger> LOGGER;// = LoggerFactory.getLogger(StreamIoHandler.class);

	static sp<EAttributeKey> KEY_IN;// = new AttributeKey(StreamIoHandler.class, "in");
	static sp<EAttributeKey> KEY_OUT;// = new AttributeKey(StreamIoHandler.class, "out");

	int readTimeout;
	int writeTimeout;

	class StreamIoException: public ERuntimeException {
	public:
		StreamIoException(const char *_file_, int _line_, const char *s, int errn = 0) :
			ERuntimeException(_file_, _line_, s, errn) {
		}
	};
};

} /* namespace eio */
} /* namespace efc */
#endif /* ESTREAMIOHANDLER_HH_ */
