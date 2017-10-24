/*
 * EIoServiceListener.hh
 *
 *  Created on: 2013-8-23
 *      Author: cxxjava@163.com
 */

#ifndef EIOSERVICELISTENER_HH_
#define EIOSERVICELISTENER_HH_

#include "Efc.hh"
#include "./EIdleStatus.hh"
#include "./EIoSession.hh"

namespace efc {
namespace eio {

class EIoService;

/**
 * Listens to events related to an {@link IoService}.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */
interface EIoServiceListener : virtual public EObject {
	virtual ~EIoServiceListener() {
	}

	/**
	 * Invoked when a new service is activated by an {@link IoService}.
	 *
	 * @param service the {@link IoService}
	 * @throws Exception if an error occurred while the service is being activated
	 */
	virtual void serviceActivated(EIoService* service) THROWS(EException) = 0;

	/**
	 * Invoked when a service is idle.
	 *
	 * @param service the {@link IoService}
     * @param idleStatus The idle status
     * @throws Exception if an error occurred while the service is being idled
	 */
	virtual void serviceIdle(EIoService* service, EIdleStatus idleStatus) THROWS(EException) = 0;

	/**
	 * Invoked when a service is deactivated by an {@link IoService}.
	 *
	 * @param service the {@link IoService}
	 * @throws Exception if an error occurred while the service is being deactivated
	 */
	virtual void serviceDeactivated(EIoService* service) THROWS(EException) = 0;

	/**
	 * Invoked when a new session is created by an {@link IoService}.
	 *
	 * @param session the new session
	 * @throws Exception if an error occurred while the session is being created
	 */
	virtual void sessionCreated(sp<EIoSession>& session) THROWS(EException) = 0;

	/**
	 * Invoked when a new session is closed by an {@link IoService}.
	 *
	 * @param session the new session
	 * @throws Exception if an error occurred while the session is being closed
	 */
	virtual void sessionClosed(sp<EIoSession>& session) THROWS(EException) = 0;

	/**
	 * Invoked when a session is being destroyed by an {@link IoService}.
	 *
	 * @param session the session to be destroyed
	 */
	virtual void sessionDestroyed(sp<EIoSession>& session) THROWS(EException) = 0;
};

} /* namespace eio */
} /* namespace efc */

#endif /* EIOSERVICELISTENER_HH_ */
