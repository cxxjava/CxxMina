/*
 * EIoHandlerChain.cpp
 *
 *  Created on: 2016-5-31
 *      Author: cxxjava@163.com
 */

#include "../inc/EIoHandlerChain.hh"

namespace efc {
namespace eio {

volatile int EIoHandlerChain::nextId = 0;

EIoHandlerChain::~EIoHandlerChain() {
	delete head->getCommand(); //!
	delete head;
	delete tail->getCommand(); //!
	delete tail;
}

EIoHandlerChain::EIoHandlerChain() {
	id = nextId++;
	NEXT_COMMAND = new EString(EString::formatOf("%s.%d.nextCommand", typeid(EIoHandlerChain).name(), id));
	head = new Entry(this, null, null, "head", createHeadCommand());
	tail = new Entry(this, head, null, "tail", createTailCommand());
	head->nextEntry = tail;
}

EIoHandlerChain::Entry* EIoHandlerChain::getEntry(const char* name) {
	EString s(name);
	return name2entry.get(&s);
}

EIoHandlerCommand* EIoHandlerChain::get(const char* name) {
	Entry* e = getEntry(name);
	if (e == null) {
		return null;
	}

	return e->getCommand();
}

sp<EIoHandlerCommand::NextCommand> EIoHandlerChain::getNextCommand(const char* name) {
	Entry* e = getEntry(name);
	if (e == null) {
		return null;
	}

	return e->getNextCommand();
}

void EIoHandlerChain::addFirst(const char* name, EIoHandlerCommand* command) {
	SYNCHRONIZED(this) {
		checkAddable(name);
		register_(head, name, command);
    }}
}

void EIoHandlerChain::addLast(const char* name, EIoHandlerCommand* command) {
	SYNCHRONIZED(this) {
		checkAddable(name);
		register_(tail->prevEntry, name, command);
    }}
}

void EIoHandlerChain::addBefore(const char* baseName, const char* name,
		EIoHandlerCommand* command) {
	SYNCHRONIZED(this) {
		Entry* baseEntry = checkOldName(baseName);
		checkAddable(name);
		register_(baseEntry->prevEntry, name, command);
    }}
}

void EIoHandlerChain::addAfter(const char* baseName, const char* name,
		EIoHandlerCommand* command) {
	SYNCHRONIZED(this) {
		Entry* baseEntry = checkOldName(baseName);
		checkAddable(name);
		register_(baseEntry, name, command);
    }}
}

EIoHandlerCommand* EIoHandlerChain::remove(const char* name) {
	SYNCHRONIZED(this) {
		Entry* entry = checkOldName(name);
		deregister(entry);
		EIoHandlerCommand* ihc = entry->getCommand();
		delete entry; //!
		return ihc;
    }}
}

void EIoHandlerChain::clear() {
	SYNCHRONIZED(this) {
		/* @see:
		 Iterator<String> it = new ArrayList<String>(name2entry.keySet()).iterator();
         while (it.hasNext()) {
            this.remove(it.next());
         }
		 */
		sp<EIterator<EMapEntry<EString*, Entry*>*> > iter = name2entry.entrySet()->iterator();
		while (iter->hasNext()) {
			EMapEntry<EString*, Entry*>* entry = iter->next();
			Entry* e = entry->getValue();
			deregister(e);
			delete e; //!
		}
    }}
}

EArrayList<EIoHandlerChain::Entry*> EIoHandlerChain::getAll() {
	EArrayList<Entry*> list(false);
	Entry* e = head->nextEntry;
	while (e != tail) {
		list.add(e);
		e = e->nextEntry;
	}

	return list;
}

EArrayList<EIoHandlerChain::Entry*> EIoHandlerChain::getAllReversed() {
	EArrayList<Entry*> list(false);
	Entry* e = tail->prevEntry;
	while (e != head) {
		list.add(e);
		e = e->prevEntry;
	}
	return list;
}

boolean EIoHandlerChain::contains(const char* name) {
	return getEntry(name) != null;
}

boolean EIoHandlerChain::contains(EIoHandlerCommand* command) {
	Entry* e = head->nextEntry;
	while (e != tail) {
		if (e->getCommand() == command) {
			return true;
		}
		e = e->nextEntry;
	}
	return false;
}

