/*
 * EIoSessionAttributeMap.hh
 *
 *  Created on: 2015-12-31
 *      Author: cxxjava@163.com
 */

#ifndef EIOSESSIONATTRIBUTEMAP_HH_
#define EIOSESSIONATTRIBUTEMAP_HH_

#include "./EIoSession.hh"

namespace efc {
namespace eio {

/**
 * Stores the user-defined attributes which is provided per {@link IoSession}.
 * All user-defined attribute accesses in {@link IoSession} are forwarded to
 * the instance of {@link IoSessionAttributeMap}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

interface EIoSessionAttributeMap : virtual public EObject {
	virtual ~EIoSessionAttributeMap() {
	}

	/**
	 * Returns the value of user defined attribute associated with the
	 * specified key.  If there's no such attribute, the specified default
	 * value is associated with the specified key, and the default value is
	 * returned.  This method is same with the following code except that the
	 * operation is performed atomically.
	 * <pre>
	 * if (containsAttribute(key)) {
	 *     return getAttribute(key);
	 * } else {
	 *     setAttribute(key, defaultValue);
	 *     return defaultValue;
	 * }
	 * </pre>
	 *
	 * @param session the session for which we want to get an attribute
     * @param key The key we are looking for
     * @param defaultValue The default returned value if the attribute is not found
	 */
	virtual sp<EObject> getAttribute(EIoSession* session, EObject* key, EObject* defaultValue) = 0;

	/**
	 * Sets a user-defined attribute.
	 *
	 * @param session the session for which we want to set an attribute
	 * @param key the key of the attribute
	 * @param value the value of the attribute
	 * @return The old value of the attribute.  <tt>null</tt> if it is new.
	     */
	virtual sp<EObject> setAttribute(EIoSession* session, sp<EObject> key, sp<EObject> value) = 0;

	/**
	 * Sets a user defined attribute if the attribute with the specified key
	 * is not set yet.  This method is same with the following code except
	 * that the operation is performed atomically.
	 * <pre>
	 * if (containsAttribute(key)) {
	 *     return getAttribute(key);
	 * } else {
	 *     return setAttribute(key, value);
	 * }
	 * </pre>
	 *
	 * @param session the session for which we want to set an attribute
     * @param key The key we are looking for
     * @param value The value to inject
     * @return The previous attribute
	 */
	virtual sp<EObject> setAttributeIfAbsent(EIoSession* session, sp<EObject> key, sp<EObject> value) = 0;

	/**
	 * Removes a user-defined attribute with the specified key.
	 *
	 * @return The old value of the attribute.  <tt>null</tt> if not found.
	 * @param session the session for which we want to remove an attribute
     * @param key The key we are looking for
	 */
	virtual sp<EObject> removeAttribute(EIoSession* session, EObject* key) = 0;

	/**
	 * Removes a user defined attribute with the specified key if the current
	 * attribute value is equal to the specified value.  This method is same
	 * with the following code except that the operation is performed
	 * atomically.
	 * <pre>
	 * if (containsAttribute(key) && getAttribute(key).equals(value)) {
	 *     removeAttribute(key);
	 *     return true;
	 * } else {
	 *     return false;
	 * }
	 * </pre>
	 *
	 * @param session the session for which we want to remove a value
     * @param key The key we are looking for
     * @param value The value to remove
     * @return <tt>true</tt> if the value has been removed, <tt>false</tt> if the key was
     * not found of the value not removed
	 */
	virtual boolean removeAttribute(EIoSession* session, EObject* key, EObject* value) = 0;

	/**
	 * Replaces a user defined attribute with the specified key if the
	 * value of the attribute is equals to the specified old value.
	 * This method is same with the following code except that the operation
	 * is performed atomically.
	 * <pre>
	 * if (containsAttribute(key) && getAttribute(key).equals(oldValue)) {
	 *     setAttribute(key, newValue);
	 *     return true;
	 * } else {
	 *     return false;
	 * }
	 * </pre>
	 *
	 * @param session the session for which we want to replace an attribute
     * @param key The key we are looking for
     * @param oldValue The old value to replace
     * @param newValue The new value to set
     * @return <tt>true</tt> if the value has been replaced, <tt>false</tt> if the key was
     * not found of the value not replaced
	 */
	virtual boolean replaceAttribute(EIoSession* session, EObject* key, EObject* oldValue, sp<EObject> newValue) = 0;

	/**
	 * @return <tt>true</tt> if this session contains the attribute with
	 * the specified <tt>key</tt>.
	 *
	 * @param session the session for which wa want to check if an attribute is present
	 * @param key The key we are looking for
	 */
	virtual boolean containsAttribute(EIoSession* session, EObject* key) = 0;

	/**
	 * @return the set of keys of all user-defined attributes.
	 *
	 * @param session the session for which we want the set of attributes
	 */
	virtual ESet<sp<EObject> >* getAttributeKeys(EIoSession* session) = 0;

	/**
	 * Disposes any releases associated with the specified session.
	 * This method is invoked on disconnection.
	 *
	 * @param session the session to be disposed
     * @throws Exception If the session can't be disposed
	 */
	virtual void dispose(sp<EIoSession> session) THROWS(EException) = 0;
};

} /* namespace eio */
} /* namespace efc */

#endif /* EIOSESSIONATTRIBUTEMAP_HH_ */
