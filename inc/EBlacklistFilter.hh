/*
 * EBlacklistFilter.hh
 *
 *  Created on: 2016-5-7
 *      Author: cxxjava@163.com
 */

#ifndef EBLACKLISTFILTER_HH_
#define EBLACKLISTFILTER_HH_

#include "./EIoFilterAdapter.hh"
#include "./ESubnet.hh"
#include "ELog.hh"

namespace efc {
namespace eio {

/**
 * A {@link IoFilter} which blocks connections from blacklisted remote
 * address.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @org.apache.xbean.XBean
 */

class EBlacklistFilter: public EIoFilterAdapter {
public:
	/**
	 * Sets the addresses to be blacklisted.
	 *
	 * NOTE: this call will remove any previously blacklisted addresses.
	 *
	 * @param addresses an array of addresses to be blacklisted.
	 */
	EBlacklistFilter* setBlacklist(EA<EInetAddress*>* addresses);

	/**
	 * Sets the subnets to be blacklisted.
	 *
	 * NOTE: this call will remove any previously blacklisted subnets.
	 *
	 * @param subnets an array of subnets to be blacklisted.
	 */
	EBlacklistFilter* setSubnetBlacklist(EA<ESubnet*>* subnets);

	/**
	 * Sets the addresses to be blacklisted.
	 *
	 * NOTE: this call will remove any previously blacklisted addresses.
	 *
	 * @param addresses a collection of InetAddress objects representing the
	 *        addresses to be blacklisted.
	 * @throws IllegalArgumentException if the specified collections contains
	 *         non-{@link InetAddress} objects.
	 */
	EBlacklistFilter* setBlacklist(EIterable<EInetAddress*>* addresses);

	/**
	 * Sets the subnets to be blacklisted.
	 *
	 * NOTE: this call will remove any previously blacklisted subnets.
	 *
	 * @param subnets an array of subnets to be blacklisted.
	 */
	EBlacklistFilter* setSubnetBlacklist(EIterable<ESubnet*>* subnets);

	/**
	 * Blocks the specified endpoint.
	 *
	 * @param address The address to block
	 */
	EBlacklistFilter* block(EInetAddress* address);
	EBlacklistFilter* block(const char* hostname);

	/**
	 * Blocks the specified subnet.
	 *
	 * @param subnet The subnet to block
	 */
	EBlacklistFilter* block(ESubnet* subnet);

	/**
	 * Unblocks the specified endpoint.
	 *
	 * @param address The address to unblock
	 */
	EBlacklistFilter* unblock(EInetAddress* address);
	EBlacklistFilter* unblock(const char* hostname);

	/**
	 * Unblocks the specified subnet.
	 *
	 * @param subnet The subnet to unblock
	 */
	EBlacklistFilter* unblock(ESubnet* subnet);

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
	virtual void messageReceived(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, sp<EObject>& message) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual void messageSent(EIoFilter::NextFilter* nextFilter, sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) THROWS(EException);

private:
	static sp<ELogger> LOGGER;// = LoggerFactory.getLogger(BlacklistFilter.class);

	/** The list of blocked addresses */
	ECopyOnWriteArrayList<ESubnet> blacklist;

	void blockSession(sp<EIoSession>& session);

	boolean isBlocked(sp<EIoSession>& session);
};

} /* namespace eio */
} /* namespace efc */
#endif /* EBLACKLISTFILTER_HH_ */