void EIoHandlerChain::execute(sp<EIoHandlerCommand::NextCommand>& next, sp<EIoSession>& session,
		sp<EObject>& message) {
	if (next != null) {
		session->setAttribute(NEXT_COMMAND, next);
	}

	ON_FINALLY_NOTHROW(
		session->removeAttribute(NEXT_COMMAND.get()); //!!!
	) {
		callNextCommand(head, session, message);
    }}
}

EStringBase EIoHandlerChain::toString() {
	EStringBase buf("{ ");

	boolean empty = true;

	Entry* e = head->nextEntry;
	while (e != tail) {
		if (!empty) {
			buf.append(", ");
		} else {
			empty = false;
		}

		EIoHandlerCommand* ihc = e->getCommand();
		EString x = ihc ? ihc->toString() : "null";

		buf.append('(');
		buf.append(e->getName());
		buf.append(':');
		buf.append(x);
		buf.append(')');

		e = e->nextEntry;
	}

	if (empty) {
		buf.append("empty");
	}

	buf.append(" }");

	return buf;
}

EIoHandlerCommand* EIoHandlerChain::createHeadCommand() {
	/* @see:
	 return new IoHandlerCommand() {
		public void execute(NextCommand next, IoSession session, Object message) throws Exception {
			next.execute(session, message);
		}
	 };
	 */
	class IoHandlerCommand: public EIoHandlerCommand {
	public:
		virtual void execute(sp<NextCommand>& next, sp<EIoSession>& session, sp<EObject>& message) THROWS(EException) {
			next->execute(session, message);
		}
	};
	return new IoHandlerCommand();
}

EIoHandlerCommand* EIoHandlerChain::createTailCommand() {
	/* @see:
	 return new IoHandlerCommand() {
		public void execute(NextCommand next, IoSession session, Object message) throws Exception {
			next = (NextCommand) session.getAttribute(NEXT_COMMAND);
			if (next != null) {
				next.execute(session, message);
			}
		}
	 };
	 */
	class IoHandlerCommand: public EIoHandlerCommand {
	private:
		EIoHandlerChain* ioHandlerChain;
	public:
		IoHandlerCommand(EIoHandlerChain* ihc): ioHandlerChain(ihc) {
		}
		virtual void execute(sp<NextCommand>& next, sp<EIoSession>& session, sp<EObject>& message) THROWS(EException) {
			sp<NextCommand> nextCommand = dynamic_pointer_cast<NextCommand>(session->getAttribute(ioHandlerChain->NEXT_COMMAND.get()));
			if (nextCommand != null) {
				nextCommand->execute(session, message);
			}
		}
	};
	return new IoHandlerCommand(this);
}

void EIoHandlerChain::register_(EIoHandlerChain::Entry* prevEntry, const char* name,
		EIoHandlerCommand* command) {
	Entry* newEntry = new Entry(this, prevEntry, prevEntry->nextEntry, name, command);
	prevEntry->nextEntry->prevEntry = newEntry;
	prevEntry->nextEntry = newEntry;

	name2entry.put(new EString(name), newEntry);
}

void EIoHandlerChain::deregister(Entry* entry) {
	Entry* prevEntry = entry->prevEntry;
	Entry* nextEntry = entry->nextEntry;
	prevEntry->nextEntry = nextEntry;
	nextEntry->prevEntry = prevEntry;

	name2entry.remove(&entry->name); //delay to free the entry!
}

EIoHandlerChain::Entry* EIoHandlerChain::checkOldName(const char* baseName) {
	EString s(baseName);
	Entry* e = name2entry.get(&s);
	if (e == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, EString::formatOf("Unknown filter name:%s", baseName).c_str());
	}
	return e;
}

void EIoHandlerChain::checkAddable(const char* name) {
	EString s(name);
	if (name2entry.containsKey(&s)) {
		throw EIllegalArgumentException(__FILE__, __LINE__, EString::formatOf("Other filter is using the same name '%s'", name).c_str());
	}
}

void EIoHandlerChain::callNextCommand(EIoHandlerChain::Entry* entry, sp<EIoSession>& session,
		sp<EObject>& message) {
	sp<NextCommand> next = entry->getNextCommand();
	entry->getCommand()->execute(next, session, message);
}

} /* namespace eio */
} /* namespace efc */
