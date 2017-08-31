/*
 * EDefaultTransportMetadata.hh
 *
 *  Created on: 2016-3-14
 *      Author: cxxjava@163.com
 */

#ifndef EDEFAULTTRANSPORTMETADATA_HH_
#define EDEFAULTTRANSPORTMETADATA_HH_

#include "./ETransportMetadata.hh"

namespace efc {
namespace eio {

/**
 * A default immutable implementation of {@link TransportMetadata}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EDefaultTransportMetadata: public ETransportMetadata {
public:
	EDefaultTransportMetadata(const char* providerName, const char* name,
			boolean connectionless, boolean fragmentation);

	virtual const char* getProviderName();
	virtual const char* getName();
	virtual boolean isConnectionless();
	virtual boolean hasFragmentation();

private:
	EString providerName;
	EString name;
	boolean connectionless;
    /** The flag indicating that the transport support fragmentation or not */
	boolean fragmentation;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EDEFAULTTRANSPORTMETADATA_HH_ */
