/*
 * EDummySession.hh
 *
 *  Created on: 2016-9-23
 *      Author: cxxjava@163.com
 */

#ifndef EDUMMYSESSION_HH_
#define EDUMMYSESSION_HH_

#include "./EAbstractIoSession.hh"

namespace efc {
namespace eio {

/**
 * A dummy {@link IoSession} for unit-testing or non-network-use of
 * the classes that depends on {@link IoSession}.
 *
 * <h2>Overriding I/O request methods</h2>
 * All I/O request methods (i.e. {@link #close()}, {@link #write(Object)}
 * are final and therefore cannot be
 * overridden, but you can always add your custom {@link IoFilter} to the
 * {@link IoFilterChain} to intercept any I/O events and requests.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EDummySession: public EAbstractIoSession {
public:
	DECLARE_STATIC_INITZZ;

public:
	virtual ~EDummySession();

	/**
	 * Creates a new instance.
	 */
	EDummySession();

	/**
	 * Sets the configuration of this session.
	 *
	 * @param config the {@link IoSessionConfig} to set
	 */
	void setConfig(EIoSessionConfig* config);

	/**
	 * Sets the {@link IoHandler} which handles this session.
	 *
	 * @param handler the {@link IoHandler} to set
	 */
	void setHandler(EIoHandler* handler);

	/**
	 * Sets the socket address of local machine which is associated with
	 * this session.
	 *
	 * @param localAddress The socket address to set
	 */
	void setLocalAddress(EInetSocketAddress* localAddress);

	/**
	 * Sets the socket address of remote peer.
	 *
	 * @param remoteAddress The socket address to set
	 */
	void setRemoteAddress(EInetSocketAddress* remoteAddress);

	/**
	 * Sets the {@link IoService} which provides I/O service to this session.
	 *
	 * @param service The {@link IoService} to set
	 */
	void setService(EIoService* service);

	/**
	 * Sets the {@link TransportMetadata} that this session runs on.
	 *
	 * @param transportMetadata The {@link TransportMetadata} to set
	 */
	void setTransportMetadata(ETransportMetadata* transportMetadata);

	/**
	 * Update all statistical properties related with throughput.  By default
	 * this method returns silently without updating the throughput properties
	 * if they were calculated already within last
	 * {@link IoSessionConfig#getThroughputCalculationInterval() calculation interval}.
	 * If, however, <tt>force</tt> is specified as <tt>true</tt>, this method
	 * updates the throughput properties immediately.
	 *
	 * @param force the flag that forces the update of properties immediately if <tt>true</tt>
	 */
	void updateThroughput(boolean force);

	/**
	 * {@inheritDoc}
	 */
	virtual void init();
	virtual EIoSessionConfig* getConfig();
	virtual EIoFilterChain* getFilterChain();
	virtual EIoHandler* getHandler();
	virtual EInetSocketAddress* getLocalAddress();
	virtual EInetSocketAddress* getRemoteAddress();
	virtual EIoService* getService();
	virtual EIoProcessor* getProcessor();
	virtual ETransportMetadata* getTransportMetadata();
	virtual void setScheduledWriteBytes(int byteCount);
	virtual void setScheduledWriteMessages(int messages);
	virtual boolean isDestroyed();
	virtual void destroy();

private:
	EIoService* volatile service;
	EIoSessionConfig* volatile config;
	EIoFilterChain* filterChain;
	EIoProcessor* processor;
	EIoHandler* volatile handler;// = new IoHandlerAdapter();
	EInetSocketAddress* volatile localAddress;// = ANONYMOUS_ADDRESS;
	EInetSocketAddress* volatile remoteAddress;// = ANONYMOUS_ADDRESS;
	ETransportMetadata* volatile transportMetadata;// = TRANSPORT_METADATA;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EDUMMYSESSION_HH_ */
