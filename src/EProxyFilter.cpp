/*
 * EProxyFilter.cpp
 *
 *  Created on: 2016-6-15
 *      Author: cxxjava@lotuseed.com
 */

#include "EProxyFilter.hh"
#include "EProxyLogicHandler.hh"
#include "ESocksProxyConstants.hh"
#include "ESocks4LogicHandler.hh"
#include "ESocks5LogicHandler.hh"
#include "EHttpSmartProxyHandler.hh"
#include "EProxyHandshakeIoBuffer.hh"

namespace efc {
namespace eio {

sp<ELogger> EProxyFilter::LOGGER = ELoggerManager::getLogger("EProxyFilter");

EProxyFilter::EProxyFilter() {
	// Do nothing
}

void EProxyFilter::onPreAdd(EIoFilterChain* chain, const char* name,
		EIoFilter::NextFilter* nextFilter) {
	if (chain->contains("EProxyFilter")) {
		throw EIllegalStateException(__FILE__, __LINE__, "A filter chain cannot contain more than one ProxyFilter.");
	}
}

void EProxyFilter::onPreRemove(EIoFilterChain* chain, const char* name,
		EIoFilter::NextFilter* nextFilter) {
	sp<EIoSession> session = chain->getSession();
	session->removeAttribute(EProxyIoSession::PROXY_SESSION.get());
}

void EProxyFilter::sessionCreated(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session) {
	LOGGER->debug__(__FILE__, __LINE__, "Session created: %s", session->toString().c_str());
	sp<EProxyIoSession> proxyIoSession = dynamic_pointer_cast<EProxyIoSession>(session->getAttribute(EProxyIoSession::PROXY_SESSION.get()));
	LOGGER->debug__(__FILE__, __LINE__, "  get proxyIoSession: %s", proxyIoSession->toString().c_str());
	proxyIoSession->setProxyFilter(this); //?

	// Create a HTTP proxy handler and start handshake.
	EProxyLogicHandler* handler = proxyIoSession->getHandler();

	// This test prevents from loosing handler conversationnal state when
	// reconnection occurs during an http handshake.
	if (handler == null) {
		sp<EProxyRequest> request = proxyIoSession->getRequest();

		sp<ESocksProxyRequest> req = dynamic_pointer_cast<ESocksProxyRequest>(request);
		if (req != null) {
			if (req->getProtocolVersion() == ESocksProxyConstants::SOCKS_VERSION_4) {
				handler = new ESocks4LogicHandler(proxyIoSession);
			} else {
				handler = new ESocks5LogicHandler(proxyIoSession);
			}
		} else {
			handler = new EHttpSmartProxyHandler(proxyIoSession);
		}

		//TODO: need to free handle!!!

		proxyIoSession->setHandler(handler);
		handler->doHandshake(nextFilter);
	}

	proxyIoSession->getEventQueue()->enqueueEventIfNecessary(
			new EIoSessionEvent(nextFilter, session, EIoSessionEventType::CREATED));
}

void EProxyFilter::sessionOpened(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session) {
	sp<EProxyIoSession> proxyIoSession = dynamic_pointer_cast<EProxyIoSession>(session->getAttribute(EProxyIoSession::PROXY_SESSION.get()));
	proxyIoSession->getEventQueue()->enqueueEventIfNecessary(
			new EIoSessionEvent(nextFilter, session, EIoSessionEventType::OPENED)); //TODO: if enqueue fail then need free it!
}

void EProxyFilter::sessionClosed(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session) {
	sp<EProxyIoSession> proxyIoSession = dynamic_pointer_cast<EProxyIoSession>(session->getAttribute(EProxyIoSession::PROXY_SESSION.get()));
	proxyIoSession->getEventQueue()->enqueueEventIfNecessary(
			new EIoSessionEvent(nextFilter, session, EIoSessionEventType::CLOSED));
}

void EProxyFilter::sessionIdle(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, EIdleStatus status) {
	sp<EProxyIoSession> proxyIoSession = dynamic_pointer_cast<EProxyIoSession>(session->getAttribute(EProxyIoSession::PROXY_SESSION.get()));
	proxyIoSession->getEventQueue()->enqueueEventIfNecessary(new EIoSessionEvent(nextFilter, session, status));
}

void EProxyFilter::exceptionCaught(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, sp<EThrowableType>& cause) {
	sp<EProxyIoSession> proxyIoSession = dynamic_pointer_cast<EProxyIoSession>(session->getAttribute(EProxyIoSession::PROXY_SESSION.get()));
	proxyIoSession->setAuthenticationFailed(true);
	EIoFilterAdapter::exceptionCaught(nextFilter, session, cause);
}

void EProxyFilter::messageReceived(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, sp<EObject>& message) {
	EProxyLogicHandler* handler = getProxyHandler(session);

	SYNCHRONIZED(handler) {
		sp<EIoBuffer> buf = dynamic_pointer_cast<EIoBuffer>(message);

		if (handler->isHandshakeComplete()) {
			// Handshake done - pass data on as-is
			sp<EObject> obj = dynamic_pointer_cast<EObject>(buf);
			nextFilter->messageReceived(session, obj);

		} else {
			LOGGER->debug__(__FILE__, __LINE__, " Data Read: %s (%s)", handler->toString().c_str(), buf->toString().c_str());

			// Keep sending handshake data to the handler until we run out
			// of data or the handshake is finished
			while (buf->hasRemaining() && !handler->isHandshakeComplete()) {
				LOGGER->debug(__FILE__, __LINE__, " Pre-handshake - passing to handler");

				int pos = buf->position();
				handler->messageReceived(nextFilter, buf);

				// Data not consumed or session closing
				if (buf->position() == pos || session->isClosing()) {
					return;
				}
			}

			// Pass on any remaining data to the next filter
			if (buf->hasRemaining()) {
				LOGGER->debug(__FILE__, __LINE__, " Passing remaining data to next filter");

				sp<EObject> obj = dynamic_pointer_cast<EObject>(buf);
				nextFilter->messageReceived(session, obj);
			}
		}
    }}
}

void EProxyFilter::messageSent(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) {
	sp<EObject> message = writeRequest->getMessage();
	if (message != null && dynamic_pointer_cast<EProxyHandshakeIoBuffer>(message) != null) {
		// Ignore buffers used in handshaking
		return;
	}

	nextFilter->messageSent(session, writeRequest);
}

void EProxyFilter::filterWrite(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, sp<EWriteRequest>& writeRequest) {
	writeData(nextFilter, session, writeRequest, false);
}

void EProxyFilter::writeData(EIoFilter::NextFilter* nextFilter,
		sp<EIoSession>& session, sp<EWriteRequest>& writeRequest,
		boolean isHandshakeData) {
	EProxyLogicHandler* handler = getProxyHandler(session);

	SYNCHRONIZED(handler) {
		if (handler->isHandshakeComplete()) {
			// Handshake is done - write data as normal
			nextFilter->filterWrite(session, writeRequest);
		} else if (isHandshakeData) {
			LOGGER->debug__(__FILE__, __LINE__, "   handshake data: %s", writeRequest->getMessage()->toString().c_str());

			// Writing handshake data
			nextFilter->filterWrite(session, writeRequest);
		} else {
			// Writing non-handshake data before the handshake finished
			if (!session->isConnected()) {
				// Not even connected - ignore
				LOGGER->debug(__FILE__, __LINE__, " Write request on closed session. Request ignored.");
			} else {
				// Queue the data to be sent as soon as the handshake completes
				LOGGER->debug(__FILE__, __LINE__, " Handshaking is not complete yet. Buffering write request.");
				handler->enqueueWriteRequest(nextFilter, writeRequest);
			}
		}
    }}
}

EProxyLogicHandler* EProxyFilter::getProxyHandler(sp<EIoSession>& session) {
	EProxyLogicHandler* handler = dynamic_pointer_cast<EProxyIoSession>(session->getAttribute(EProxyIoSession::PROXY_SESSION.get()))->getHandler();

	if (handler == null) {
		throw EIllegalStateException(__FILE__, __LINE__);
	}

	// Sanity check
	if (handler->getProxyIoSession()->getProxyFilter() != this) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "Not managed by this filter.");
	}

	return handler;
}

} /* namespace eio */
} /* namespace efc */
