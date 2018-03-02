/*
 * EAttributeKey.hh
 *
 *  Created on: 2016-3-3
 *      Author: cxxjava@163.com
 */

#ifndef EATTRIBUTEKEY_HH_
#define EATTRIBUTEKEY_HH_

#include "Efc.hh"

namespace efc {
namespace eio {

/**
 * Creates a Key from a class name and an attribute name. The resulting Key will
 * be stored in the session Map.<br>
 * For instance, we can create a 'processor' AttributeKey this way :
 *
 * <pre>
 * private static final AttributeKey PROCESSOR = new AttributeKey(
 * 	SimpleIoProcessorPool.class, &quot;processor&quot;);
 * </pre>
 *
 * This will create the <b>SimpleIoProcessorPool.processor@7DE45C99</b> key
 * which will be stored in the session map.<br>
 * Such an attributeKey is mainly useful for debug purposes.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EAttributeKey: public EObject {
public:
	/**
	 * Creates a new instance. It's built from :
	 * <ul>
	 * <li>the class' name</li>
	 * <li>the attribute's name</li>
	 * <li>this attribute hashCode</li>
	 * </ul>
	 *
	 * @param source The class this AttributeKey will be attached to
	 * @param name The Attribute name
	 */
	EAttributeKey(const char* source, const char* name);

	/**
	 * The String representation of this object.
	 */
	virtual EString toString();

	virtual int hashCode();

	virtual boolean equals(EObject* obj);
	boolean equals(EAttributeKey* obj);

private:
	/** The attribute's name */
	EString name;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EATTRIBUTEKEY_HH_ */
