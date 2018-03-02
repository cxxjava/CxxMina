/*
 * EIoFilterAdapter.hh
 *
 *  Created on: 2016-1-22
 *      Author: cxxjava@163.com
 */

#ifndef EIOFILTERADAPTER_HH_
#define EIOFILTERADAPTER_HH_

#include "./EIoFilter.hh"

namespace efc {
namespace eio {

/**
 * An adapter class for {@link IoFilter}.  You can extend
 * this class and selectively override required event filter methods only.  All
 * methods forwards events to the next filter by default.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EIoFilterAdapter: virtual public EIoFilter {
public:
	/**
	 * {@inheritDoc}
	 */
	virtual void init() THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void destroy() THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void onPreAdd(EIoFilterChain* parent, const char* name, EIoFilter::NextFilter* nextFilter) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void onPostAdd(EIoFilterChain* parent, const char* name, EIoFilter::NextFilter* nextFilter) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void onPreRemove(EIoFilterChain* parent, const char* name, EIoFilter::NextFilter* nextFilter) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void onPostRemove(EIoFilterChain* parent, const char* name, EIoFilter::NextFilter* nextFilter) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void sessionCreated(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void sessionOpened(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void sessionClosed(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void sessionIdle(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, EIdleStatus status) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void exceptionCaught(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, sp<EThrowableType>& cause) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void inputClosed(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void messageReceived(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, sp<EObject>& message) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void messageSent(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void filterClose(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void filterWrite(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) THROWS(EException);

	/**
	 *
	 */
	virtual EString toString();
};

} /* namespace eio */
} /* namespace efc */
#endif /* EIOFILTERADAPTER_HH_ */
