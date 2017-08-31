/*
 * EIoSessionOutputStream.hh
 *
 *  Created on: 2016-6-1
 *      Author: cxxjava@163.com
 */

#ifndef EIOSESSIONOUTPUTSTREAM_HH_
#define EIOSESSIONOUTPUTSTREAM_HH_

#include "./EIoSession.hh"
#include "./EWriteFuture.hh"
#include "./EIoBuffer.hh"

namespace efc {
namespace eio {

/**
 * An {@link OutputStream} that forwards all write operations to
 * the associated {@link IoSession}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EIoSessionOutputStream: public EOutputStream, public ESynchronizeable {
public:
	EIoSessionOutputStream(sp<EIoSession>& session);

	/**
	 * {@inheritDoc}
	 */
	virtual void write(const void *b, int len) THROWS(EIOException);
	virtual void write(int b) THROWS(EIOException);

	/**
	 * {@inheritDoc}
	 */
	virtual synchronized void flush() THROWS(EIOException);

	/**
	 * {@inheritDoc}
	 */
	virtual void close() THROWS(EIOException);

private:
    wp<EIoSession> session;

	sp<EWriteFuture> lastWriteFuture;

	synchronized void write(sp<EIoBuffer> buf) THROWS(EIOException);
};

} /* namespace eio */
} /* namespace efc */
#endif /* EIOSESSIONOUTPUTSTREAM_HH_ */
