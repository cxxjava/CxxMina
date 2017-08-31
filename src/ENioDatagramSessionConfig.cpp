/*
 * ENioDatagramSessionConfig.cpp
 *
 *  Created on: 2016-8-30
 *      Author: cxxjava@163.com
 */

#include "../inc/ENioDatagramSessionConfig.hh"
#include "../inc/ERuntimeIoException.hh"

namespace efc {
namespace eio {

ENioDatagramSessionConfig::ENioDatagramSessionConfig(EDatagramChannel* channel) {
	this->channel = channel;
}

int ENioDatagramSessionConfig::getReceiveBufferSize() {
	try {
		return channel->socket()->getReceiveBufferSize();
	} catch (ESocketException& e) {
		throw ERuntimeIoException(__FILE__, __LINE__, &e);
	}
}

void ENioDatagramSessionConfig::setReceiveBufferSize(int receiveBufferSize) {
	try {
		channel->socket()->setReceiveBufferSize(receiveBufferSize);
	} catch (ESocketException& e) {
		throw ERuntimeIoException(__FILE__, __LINE__, &e);
	}
}

boolean ENioDatagramSessionConfig::isBroadcast() {
	try {
		return channel->socket()->getBroadcast();
	} catch (ESocketException& e) {
		throw ERuntimeIoException(__FILE__, __LINE__, &e);
	}
}

void ENioDatagramSessionConfig::setBroadcast(boolean broadcast) {
	try {
		channel->socket()->setBroadcast(broadcast);
	} catch (ESocketException& e) {
		throw ERuntimeIoException(__FILE__, __LINE__, &e);
	}
}

int ENioDatagramSessionConfig::getSendBufferSize() {
	try {
		return channel->socket()->getSendBufferSize();
	} catch (ESocketException& e) {
		throw ERuntimeIoException(__FILE__, __LINE__, &e);
	}
}

void ENioDatagramSessionConfig::setSendBufferSize(int sendBufferSize) {
	try {
		channel->socket()->setSendBufferSize(sendBufferSize);
	} catch (ESocketException& e) {
		throw ERuntimeIoException(__FILE__, __LINE__, &e);
	}
}

boolean ENioDatagramSessionConfig::isReuseAddress() {
	try {
		return channel->socket()->getReuseAddress();
	} catch (ESocketException& e) {
		throw ERuntimeIoException(__FILE__, __LINE__, &e);
	}
}

void ENioDatagramSessionConfig::setReuseAddress(boolean reuseAddress) {
	try {
		channel->socket()->setReuseAddress(reuseAddress);
	} catch (ESocketException& e) {
		throw ERuntimeIoException(__FILE__, __LINE__, &e);
	}
}

int ENioDatagramSessionConfig::getTrafficClass() {
	try {
		return channel->socket()->getTrafficClass();
	} catch (ESocketException& e) {
		throw ERuntimeIoException(__FILE__, __LINE__, &e);
	}
}

void ENioDatagramSessionConfig::setTrafficClass(int trafficClass) {
	try {
		channel->socket()->setTrafficClass(trafficClass);
	} catch (ESocketException& e) {
		throw ERuntimeIoException(__FILE__, __LINE__, &e);
	}
}

} /* namespace eio */
} /* namespace efc */
