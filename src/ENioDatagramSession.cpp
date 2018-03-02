/*
 * ENioDatagramSession.cpp
 *
 *  Created on: 2016-8-30
 *      Author: cxxjava@163.com
 */

#include "../inc/ENioDatagramSession.hh"
#include "../inc/ENioDatagramSessionConfig.hh"
#include "../inc/EIoService.hh"
#include "../inc/EDefaultTransportMetadata.hh"

namespace efc {
namespace eio {

ENioDatagramSession::~ENioDatagramSession() {
	delete config;
	delete remoteAddress;
	delete localAddress;
}

ENioDatagramSession::ENioDatagramSession(EIoService* service, EIoProcessor* processor, sp<EDatagramChannel> channel,
		EInetSocketAddress* remoteAddress): ENioSession(processor, service, channel) {
	config = new ENioDatagramSessionConfig(channel);
	config->setAll(service->getSessionConfig());

	this->remoteAddress = remoteAddress ? new EInetSocketAddress(*remoteAddress) : null;
	sp<EInetSocketAddress> l = channel->socket()->getLocalSocketAddress();
	this->localAddress = (l != null) ? new EInetSocketAddress(*l) : null;
}

ENioDatagramSession::ENioDatagramSession(EIoService* service, EIoProcessor* processor, sp<EDatagramChannel> channel):
	ENioSession(processor, service, channel)  {
	config = new ENioDatagramSessionConfig(channel);
	config->setAll(service->getSessionConfig());

	sp<EInetSocketAddress> r = channel->socket()->getRemoteSocketAddress();
	this->remoteAddress = (r != null) ? new EInetSocketAddress(*r) : null;
	sp<EInetSocketAddress> l = channel->socket()->getLocalSocketAddress();
	this->localAddress = (l != null) ? new EInetSocketAddress(*l) : null;
}

EDatagramSessionConfig* ENioDatagramSession::getConfig() {
	return dynamic_cast<EDatagramSessionConfig*>(config);
}

sp<EByteChannel> ENioDatagramSession::getChannel() {
	return channel;
}

ETransportMetadata* ENioDatagramSession::METADATA() {
	static EDefaultTransportMetadata metadata("nio", "datagram", true, false);
	return &metadata;
}

ETransportMetadata* ENioDatagramSession::getTransportMetadata() {
	return METADATA();
}

EInetSocketAddress* ENioDatagramSession::getRemoteAddress() {
	return remoteAddress;
}

EInetSocketAddress* ENioDatagramSession::getLocalAddress() {
	return localAddress;
}

EInetSocketAddress* ENioDatagramSession::getServiceAddress() {
	return ENioSession::getServiceAddress();
}

} /* namespace eio */
} /* namespace efc */
