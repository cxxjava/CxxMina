/*
 * ENioSession.hh
 *
 *  Created on: 2016-1-19
 *      Author: cxxjava@163.com
 */

#ifndef ENIOSESSION_HH_
#define ENIOSESSION_HH_

#include "./EAbstractIoSession.hh"
#include "./EIoProcessor.hh"
#include "./EIoSession.hh"

namespace efc {
namespace eio {

abstract class ENioSession: public EAbstractIoSession {
public:
	virtual ~ENioSession();

	EIoFilterChain* getFilterChain();

	/**
	 * {@inheritDoc}
	 */
	virtual void init();

	/**
	 * {@inheritDoc}
	 */
	virtual EIoProcessor* getProcessor();

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isDestroyed();

	/**
	 * {@inheritDoc}
	 */
	virtual void destroy();

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isActive();

	/**
	 * @return The ByteChannel associated with this {@link IoSession}
	 */
	virtual sp<EByteChannel> getChannel() = 0;

	/**
	 * @return The {@link SelectionKey} associated with this {@link IoSession}
	 */
	sp<ESelectionKey> getSelectionKey();

	/**
	 * Sets the {@link SelectionKey} for this {@link IoSession}
	 *
	 * @param key The new {@link SelectionKey}
	 */
	void setSelectionKey(sp<ESelectionKey> key);

protected:
	/** The NioSession processor */
	EIoProcessor* processor;

	/** The communication channel */
	sp<EByteChannel> channel;

	/**
	 * Creates a new instance of NioSession, with its associated IoProcessor.
	 * <br>
	 * This method is only called by the inherited class.
	 *
	 * @param processor The associated {@link IoProcessor}
	 * @param service The associated {@link IoService}
	 * @param channel The associated {@link Channel}
	 */
	ENioSession(EIoProcessor* processor, EIoService* service, sp<EByteChannel> channel);

private:
	/** The SelectionKey used for this session */
	sp<ESelectionKey> key;
	EReentrantLock keyLock;

	/** The FilterChain created for this session */
	EIoFilterChain* filterChain;

	volatile boolean destroyed;
};

} /* namespace eio */
} /* namespace efc */
#endif /* ENIOSESSION_HH_ */

