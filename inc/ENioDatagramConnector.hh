/*
 * ENioDatagramConnector.hh
 *
 *  Created on: 2016-8-30
 *      Author: cxxjava@163.com
 */

#ifndef ENIODATAGRAMCONNECTOR_HH_
#define ENIODATAGRAMCONNECTOR_HH_

#include "Efc.hh"
#include "./EDatagramConnector.hh"
#include "./EAbstractPollingIoConnector.hh"
#include "./ETransportMetadata.hh"
#include "./EDefaultDatagramSessionConfig.hh"
#include "./EDatagramSessionConfig.hh"

namespace efc {
namespace eio {

class ENioDatagramConnector: public EAbstractPollingIoConnector<sp<EDatagramChannel> >,
	virtual public EDatagramConnector {
public:
	virtual ~ENioDatagramConnector();

	/**
	 * Creates a new instance.
	 */
	ENioDatagramConnector();

	/**
	 * Creates a new instance.
	 *
	 * @param processorCount The number of IoProcessor instance to create
	 */
	ENioDatagramConnector(int processorCount);

	/**
	 * Creates a new instance.
	 *
	 * @param processor The IoProcessor instance to use
	 */
	ENioDatagramConnector(EIoProcessor* processor);

	virtual EDatagramSessionConfig* getSessionConfig();
	virtual ETransportMetadata* getTransportMetadata();
	virtual EInetSocketAddress* getDefaultRemoteAddress();
	virtual void setDefaultRemoteAddress(EInetSocketAddress* defaultRemoteAddress);

	virtual sp<EConnectFuture> connect();
	virtual sp<EConnectFuture> connect(EIoSessionInitializer* sessionInitializer);
	virtual sp<EConnectFuture> connect(EInetSocketAddress* remoteAddress);
	virtual sp<EConnectFuture> connect(EInetSocketAddress* remoteAddress,
			EIoSessionInitializer* sessionInitializer);
	virtual sp<EConnectFuture> connect(EInetSocketAddress* remoteAddress,
			EInetSocketAddress* localAddress);
	virtual sp<EConnectFuture> connect(EInetSocketAddress* remoteAddress,
			EInetSocketAddress* localAddress,
			EIoSessionInitializer* sessionInitializer);

protected:
	EDefaultDatagramSessionConfig* ddsc;

	virtual void init() THROWS(EException);
	virtual void destroy() THROWS(EException);
	virtual sp<EDatagramChannel> newHandle(EInetSocketAddress* localAddress) THROWS(EException);
	virtual boolean connect(sp<EDatagramChannel> handle, EInetSocketAddress* remoteAddress) THROWS(EException);
	virtual sp<EIoSession> newSession(EIoProcessor* processor, sp<EDatagramChannel> handle) THROWS(EException);
	virtual void close(sp<EDatagramChannel> handle) THROWS(EException);
	virtual sp<ConnectionRequest> getConnectionRequest(sp<EDatagramChannel> handle);
	virtual boolean finishConnect(sp<EDatagramChannel> handle) THROWS(EException);
	virtual void register_(sp<EDatagramChannel> handle, sp<ConnectionRequest> request) THROWS(EException);
	virtual int select(int timeout) THROWS(EException);
	virtual void wakeup();
	virtual sp<EIterator<sp<EDatagramChannel>> > selectedHandles();
	virtual sp<EIterator<sp<EDatagramChannel>> > allHandles();
};

} /* namespace eio */
} /* namespace efc */
#endif /* ENIODATAGRAMCONNECTOR_HH_ */
