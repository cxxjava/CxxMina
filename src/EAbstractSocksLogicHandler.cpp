/*
 * EAbstractSocksLogicHandler.cpp
 *
 *  Created on: 2016-7-6
 *      Author: cxxjava@lotuseed.com
 */

#include "EAbstractSocksLogicHandler.hh"

namespace efc {
namespace eio {

EAbstractSocksLogicHandler::EAbstractSocksLogicHandler(sp<EProxyIoSession>& proxyIoSession) :
		EAbstractProxyLogicHandler(proxyIoSession) {
	this->request = dynamic_pointer_cast<ESocksProxyRequest>(proxyIoSession->getRequest());
}

} /* namespace eio */
} /* namespace efc */
