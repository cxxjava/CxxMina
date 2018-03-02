/*
 * ENioDatagramSession.hh
 *
 *  Created on: 2016-8-30
 *      Author: cxxjava@163.com
 */

#ifndef ENIODATAGRAMSESSION_HH_
#define ENIODATAGRAMSESSION_HH_

#include "./ENioSession.hh"
#include "./EDatagramSessionConfig.hh"

namespace efc {
namespace eio {

/**
 * An {@link IoSession} for datagram transport (UDP/IP).
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class ENioDatagramSession: public ENioSession {
public:
	static ETransportMetadata* METADATA();

public:
	virtual ~ENioDatagramSession();

	/**
	 * Creates a new acceptor-side session instance.
	 */
	ENioDatagramSession(EIoService* service, EIoProcessor* processor, sp<EDatagramChannel> channel,
			EInetSocketAddress* remoteAddress);

	/**
	 * Creates a new connector-side session instance.
	 */
	ENioDatagramSession(EIoService* service, EIoProcessor* processor, sp<EDatagramChannel> channel);

	/**
	 * {@inheritDoc}
	 */
	virtual EDatagramSessionConfig* getConfig();

	virtual sp<EByteChannel> getChannel();

	virtual ETransportMetadata* getTransportMetadata();

	virtual EInetSocketAddress* getRemoteAddress();

	virtual EInetSocketAddress* getLocalAddress();

	virtual EInetSocketAddress* getServiceAddress();

private:
	EInetSocketAddress* localAddress;

	EInetSocketAddress* remoteAddress;
};

} /* namespace eio */
} /* namespace efc */
#endif /* ENIODATAGRAMSESSION_HH_ */
