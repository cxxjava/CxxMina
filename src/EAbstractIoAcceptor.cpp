/*
 * EAbstractIoAcceptor.cpp
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#include "../inc/EAbstractIoAcceptor.hh"
#include "../inc/ERuntimeIoException.hh"

namespace efc {
namespace eio {

EAbstractIoAcceptor::~EAbstractIoAcceptor() {
	delete defaultLocalAddresses_;
	delete boundAddresses_;
}

EAbstractIoAcceptor::EAbstractIoAcceptor(EIoSessionConfig* sessionConfig,
		EExecutorService* executor) :
		EAbstractIoService(sessionConfig, executor), disconnectOnUnbind_(true ) {
	defaultLocalAddresses_ = new EArrayList<EInetSocketAddress*>();
	boundAddresses_ = new EHashSet<EInetSocketAddress*>();
}

EInetSocketAddress* EAbstractIoAcceptor::getLocalAddress() {
	ESet<EInetSocketAddress*>* localAddresses = getLocalAddresses();
	if (localAddresses->isEmpty()) {
		return null;
	}

	EInetSocketAddress* firstOne = null;
	sp<EIterator<EInetSocketAddress*> > iter = localAddresses->iterator();
	firstOne = iter->next();
	return firstOne;
}

ESet<EInetSocketAddress*>* EAbstractIoAcceptor::getLocalAddresses() {
	return boundAddresses_;
}

EInetSocketAddress* EAbstractIoAcceptor::getDefaultLocalAddress() {
	if (defaultLocalAddresses_->isEmpty()) {
		return null;
	}
	EInetSocketAddress* firstOne = null;
	sp<EIterator<EInetSocketAddress*> > iter = defaultLocalAddresses_->iterator();
	firstOne = iter->next();
	return firstOne;
}

EList<EInetSocketAddress*>* EAbstractIoAcceptor::getDefaultLocalAddresses() {
	return defaultLocalAddresses_;
}

void EAbstractIoAcceptor::setDefaultLocalAddress(EInetSocketAddress* localAddress) {
	if (localAddress == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "localAddress");
	}

	EArrayList<EInetSocketAddress*> newLocalAddresses(false);
	newLocalAddresses.add(localAddress);

	setDefaultLocalAddresses(&newLocalAddresses);
}

void EAbstractIoAcceptor::setDefaultLocalAddress(const char* hostname, int port) {
	EInetSocketAddress isa(hostname, port);
	this->setDefaultLocalAddress(&isa);
}

void EAbstractIoAcceptor::setDefaultLocalAddresses(
		EIterable<EInetSocketAddress*> *localAddresses) {
	if (localAddresses == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "localAddresses");
	}

	SYNCBLOCK(&bindLock) {
		SYNCBLOCK(&boundAddressesLock) {
			if (!boundAddresses_->isEmpty()) {
				throw EIllegalStateException(__FILE__, __LINE__, "localAddress can't be set while the acceptor is bound.");
			}

			EArrayList<EInetSocketAddress*> newLocalAddresses;
			sp<EIterator<EInetSocketAddress*> > iter = localAddresses->iterator();
			while (iter->hasNext()) {
				EInetSocketAddress* a = iter->next();
				checkAddressType(a);
				newLocalAddresses.add(new EInetSocketAddress(*a));
			}

			if (newLocalAddresses.isEmpty()) {
				throw EIllegalArgumentException(__FILE__, __LINE__, "empty localAddresses");
			}

			defaultLocalAddresses_->clear();

			newLocalAddresses.setAutoFree(false);
			for (int i=0; i<newLocalAddresses.size(); i++) {
				EInetSocketAddress* a = newLocalAddresses.getAt(i);
				defaultLocalAddresses_->add(a);
			}
        }}
    }}
}

boolean EAbstractIoAcceptor::isCloseOnDeactivation() {
	return disconnectOnUnbind_;
}

void EAbstractIoAcceptor::setCloseOnDeactivation(boolean disconnectClientsOnUnbind) {
	disconnectOnUnbind_ = disconnectClientsOnUnbind;
}

void EAbstractIoAcceptor::bind()
{
	bind(getDefaultLocalAddresses());
}

void EAbstractIoAcceptor::bind(EInetSocketAddress *localddress) {
	if (localddress == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "localddress");
	}

	EArrayList<EInetSocketAddress*> newLocalAddresses(false);
	newLocalAddresses.add(localddress);
	bind(&newLocalAddresses);
}

void EAbstractIoAcceptor::bind(const char *hostname, int port) {
	EInetSocketAddress isa(hostname, port);
	this->bind(&isa);
}

void EAbstractIoAcceptor::bind(EIterable<EInetSocketAddress*> *localAddresses)
{
	if (isDisposing()) {
		throw EIllegalStateException(__FILE__, __LINE__, "Already disposed.");
	}

	if (localAddresses == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "localAddresses");
	}

	EArrayList<EInetSocketAddress*> localAddressesCopy;

	sp<EIterator<EInetSocketAddress*> > iter = localAddresses->iterator();
	while (iter->hasNext()) {
		EInetSocketAddress* a = iter->next();
		checkAddressType(a);
		localAddressesCopy.add(new EInetSocketAddress(*a));
	}

	if (localAddressesCopy.isEmpty()) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "localAddresses is empty.");
	}

	boolean activate = false;
	SYNCBLOCK(&bindLock) {
		SYNCBLOCK(&boundAddressesLock) {
			if (boundAddresses_->isEmpty()) {
				activate = true;
			}
        }}

		if (getHandler() == null) {
			throw EIllegalStateException(__FILE__, __LINE__, "handler is not set.");
		}

		try {
			sp<ESet<EInetSocketAddress*> > addresses = bindInternal(localAddressesCopy);
			if (addresses != null) {
				SYNCBLOCK(&boundAddressesLock) {
					sp<EIterator<EInetSocketAddress*> > iter = addresses->iterator();
					while (iter->hasNext()) {
						EInetSocketAddress* a = iter->next();
						boundAddresses_->add(new EInetSocketAddress(*a));
					}
                }}
			}
		} catch (EIOException& e) {
			throw e;
		} catch (ERuntimeException& e) {
			throw e;
		} catch (EException& e) {
			EString msg("Failed to bind to: ");
			msg << getLocalAddresses()->toString() << "\nCause:\n  " << e.getMessage();
			throw ERuntimeIoException(__FILE__, __LINE__, msg.c_str());
		}
    }}

	if (activate) {
		getListeners()->fireServiceActivated();
	}
}

void EAbstractIoAcceptor::unbind() {
	unbind(getLocalAddresses());
}

void EAbstractIoAcceptor::unbind(EInetSocketAddress *localAddress) {
	if (localAddress == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "localAddress");
	}

	EArrayList<EInetSocketAddress*> newLocalAddresses(false);
	newLocalAddresses.add(localAddress);
	unbind(&newLocalAddresses);
}

void EAbstractIoAcceptor::unbind(const char* hostname, int port) {
	EInetSocketAddress isa(hostname, port);
	this->unbind(&isa);
}

void EAbstractIoAcceptor::unbind(EIterable<EInetSocketAddress*> *localAddresses) {
	if (localAddresses == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "localAddresses");
	}

	boolean deactivate = false;
	SYNCBLOCK(&bindLock) {
		SYNCBLOCK(&boundAddressesLock) {
			if (boundAddresses_->isEmpty()) {
				return;
            }

			EArrayList<EInetSocketAddress*> localAddressesCopy;
			int specifiedAddressCount = 0;

			sp<EIterator<EInetSocketAddress*> > iter = localAddresses->iterator();
			while (iter->hasNext()) {
				EInetSocketAddress* a = iter->next();
				specifiedAddressCount++;

				if ((a != null) && boundAddresses_->contains(a)) {
					localAddressesCopy.add(new EInetSocketAddress(*a));
				}
			}

			if (specifiedAddressCount == 0) {
				throw EIllegalArgumentException(__FILE__, __LINE__, "localAddresses is empty.");
			}

			if (!localAddressesCopy.isEmpty()) {
				try {
					unbind0(localAddressesCopy);
				} catch (ERuntimeException& e) {
					throw e;
				} catch (EException& e) {
					EString msg("Failed to unbind from: ");
					throw ERuntimeIoException(__FILE__, __LINE__, getLocalAddresses()->toString().c_str());
				}

				boundAddresses_->removeAll(&localAddressesCopy);

				if (boundAddresses_->isEmpty()) {
					deactivate = true;
				}
			}
        }}
    }}

	if (deactivate) {
		getListeners()->fireServiceDeactivated();
	}
}

EStringBase EAbstractIoAcceptor::toString() {
	ETransportMetadata* m = getTransportMetadata();
	EStringBase str;
	str += isActive() ? EStringBase::formatOf("(%s %s acceptor: localAddress(es): %s, managedSessionCount: %d)",
			m->getProviderName(), m->getName(),
			getLocalAddresses()->toString().c_str(),  getManagedSessionCount()).c_str() : "not bound)";
	return str;
}

void EAbstractIoAcceptor::checkAddressType(EInetSocketAddress* a) {
	//
}

} /* namespace eio */
} /* namespace efc */
