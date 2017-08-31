/*
 * EChainedIoHandler.cpp
 *
 *  Created on: 2016-5-31
 *      Author: cxxjava@163.com
 */

#include "../inc/EChainedIoHandler.hh"

namespace efc {
namespace eio {

EChainedIoHandler::~EChainedIoHandler() {
	delete chain;
}

EChainedIoHandler::EChainedIoHandler() {
	chain = new EIoHandlerChain();
}

EIoHandlerChain* EChainedIoHandler::getChain() {
	return chain;
}

void EChainedIoHandler::messageReceived(sp<EIoSession>& session,
		sp<EObject>& message) {
	sp<EIoHandlerCommand::NextCommand> next(null);
	chain->execute(next, session, message);
}

} /* namespace eio */
} /* namespace efc */
