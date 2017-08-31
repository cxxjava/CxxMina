/*
 * EProtocolCodecFactory.hh
 *
 *  Created on: 2016-5-20
 *      Author: cxxjava@163.com
 */

#ifndef EPROTOCOLCODECFACTORY_HH_
#define EPROTOCOLCODECFACTORY_HH_

#include "./EIoSession.hh"
#include "./EProtocolEncoder.hh"
#include "./EProtocolDecoder.hh"

namespace efc {
namespace eio {

/**
 * Provides {@link ProtocolEncoder} and {@link ProtocolDecoder} which translates
 * binary or protocol specific data into message object and vice versa.
 * <p>
 * Please refer to
 * <a href="../../../../../xref-examples/org/apache/mina/examples/reverser/ReverseProtocolProvider.html"><code>ReverserProtocolProvider</code></a>
 * example.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

interface EProtocolCodecFactory : virtual public EObject {
	virtual ~EProtocolCodecFactory(){}

	/**
	 * Returns a new (or reusable) instance of {@link ProtocolEncoder} which
	 * encodes message objects into binary or protocol-specific data.
	 *
	 * @param session The current session
	 * @return The encoder instance
	 * @throws Exception If an error occurred while retrieving the encoder
	 */
	virtual EProtocolEncoder* getEncoder(sp<EIoSession>& session) THROWS(EException) = 0;

	/**
	 * Returns a new (or reusable) instance of {@link ProtocolDecoder} which
	 * decodes binary or protocol-specific data into message objects.
	 *
	 * @param session The current session
	 * @return The decoder instance
	 * @throws Exception If an error occurred while retrieving the decoder
	 */
	virtual EProtocolDecoder* getDecoder(sp<EIoSession>& session) THROWS(EException) = 0;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EPROTOCOLCODECFACTORY_HH_ */
