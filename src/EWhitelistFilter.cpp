/*
 * EWhitelistFilter.cpp
 *
 *  Created on: 2016-5-7
 *      Author: cxxjava@163.com
 */

#include "../inc/EWhitelistFilter.hh"

namespace efc {
namespace eio {

sp<ELogger> EWhitelistFilter::LOGGER = ELoggerManager::getLogger("EWhitelistFilter");

EWhitelistFilter* EWhitelistFilter::setWhitelist(EA<EInetAddress*>* addresses) {
	if (addresses == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "addresses");
	}

	whitelist.clear();

	for (int i = 0; i < addresses->length(); i++) {
		EInetAddress* addr = (*addresses)[i];
		allow(addr);
	}

	return this;
}

EWhitelistFilter* EWhitelistFilter::setSubnetWhitelist(EA<ESubnet*>* subnets) {
	if (subnets == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "Subnets must not be null");
	}

	whitelist.clear();

	for (int i = 0; i < subnets->length(); i++) {
		ESubnet* subnet = (*subnets)[i];
		allow(subnet);
	}

	return this;
}

EWhitelistFilter* EWhitelistFilter::setWhitelist(EIterable<EInetAddress*>* addresses) {
	if (addresses == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "addresses");
	}

	whitelist.clear();

	sp<EIterator<EInetAddress*> > iter = addresses->iterator();
	while (iter->hasNext()) {
		allow(iter->next());
	}

	return this;
}

EWhitelistFilter* EWhitelistFilter::setSubnetWhitelist(EIterable<ESubnet*>* subnets) {
	if (subnets == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "Subnets must not be null");
	}

	whitelist.clear();

	sp<EIterator<ESubnet*> > iter = subnets->iterator();
	while (iter->hasNext()) {
		allow(iter->next());
	}

	return this;
}

EWhitelistFilter* EWhitelistFilter::allow(EInetAddress* address) {
	if (address == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "Adress to allow can not be null");
	}

	whitelist.add(new ESubnet(address, 32));

	return this;
}

EWhitelistFilter* EWhitelistFilter::allow(const char* hostname) {
	EInetAddress address = EInetAddress::getByName(hostname);
	return this->allow(&address);
}

EWhitelistFilter* EWhitelistFilter::allow(ESubnet* subnet) {
	if (subnet == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "Subnet can not be null");
	}

	whitelist.add(new ESubnet(*subnet));

	return this;
}

EWhitelistFilter* EWhitelistFilter::disallow(EInetAddress* address) {
	if (address == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "Adress to disallow can not be null");
	}

	ESubnet subnet(address, 32);
	whitelist.remove(&subnet);

	return this;
}

EWhitelistFilter* EWhitelistFilter::disallow(const char* hostname) {
	EInetAddress address = EInetAddress::getByName(hostname);
	return this->disallow(&address);
}

EWhitelistFilter* EWhitelistFilter::disallow(ESubnet* subnet) {
	if (subnet == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "Subnet can not be null");
	}

	whitelist.remove(subnet);

	return this;
}

void EWhitelistFilter::sessionCreated(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session) {
	if (isAllowed(session)) {
		// forward if allowed
		nextFilter->sessionCreated(session);
	} else {
		blockSession(session);
	}
}

void EWhitelistFilter::sessionOpened(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session) {
	if (isAllowed(session)) {
		// forward if allowed
		nextFilter->sessionOpened(session);
	} else {
		blockSession(session);
	}
}

void EWhitelistFilter::sessionClosed(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session) {
	if (isAllowed(session)) {
		// forward if allowed
		nextFilter->sessionClosed(session);
	} else {
		blockSession(session);
	}
}

void EWhitelistFilter::sessionIdle(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, EIdleStatus status) {
	if (isAllowed(session)) {
		// forward if allowed
		nextFilter->sessionIdle(session, status);
	} else {
		blockSession(session);
	}
}

void EWhitelistFilter::messageReceived(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, sp<EObject>& message) {
	if (isAllowed(session)) {
		// forward if allowed
		nextFilter->messageReceived(session, message);
	} else {
		blockSession(session);
	}
}

void EWhitelistFilter::messageSent(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) {
	if (isAllowed(session)) {
		// forward if allowed
		nextFilter->messageSent(session, writeRequest);
	} else {
		blockSession(session);
	}
}

void EWhitelistFilter::blockSession(sp<EIoSession>& session) {
	LOGGER->warn(__FILE__, __LINE__, "Remote address not in the whitelist; closing.");
	session->closeNow();
}

boolean EWhitelistFilter::isAllowed(sp<EIoSession>& session) {
	EInetSocketAddress* remoteAddress = session->getRemoteAddress();

	//if (remoteAddress instanceof InetSocketAddress)
	{
		EInetAddress* address = remoteAddress->getAddress();

		// check all subnets
		sp<EConcurrentIterator<ESubnet> > iter = whitelist.iterator();
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
