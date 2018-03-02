/*
 * EDefaultIoFilterChainBuilder.hh
 *
 *  Created on: 2015-12-28
 *      Author: cxxjava@163.com
 */

#ifndef EDEFAULTIOFILTERCHAINBUILDER_HH_
#define EDEFAULTIOFILTERCHAINBUILDER_HH_

#include "./EIoFilterChainBuilder.hh"
#include "ELog.hh"

namespace efc {
namespace eio {

/**
 * The default implementation of {@link IoFilterChainBuilder} which is useful
 * in most cases.  {@link DefaultIoFilterChainBuilder} has an identical interface
 * with {@link IoFilter}; it contains a list of {@link IoFilter}s that you can
 * modify. The {@link IoFilter}s which are added to this builder will be appended
 * to the {@link IoFilterChain} when {@link #buildFilterChain(IoFilterChain)} is
 * invoked.
 * <p>
 * However, the identical interface doesn't mean that it behaves in an exactly
 * same way with {@link IoFilterChain}.  {@link DefaultIoFilterChainBuilder}
 * doesn't manage the life cycle of the {@link IoFilter}s at all, and the
 * existing {@link IoSession}s won't get affected by the changes in this builder.
 * {@link IoFilterChainBuilder}s affect only newly created {@link IoSession}s.
 *
 * <pre>
 * IoAcceptor acceptor = ...;
 * DefaultIoFilterChainBuilder builder = acceptor.getFilterChain();
 * builder.addLast( "myFilter", new MyFilter() );
 * ...
 * </pre>
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @org.apache.xbean.XBean
 */

class EDefaultIoFilterChainBuilder: public EIoFilterChainBuilder, public ESynchronizeable {
public:
	virtual ~EDefaultIoFilterChainBuilder();

	/**
	 * Creates a new instance with an empty filter list.
	 */
	EDefaultIoFilterChainBuilder();

	/**
	 * Creates a new copy of the specified {@link DefaultIoFilterChainBuilder}.
	 *
	 * @param filterChain The FilterChain we will copy
	 */
	EDefaultIoFilterChainBuilder(EDefaultIoFilterChainBuilder* filterChain);

	/**
	 * @see IoFilterChain#getEntry(String)
	 *
	 * @param name The Filter's name we are looking for
     * @return The found Entry
	 */
	sp<EIoFilterChain::Entry> getEntry(const char* name);

	/**
	 * @see IoFilterChain#getEntry(IoFilter)
	 *
	 * @param filter The Filter we are looking for
     * @return The found Entry
	 */
	sp<EIoFilterChain::Entry> getEntry(EIoFilter* filter);

	/**
	 * @see IoFilterChain#get(String)
	 *
	 * @param name The Filter's name we are looking for
     * @return The found Filter, or null
	 */
	EIoFilter* get(const char* name);

	/**
	 * @see IoFilterChain#contains(String)
	 *
	 * @param name The Filter's name we want to check if it's in the chain
     * @return <tt>true</tt> if the chain contains the given filter name
	 */
	boolean contains(const char* name);

	/**
	 * @see IoFilterChain#contains(IoFilter)
	 *
	 * @param filter The Filter we want to check if it's in the chain
     * @return <tt>true</tt> if the chain contains the given filter
	 */
	boolean contains(EIoFilter* filter);

	/**
	 * @see IoFilterChain#addFirst(String, IoFilter)
	 *
	 * @param name The filter's name
     * @param filter The filter to add
	 */
	synchronized void addFirst(const char* name, EIoFilter* filter);

	/**
	 * @see IoFilterChain#addLast(String, IoFilter)
	 *
	 * @param name The filter's name
     * @param filter The filter to add
	 */
	synchronized void addLast(const char* name, EIoFilter* filter);

	 /**
	 * @see IoFilterChain#addBefore(String, String, IoFilter)
	 *
	 * @param baseName The filter baseName
     * @param name The filter's name
     * @param filter The filter to add
	 */
	synchronized void addBefore(const char* baseName, const char* name, EIoFilter* filter);

	/**
	 * @see IoFilterChain#addAfter(String, String, IoFilter)
	 *
	 * @param baseName The filter baseName
     * @param name The filter's name
     * @param filter The filter to add
	 */
	synchronized void addAfter(const char* baseName, const char* name, EIoFilter* filter);

	/**
	 * @see IoFilterChain#remove(String)
	 *
	 * @param name The Filter's name to remove from the list of Filters
     * @return The removed IoFilter
	 */
	synchronized EIoFilter* remove(const char* name);

	/**
	 * @see IoFilterChain#remove(IoFilter)
	 *
	 * @param filter The Filter we want to remove from the list of Filters
     * @return The removed IoFilter
	 */
	synchronized EIoFilter* remove(EIoFilter* filter);

	synchronized EIoFilter* replace(const char* name, EIoFilter* newFilter);

	synchronized void replace(EIoFilter* oldFilter, EIoFilter* newFilter);

	/**
	 * @see IoFilterChain#clear()
	 */
	synchronized void clear();

	/**
	 * @see IoFilterChain#buildFilterChain(EIoFilterChain)
	 */
	void buildFilterChain(EIoFilterChain* chain) THROWS(EException);

	virtual void destroy() THROWS(EException);

	virtual EString toString();

private:
	EList<sp<EIoFilterChain::Entry> >* entries;

	static sp<ELogger> LOGGER;// = LoggerFactory.getLogger(DefaultIoFilterChainBuilder.class);

	void register_(int index, EIoFilterChain::Entry* e);
	void checkBaseName(const char* baseName);
};

} /* namespace eio */
} /* namespace efc */
#endif /* EDEFAULTIOFILTERCHAINBUILDER_HH_ */
