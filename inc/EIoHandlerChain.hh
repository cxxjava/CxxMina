/*
 * EIoHandlerChain.hh
 *
 *  Created on: 2016-5-31
 *      Author: cxxjava@163.com
 */

#ifndef EIOHANDLERCHAIN_HH_
#define EIOHANDLERCHAIN_HH_

#include "./EIoHandlerCommand.hh"

namespace efc {
namespace eio {

/**
 * A chain of {@link IoHandlerCommand}s.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EIoHandlerChain: virtual public EIoHandlerCommand, public ESynchronizeable {
public:
	/**
	 * Represents a name-command pair that an {@link IoHandlerChain} contains.
	 *
	 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
	 */
	class Entry: public EObject {
	public:
		virtual ~Entry() {
			//
		}
		Entry(EIoHandlerChain* ioHanderChain, Entry* prevEntry, Entry* nextEntry, const char* name, EIoHandlerCommand* command) {
			if (command == null) {
				throw EIllegalArgumentException(__FILE__, __LINE__, "command");
			}
			if (name == null) {
				throw EIllegalArgumentException(__FILE__, __LINE__, "name");
			}

			this->ioHanderChain = ioHanderChain;
			this->prevEntry = prevEntry;
			this->nextEntry = nextEntry;
			this->name = name;
			this->command = command;

			class NextCommand_ : public NextCommand {
			public:
				NextCommand_(Entry* e): entry(e) {
				}
				virtual void execute(sp<EIoSession>& session, sp<EObject>& message) THROWS(EException) {
					entry->ioHanderChain->callNextCommand(entry->nextEntry, session, message);
				}
			private:
				Entry* entry;
			};
			this->nextCommand = new NextCommand_(this);
		}

		/**
		 * @return the name of the command.
		 */
		const char* getName() {
            return name.c_str();
        }

		/**
		 * @return the command.
		 */
		EIoHandlerCommand* getCommand() {
            return command;
        }

		/**
		 * @return the {@link IoHandlerCommand.NextCommand} of the command.
		 */
		sp<NextCommand> getNextCommand() {
            return nextCommand;
        }

	private:
		friend class EIoHandlerChain;
		friend class NextCommand_;

		EIoHandlerChain* ioHanderChain;

		Entry* prevEntry;

		Entry* nextEntry;

		EString name;

		EIoHandlerCommand* command;

		sp<NextCommand> nextCommand;
	};

public:
	virtual ~EIoHandlerChain();

	/**
	 * Creates a new, empty chain of {@link IoHandlerCommand}s.
	 */
	EIoHandlerChain();

	Entry* getEntry(const char* name);

	EIoHandlerCommand* get(const char* name);

	sp<NextCommand> getNextCommand(const char* name);

	synchronized void addFirst(const char* name, EIoHandlerCommand* command);
	synchronized void addLast(const char* name, EIoHandlerCommand* command);
	synchronized void addBefore(const char* baseName, const char* name, EIoHandlerCommand* command);
	synchronized void addAfter(const char* baseName, const char* name, EIoHandlerCommand* command);

	synchronized EIoHandlerCommand* remove(const char* name);
	synchronized void clear() THROWS(EException);

	EArrayList<Entry*> getAll();
	EArrayList<Entry*> getAllReversed();

	boolean contains(const char* name);
	boolean contains(EIoHandlerCommand* command);

	/**
	 * {@inheritDoc}
	 */
	virtual void execute(sp<NextCommand>& next, sp<EIoSession>& session, sp<EObject>& message) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual EString toString();

private:
	static volatile int nextId;// = 0;
	int id;// = nextId++;
	sp<EString> NEXT_COMMAND;// = IoHandlerChain.class.getName() + '.' + id + ".nextCommand";
	EHashMap<EString*, Entry*> name2entry;// = new ConcurrentHashMap<String, Entry>();
	Entry* head;
	Entry* tail;

	EIoHandlerCommand* createHeadCommand();
	EIoHandlerCommand* createTailCommand();

	void register_(Entry* prevEntry, const char* name, EIoHandlerCommand* command);
	void deregister(Entry* entry);

	/**
	 * Throws an exception when the specified filter name is not registered in this chain.
	 *
	 * @return An filter entry with the specified name.
	 */
	Entry* checkOldName(const char* baseName);

	/**
	 * Checks the specified filter name is already taken and throws an exception if already taken.
	 */
	void checkAddable(const char* name);

	void callNextCommand(Entry* entry, sp<EIoSession>& session, sp<EObject>& message) THROWS(EException);
};

} /* namespace eio */
} /* namespace efc */
#endif /* EIOHANDLERCHAIN_HH_ */
