/*
 * EDefaultIoFilterChainBuilder.cpp
 *
 *  Created on: 2015-12-28
 *      Author: cxxjava@163.com
 */

#include "EDefaultIoFilterChainBuilder.hh"

namespace efc {
namespace eio {

class EntryImpl : public EIoFilterChain::Entry {
public:
	EString name;
	EIoFilter* volatile filter;
	EDefaultIoFilterChainBuilder* owner;

	virtual ~EntryImpl() {
		//TODO...
	}

	EntryImpl(const char* name, EIoFilter* filter, EDefaultIoFilterChainBuilder* dicb) {
		if (name == null) {
			throw EIllegalArgumentException(__FILE__, __LINE__, "name");
		}
		if (filter == null) {
			throw EIllegalArgumentException(__FILE__, __LINE__, "filter");
		}

		this->name = name;
		this->filter = filter;
		this->owner = dicb;
	}

	const char* getName() {
		return name.c_str();
	}

	EIoFilter* getFilter() {
		return filter;
	}

	void setFilter(EIoFilter* filter) {
		this->filter = filter;
	}

	EIoFilter::NextFilter* getNextFilter() {
		throw EIllegalStateException(__FILE__, __LINE__);
	}

	virtual EStringBase toString() {
		return EStringBase::formatOf("(%s:%s)", name.c_str(), filter->toString().c_str());
	}

	void addAfter(const char* name, EIoFilter* filter) {
		owner->addAfter(getName(), name, filter);
	}

	void addBefore(const char* name, EIoFilter* filter) {
		owner->addBefore(getName(), name, filter);
	}

	void remove() {
		owner->remove(getName());
	}

	void replace(EIoFilter* newFilter) {
		owner->replace(getName(), newFilter);
	}
};

EDefaultIoFilterChainBuilder::~EDefaultIoFilterChainBuilder() {
	delete entries;
}

EDefaultIoFilterChainBuilder::EDefaultIoFilterChainBuilder() {
	entries = new ECopyOnWriteArrayList<EIoFilterChain::Entry>();
}

EDefaultIoFilterChainBuilder::EDefaultIoFilterChainBuilder(
		EDefaultIoFilterChainBuilder* filterChain) {
	if (filterChain == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "filterChain");
	}
}

sp<EIoFilterChain::Entry> EDefaultIoFilterChainBuilder::getEntry(
		const char* name) {
	sp<EConcurrentIterator<EIoFilterChain::Entry> > citer = entries->iterator();
	while (citer->hasNext()) {
		sp<EIoFilterChain::Entry> e = citer->next();
        EString n(e->getName());
		if (n.equals(name)) {
			return e;
		}
	}

	return null;
}

sp<EIoFilterChain::Entry> EDefaultIoFilterChainBuilder::getEntry(
		EIoFilter* filter) {
	sp<EConcurrentIterator<EIoFilterChain::Entry> > citer = entries->iterator();
	while (citer->hasNext()) {
		sp<EIoFilterChain::Entry> e = citer->next();
		if (e->getFilter() == filter) {
			return e;
		}
	}

	return null;
}

EIoFilter* EDefaultIoFilterChainBuilder::get(const char* name) {
	sp<EIoFilterChain::Entry> e = getEntry(name);
	if (e == null) {
		return null;
	}

	return e->getFilter();
}

boolean EDefaultIoFilterChainBuilder::contains(const char* name) {
	return getEntry(name) != null;
}

boolean EDefaultIoFilterChainBuilder::contains(EIoFilter* filter) {
	return getEntry(filter) != null;
}

void EDefaultIoFilterChainBuilder::addFirst(const char* name,
		EIoFilter* filter) {
	SYNCHRONIZED(this) {
		register_(0, new EntryImpl(name, filter, this));
    }}
}

void EDefaultIoFilterChainBuilder::addLast(const char* name,
		EIoFilter* filter) {
	SYNCHRONIZED(this) {
		register_(entries->size(), new EntryImpl(name, filter, this));
    }}
}

void EDefaultIoFilterChainBuilder::addBefore(const char* baseName,
		const char* name, EIoFilter* filter) {
	SYNCHRONIZED(this) {
		checkBaseName(baseName);

		for (sp<EConcurrentListIterator<EIoFilterChain::Entry> > i = entries->listIterator(); i->hasNext();) {
			sp<EIoFilterChain::Entry> base = i->next();
			if (eso_strcmp(base->getName(), baseName) == 0) {
				register_(i->previousIndex(), new EntryImpl(name, filter, this));
				break;
			}
		}
    }}
}

void EDefaultIoFilterChainBuilder::addAfter(const char* baseName,
		const char* name, EIoFilter* filter) {
	SYNCHRONIZED(this) {
		checkBaseName(baseName);

		for (sp<EConcurrentListIterator<EIoFilterChain::Entry> > i = entries->listIterator(); i->hasNext();) {
			sp<EIoFilterChain::Entry> base = i->next();
			if (eso_strcmp(base->getName(), baseName) == 0) {
				register_(i->nextIndex(), new EntryImpl(name, filter, this));
				break;
			}
		}
    }}
}

