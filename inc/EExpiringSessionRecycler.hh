/*
 * EExpiringSessionRecycler.hh
 *
 *  Created on: 2016-8-29
 *      Author: cxxjava@163.com
 */

#ifndef EEXPIRINGSESSIONRECYCLER_HH_
#define EEXPIRINGSESSIONRECYCLER_HH_

#include "./EIoSessionRecycler.hh"
#include "./EExpiringMap.hh"

namespace efc {
namespace eio {

/**
 * An {@link IoSessionRecycler} with sessions that time out on inactivity.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @org.apache.xbean.XBean
 */

class EExpiringSessionRecycler: public EIoSessionRecycler {
public:
	virtual ~EExpiringSessionRecycler();

	EExpiringSessionRecycler(int timeToLive=DEFAULT_TIME_TO_LIVE,
			int expirationInterval=DEFAULT_EXPIRATION_INTERVAL);

	/**
	 * {@inheritDoc}
	 */
	virtual void put(sp<EIoSession>& session);

	/**
	 * {@inheritDoc}
	 */
	virtual sp<EIoSession> recycle(EInetSocketAddress* remoteAddress);

	/**
	 * {@inheritDoc}
	 */
	virtual void remove(sp<EIoSession>& session);

	void setTimeToLive(int timeToLive);
	int getTimeToLive();
	void setExpirationInterval(int expirationInterval);
	int getExpirationInterval();
	void stopExpiring();

private:
	/** A map used to store the session */
	EExpiringMap<EInetSocketAddress, EIoSession> sessionMap;

	class DefaultExpirationListener: public EExpirationListener<EIoSession> {
	public:
		virtual void expired(sp<EIoSession> expiredSession) {
			expiredSession->closeNow();
		}
	};
};

} /* namespace eio */
} /* namespace efc */
#endif /* EEXPIRINGSESSIONRECYCLER_HH_ */
