/*
 * EIoSessionDataStructureFactory.hh
 *
 *  Created on: 2015-12-31
 *      Author: cxxjava@163.com
 */

#ifndef EIOSESSIONDATASTRUCTUREFACTORY_HH_
#define EIOSESSIONDATASTRUCTUREFACTORY_HH_

#include "./EIoSession.hh"
#include "./EIoSessionAttributeMap.hh"

namespace efc {
namespace eio {

/**
 * Provides data structures to a newly created session.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

interface EIoSessionDataStructureFactory : virtual public EObject {
	virtual ~EIoSessionDataStructureFactory() {
	}

	/**
	 * Returns an {@link IoSessionAttributeMap} which is going to be associated
	 * with the specified <tt>session</tt>.  Please note that the returned
	 * implementation must be thread-safe.
	 *
	 * @param session The session for which we want the Attribute Map
     * @throws Exception If an error occured while retrieving the map
	 */
	virtual EIoSessionAttributeMap* getAttributeMap(EIoSession* session) THROWS(EException) = 0;

	/**
	 * Returns an {@link WriteRequest} which is going to be associated with
	 * the specified <tt>session</tt>.  Please note that the returned
	 * implementation must be thread-safe and robust enough to deal
	 * with various messages types (even what you didn't expect at all),
	 * especially when you are going to implement a priority queue which
	 * involves {@link Comparator}.
	 *
	 * @param session The session for which we want the WriteRequest queue
     * @throws Exception If an error occured while retrieving the queue
	 */
	virtual EWriteRequestQueue* getWriteRequestQueue(EIoSession* session) THROWS(EException) = 0;
};

} /* namespace eio */
} /* namespace efc */

#endif /* EIOSESSIONDATASTRUCTUREFACTORY_HH_ */
