/*
 * EIoFilterChainBuilder.hh
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#ifndef EIOFILTERCHAINBUILDER_HH_
#define EIOFILTERCHAINBUILDER_HH_

#include "./EIoFilterChain.hh"
#include "Efc.hh"

namespace efc {
namespace eio {

/**
 * An interface that builds {@link IoFilterChain} in predefined way
 * when {@link IoSession} is created.  You can extract common filter chain
 * modification logic to this interface.  For example, to add a filter
 * to the chain,
 * <pre>
 * public class MyFilterChainBuilder implements IoFilterChainBuilder {
 *     public void buildFilterChain( IoFilterChain chain ) throws Exception {
 *         chain.addLast( "myFilter", new MyFilter() );
 *     }
 * }
 * </pre>
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

interface EIoFilterChainBuilder {
	virtual ~EIoFilterChainBuilder(){
	}

    /**
	 * Modifies the specified <tt>chain</tt>.
	 *
	 * @param chain The chain to modify
	 * @throws Exception If the chain modification failed
	 */
    virtual void buildFilterChain(EIoFilterChain* chain) THROWS(EException) = 0;

    /**
	 * Shuts down the underlying executor if this filter hase been created via
	 * a convenience constructor.
	 */
	virtual void destroy() THROWS(EException) = 0;
};

/**
 * An implementation which does nothing.
 */
class DummyIoFilterChainBuilder: virtual public EIoFilterChainBuilder {
public:
	DummyIoFilterChainBuilder(EIoFilterChain* chain) {
    }

    virtual EString toString() {
        return "NOOP";
    }
};

}
} // namespace efc::eio

#endif /* EIOFILTERCHAINBUILDER_HH_ */