EIoFilter* EDefaultIoFilterChainBuilder::remove(const char* name) {
	SYNCHRONIZED(this) {
		if (name == null) {
			throw EIllegalArgumentException(__FILE__, __LINE__, "name");
		}

		for (sp<EConcurrentListIterator<EIoFilterChain::Entry> > i = entries->listIterator(); i->hasNext();) {
			sp<EIoFilterChain::Entry> e = i->next();
			if (eso_strcmp(e->getName(), name) == 0) {
				entries->removeAt(i->previousIndex());
				return e->getFilter();
			}
		}

		EString msg("Unknown filter name: ");
		msg += name;
		throw EIllegalArgumentException(__FILE__, __LINE__, msg.c_str());
    }}
}

EIoFilter* EDefaultIoFilterChainBuilder::remove(EIoFilter* filter) {
	SYNCHRONIZED(this) {
		if (filter == null) {
			throw EIllegalArgumentException(__FILE__, __LINE__, "filter");
		}

		for (sp<EConcurrentListIterator<EIoFilterChain::Entry> > i = entries->listIterator(); i->hasNext();) {
			sp<EIoFilterChain::Entry> e = i->next();
			if (e->getFilter() == filter) {
				entries->removeAt(i->previousIndex());
				return e->getFilter();
			}
		}

		EString msg("Filter not found: ");
		msg += filter->toString();
		throw EIllegalArgumentException(__FILE__, __LINE__, msg.c_str());
    }}
}

EIoFilter* EDefaultIoFilterChainBuilder::replace(const char* name,
		EIoFilter* newFilter) {
	SYNCHRONIZED(this) {
		checkBaseName(name);
		sp<EIoFilterChain::Entry> e = getEntry(name);
		EntryImpl* ei = dynamic_cast<EntryImpl*>(e.get());
		EIoFilter* oldFilter = ei->getFilter();
		ei->setFilter(newFilter);
		return oldFilter;
    }}
}

void EDefaultIoFilterChainBuilder::replace(EIoFilter* oldFilter,
		EIoFilter* newFilter) {
	SYNCHRONIZED(this) {
		sp<EConcurrentIterator<EIoFilterChain::Entry> > citer = entries->iterator();
		while (citer->hasNext()) {
			sp<EIoFilterChain::Entry> e = citer->next();
			if (e->getFilter() == oldFilter) {
				EntryImpl* ei = dynamic_cast<EntryImpl*>(e.get());
				ei->setFilter(newFilter);
				return;
			}
		}
		EString msg("Filter not found: ");
		msg += oldFilter->toString();
		throw EIllegalArgumentException(__FILE__, __LINE__, msg.c_str());
    }}
}

void EDefaultIoFilterChainBuilder::clear() {
	SYNCHRONIZED(this) {
		entries->clear();
    }}
}

void EDefaultIoFilterChainBuilder::buildFilterChain(EIoFilterChain* chain) {
	sp<EConcurrentIterator<EIoFilterChain::Entry> > citer = entries->iterator();
	while (citer->hasNext()) {
		sp<EIoFilterChain::Entry> e = citer->next();
		chain->addLast(e->getName(), e->getFilter());
	}
}

EStringBase EDefaultIoFilterChainBuilder::toString() {
	EStringBase buf("{ ");

	boolean empty = true;

	sp<EConcurrentIterator<EIoFilterChain::Entry> > citer = entries->iterator();
	while (citer->hasNext()) {
		sp<EIoFilterChain::Entry> e = citer->next();
		if (!empty) {
			buf.append(", ");
		} else {
			empty = false;
		}

		buf.append('(');
		buf.append(e->getName());
		buf.append(':');
		buf.append(e->getFilter()->toString());
		buf.append(')');
	}

	if (empty) {
		buf.append("empty");
	}

	buf.append(" }");

	return buf;
}

void EDefaultIoFilterChainBuilder::register_(int index, EIoFilterChain::Entry* e) {
	if (contains(e->getName())) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "Other filter is using the same name.");
	}

	entries->addAt(index, e);
}

void EDefaultIoFilterChainBuilder::checkBaseName(const char* baseName) {
	if (baseName == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "baseName");
	}

	if (!contains(baseName)) {
		EString msg("Unknown filter name: ");
		msg += baseName;
		throw EIllegalArgumentException(__FILE__, __LINE__, msg.c_str());
	}
}

void EDefaultIoFilterChainBuilder::destroy() {
	sp<EConcurrentIterator<EIoFilterChain::Entry> > citer = entries->iterator();
	while (citer->hasNext()) {
		sp<EIoFilterChain::Entry> e = citer->next();
		e->getFilter()->destroy();
	}
}

} /* namespace eio */
} /* namespace efc */
