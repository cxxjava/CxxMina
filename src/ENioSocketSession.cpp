/*
 * ENioSocketSession.cpp
 *
 *  Created on: 2016-1-19
 *      Author: cxxjava@163.com
 */

#include "../inc/ENioSocketSession.hh"
#include "../inc/EAbstractSocketSessionConfig.hh"
#include "../inc/EIoService.hh"
#include "../inc/EIoFilter.hh"
#include "../inc/EIoFilterChain.hh"
#include "../inc/EDefaultTransportMetadata.hh"
#include "../inc/ERuntimeIoException.hh"

namespace efc {
namespace eio {

class SessionConfigImpl : public EAbstractSocketSessionConfig {
private:
	ENioSocketSession* session;

public:
	SessionConfigImpl(ENioSocketSession* nss) : session(nss) {
	}
	boolean isKeepAlive() {
		try {
			return session->getSocket()->getKeepAlive();
		} catch (ESocketException& e) {
			throw ERuntimeIoException(__FILE__, __LINE__, e.toString().c_str());
		}
	}

	void setKeepAlive(boolean on) {
		try {
			session->getSocket()->setKeepAlive(on);
		} catch (ESocketException& e) {
			throw ERuntimeIoException(__FILE__, __LINE__, e.toString().c_str());
		}
	}

	boolean isOobInline() {
		try {
			return session->getSocket()->getOOBInline();
		} catch (ESocketException& e) {
			throw ERuntimeIoException(__FILE__, __LINE__, e.toString().c_str());
		}
	}

	void setOobInline(boolean on) {
		try {
			session->getSocket()->setOOBInline(on);
		} catch (ESocketException& e) {
			throw ERuntimeIoException(__FILE__, __LINE__, e.toString().c_str());
		}
	}

	boolean isReuseAddress() {
		try {
			return session->getSocket()->getReuseAddress();
		} catch (ESocketException& e) {
			throw ERuntimeIoException(__FILE__, __LINE__, e.toString().c_str());
		}
	}

	void setReuseAddress(boolean on) {
		try {
			session->getSocket()->setReuseAddress(on);
		} catch (ESocketException& e) {
			throw ERuntimeIoException(__FILE__, __LINE__, e.toString().c_str());
		}
	}

	int getSoLinger() {
		try {
			return session->getSocket()->getSoLinger();
		} catch (ESocketException& e) {
			throw ERuntimeIoException(__FILE__, __LINE__, e.toString().c_str());
		}
	}

	void setSoLinger(int linger) {
		try {
			if (linger < 0) {
				session->getSocket()->setSoLinger(false, 0);
			} else {
				session->getSocket()->setSoLinger(true, linger);
			}
		} catch (ESocketException& e) {
			throw ERuntimeIoException(__FILE__, __LINE__, e.toString().c_str());
		}
	}

	boolean isTcpNoDelay() {
		if (!session->isConnected()) {
			return false;
		}

		try {
			return session->getSocket()->getTcpNoDelay();
		} catch (ESocketException& e) {
			throw ERuntimeIoException(__FILE__, __LINE__, e.toString().c_str());
		}
	}

	void setTcpNoDelay(boolean on) {
		try {
			session->getSocket()->setTcpNoDelay(on);
		} catch (ESocketException& e) {
			throw ERuntimeIoException(__FILE__, __LINE__, e.toString().c_str());
		}
	}

	/**
	 * {@inheritDoc}
	 */
	int getTrafficClass() {
		try {
			return session->getSocket()->getTrafficClass();
		} catch (ESocketException& e) {
			throw ERuntimeIoException(__FILE__, __LINE__, e.toString().c_str());
		}
	}

	/**
	 * {@inheritDoc}
	 */
	void setTrafficClass(int tc) {
		try {
			session->getSocket()->setTrafficClass(tc);
		} catch (ESocketException& e) {
			throw ERuntimeIoException(__FILE__, __LINE__, e.toString().c_str());
		}
	}

	int getSendBufferSize() {
		try {
			return session->getSocket()->getSendBufferSize();
		} catch (ESocketException& e) {
			throw ERuntimeIoException(__FILE__, __LINE__, e.toString().c_str());
		}
	}

	void setSendBufferSize(int size) {
		try {
			session->getSocket()->setSendBufferSize(size);
		} catch (ESocketException& e) {
			throw ERuntimeIoException(__FILE__, __LINE__, e.toString().c_str());
		}
	}

	int getReceiveBufferSize() {
		try {
			return session->getSocket()->getReceiveBufferSize();
		} catch (ESocketException& e) {
			throw ERuntimeIoException(__FILE__, __LINE__, e.toString().c_str());
		}
	}

	void setReceiveBufferSize(int size) {
		try {
			session->getSocket()->setReceiveBufferSize(size);
		} catch (ESocketException& e) {
			throw ERuntimeIoException(__FILE__, __LINE__, e.toString().c_str());
		}
	}
};

ENioSocketSession::~ENioSocketSession() {
	delete config;
}

ENioSocketSession::ENioSocketSession(EIoService* service,
		EIoProcessor* processor, sp<ESocketChannel> channel) :
		ENioSession(processor, service, channel) {
	config = new SessionConfigImpl(this);
	this->config->setAll(reinterpret_cast<ESocketSessionConfig*>(service->getSessionConfig()));
}

ETransportMetadata* ENioSocketSession::METADATA() {
	static EDefaultTransportMetadata metadata("nio", "socket", false, true);
	return &metadata;
}

ETransportMetadata* ENioSocketSession::getTransportMetadata() {
	return METADATA();
}

ESocketSessionConfig* ENioSocketSession::getConfig() {
	return dynamic_cast<ESocketSessionConfig*>(config);
}

sp<EByteChannel> ENioSocketSession::getChannel() {
	return channel;
}

EInetSocketAddress* ENioSocketSession::getRemoteAddress() {
	if (channel == null) {
		return null;
	}

	ESocket* socket = getSocket();

	if (socket == null) {
		return null;
	}

	return dynamic_cast<EInetSocketAddress*>(socket->getRemoteSocketAddress());
}

EInetSocketAddress* ENioSocketSession::getLocalAddress() {
	if (channel == null) {
		return null;
	}

	ESocket* socket = getSocket();

	if (socket == null) {
		return null;
	}

	return dynamic_cast<EInetSocketAddress*>(socket->getLocalSocketAddress());
}

EInetSocketAddress* ENioSocketSession::getServiceAddress() {
	return dynamic_cast<EInetSocketAddress*>(ENioSession::getServiceAddress());
}

boolean ENioSocketSession::isSecured() {
	// If the session does not have a SslFilter, we can return false
	EIoFilterChain* chain = getFilterChain();

	EIoFilter* sslFilter = chain->get("SslFilter");

	if (sslFilter != null) {
		// Get the SslHandler from the SslFilter

		//TODO...
		//return ((SslFilter)sslFilter).isSslStarted(this);
		return false;
	} else {
		return false;
	}
}

ESocket* ENioSocketSession::getSocket() {
	return (dynamic_pointer_cast<ESocketChannel>(channel))->socket();
}

} /* namespace eio */
} /* namespace efc */
