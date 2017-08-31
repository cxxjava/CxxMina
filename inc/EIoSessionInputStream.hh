/*
 * EIoSessionInputStream.hh
 *
 *  Created on: 2016-6-1
 *      Author: cxxjava@163.com
 */

#ifndef EIOSESSIONINPUTSTREAM_HH_
#define EIOSESSIONINPUTSTREAM_HH_

#include "./EIoBuffer.hh"

namespace efc {
namespace eio {

/**
 * An {@link InputStream} that buffers data read from
 * {@link IoHandler#messageReceived(IoSession, Object)} events.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EIoSessionInputStream: public EInputStream, public ESynchronizeable {
public:
	virtual ~EIoSessionInputStream();

	EIoSessionInputStream();

	void write(EIoBuffer* src);

	void throwException(EIOException& e);

	/**
	 * {@inheritDoc}
	 */
	virtual int read(void *b, int len) THROWS(EIOException);
	virtual int read() THROWS(EIOException);

	/**
	 * {@inheritDoc}
	 */
	virtual long available() THROWS(EIOException);

	/**
	 * {@inheritDoc}
	 */
	virtual void close() THROWS(EIOException);

private:
	EIoBuffer* buf;
	volatile boolean closed;
	volatile boolean released;
	EIOException* exception;

	boolean waitForData() THROWS(EIOException);
	void releaseBuffer();
};

} /* namespace eio */
} /* namespace efc */
#endif /* EIOSESSIONINPUTSTREAM_HH_ */
