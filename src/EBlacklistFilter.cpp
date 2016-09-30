/*
 * EBlacklistFilter.cpp
 *
 *  Created on: 2016-5-7
 *      Author: cxxjava@163.com
 */

#include "EBlacklistFilter.hh"

namespace efc {
namespace eio {

sp<ELogger> EBlacklistFilter::LOGGER = ELoggerManager::getLogger("EBlacklistFilter");

EBlacklistFilter* EBlacklistFilter::setBlacklist(EA<EInetAddress*>* addresses) {
	if (addresses == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "addresses");
	}

	blacklist.clear();

	for (int i = 0; i < addresses->length(); i++) {
		EInetAddress* addr = (*addresses)[i];
		block(addr);
	}

	return this;
}

EBlacklistFilter* EBlacklistFilter::setSubnetBlacklist(EA<ESubnet*>* subnets) {
	if (subnets == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "Subnets must not be null");
	}

	blacklist.clear();

	for (int i = 0; i < subnets->length(); i++) {
		ESubnet* subnet = (*subnets)[i];
		block(subnet);
	}

	return this;
}

EBlacklistFilter* EBlacklistFilter::setBlacklist(EIterable<EInetAddress*>* addresses) {
	if (addresses == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "addresses");
	}

	blacklist.clear();

	sp<EIterator<EInetAddress*> > iter = addresses->iterator();
	while (iter->hasNext()) {
		block(iter->next());
	}

	return this;
}

EBlacklistFilter* EBlacklistFilter::setSubnetBlacklist(EIterable<ESubnet*>* subnets) {
	if (subnets == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "Subnets must not be null");
	}

	blacklist.clear();

	sp<EIterator<ESubnet*> > iter = subnets->iterator();
	while (iter->hasNext()) {
		block(iter->next());
	}

	return this;
}

EBlacklistFilter* EBlacklistFilter::block(EInetAddress* address) {
	if (address == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "Adress to block can not be null");
	}

	blacklist.add(new ESubnet(address, 32));

	return this;
}

EBlacklistFilter* EBlacklistFilter::block(const char* hostname) {
	EInetAddress address = EInetAddress::getByName(hostname);
	return this->block(&address);
}

EBlacklistFilter* EBlacklistFilter::block(ESubnet* subnet) {
	if (subnet == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "Subnet can not be null");
	}

	blacklist.add(new ESubnet(*subnet));

	return this;
}

EBlacklistFilter* EBlacklistFilter::unblock(EInetAddress* address) {
	if (address == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "Adress to unblock can not be null");
	}

	ESubnet subnet(address, 32);
	blacklist.remove(&subnet);

	return this;
}

EBlacklistFilter* EBlacklistFilter::unblock(const char* hostname) {
	EInetAddress address = EInetAddress::getByName(hostname);
	return this->unblock(&address);
}

EBlacklistFilter* EBlacklistFilter::unblock(ESubnet* subnet) {
	if (subnet == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "Subnet can not be null");
	}

	blacklist.remove(subnet);

	return this;
}

void EBlacklistFilter::sessionCreated(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session) {
	if (!isBlocked(session)) {
		// forward if not blocked
		nextFilter->sessionCreated(session);
	} else {
		blockSession(session);
	}
}

void EBlacklistFilter::sessionOpened(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session) {
	if (!isBlocked(session)) {
		// forward if not blocked
		nextFilter->sessionOpened(session);
	} else {
		blockSession(session);
	}
}

void EBlacklistFilter::sessionClosed(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session) {
	if (!isBlocked(session)) {
		// forward if not blocked
		nextFilter->sessionClosed(session);
	} else {
		blockSession(session);
	}
}

void EBlacklistFilter::sessionIdle(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, EIdleStatus status) {
	if (!isBlocked(session)) {
		// forward if not blocked
		nextFilter->sessionIdle(session, status);
	} else {
		blockSession(session);
	}
}

void EBlacklistFilter::messageReceived(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, sp<EObject>& message) {
	if (!isBlocked(session)) {
		// forward if not blocked
		nextFilter->messageReceived(session, message);
	} else {
		blockSession(session);
	}
}

void EBlacklistFilter::messageSent(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) {
	if (!isBlocked(session)) {
		// forward if not blocked
		nextFilter->messageSent(session, writeRequest);
	} else {
		blockSession(session);
	}
}

void EBlacklistFilter::blockSession(sp<EIoSession>& session) {
	LOGGER->warn(__FILE__, __LINE__, "Remote address in the blacklist; closing.");
	session->closeNow();
}

boolean EBlacklistFilter::isBlocked(sp<EIoSession>& session) {
	EInetSocketAddress* remoteAddress = session->getRemoteAddress();

	//if (remoteAddress instanceof InetSocketAddress)
	{
		EInetAddress* address = remoteAddress->getAddress();

		// check all subnets
		sp<EConcurrentIterator<ESubnet> > iter = blacklist.iterator();
		while (iter->hasNext()) {
			sp<ESubnet> subnet = iter->next();
			if (subnet->inSubnet(address)) {
				return true;
			}
		}
	}

	return false;
}

} /* namespace eio */
} /* namespace efc */
