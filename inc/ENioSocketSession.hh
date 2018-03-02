/*
 * ENioSocketSession.hh
 *
 *  Created on: 2016-1-19
 *      Author: cxxjava@163.com
 */

#ifndef ENIOSOCKETSESSION_HH_
#define ENIOSOCKETSESSION_HH_

#include "./ENioSession.hh"
#include "./ESocketSessionConfig.hh"

namespace efc {
namespace eio {

class ENioSocketSession: public ENioSession {
public:
	static ETransportMetadata* METADATA();

public:
	virtual ~ENioSocketSession();

	/**
	 *
	 * Creates a new instance of NioSocketSession.
	 *
	 * @param service the associated IoService
	 * @param processor the associated IoProcessor
	 * @param ch the used channel
	 */
	ENioSocketSession(EIoService* service, EIoProcessor* processor, sp<ESocketChannel> channel);

	/**
	 * {@inheritDoc}
	 */
	virtual ETransportMetadata* getTransportMetadata();

	/**
	 * {@inheritDoc}
	 */
	virtual ESocketSessionConfig* getConfig();

	/*
	 * @Override
	 */
	virtual sp<EByteChannel> getChannel();

	/**
	 * {@inheritDoc}
	 */
	virtual EInetSocketAddress* getRemoteAddress();

	/**
	 * {@inheritDoc}
	 */
	virtual EInetSocketAddress* getLocalAddress();

	/*
	 * @Override
	 */
	virtual EInetSocketAddress* getServiceAddress();

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isSecured();

private:
	friend class SessionConfigImpl;

	ESocket* getSocket();
};

} /* namespace eio */
} /* namespace efc */
#endif /* ENIOSOCKETSESSION_HH_ */
