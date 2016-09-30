/*
 * ENioProcessor.hh
 *
 *  Created on: 2016-3-9
 *      Author: cxxjava@163.com
 */

#ifndef ENIOPROCESSOR_HH_
#define ENIOPROCESSOR_HH_

#include "EAbstractPollingIoProcessor.hh"

namespace efc {
namespace eio {

/**
 * TODO Add documentation
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class ENioProcessor: public EAbstractPollingIoProcessor {
public:
	virtual ~ENioProcessor();

	/**
	 *
	 * Creates a new instance of NioProcessor.
	 *
	 * @param executor The executor to use
	 */
	ENioProcessor(EExecutor* executor);

protected:
	/**
	 * An encapsulating iterator around the {@link Selector#selectedKeys()} or
	 * the {@link Selector#keys()} iterator;
	 */
	class IoSessionIterator: public EIterator<sp<ENioSession> > {
	private:
		sp<EIterator<ESelectionKey*> > iterator;

	public:
		virtual ~IoSessionIterator() {
			//
		}

		/**
		 * Create this iterator as a wrapper on top of the selectionKey Set.
		 *
		 * @param keys
		 *            The set of selected sessions
		 */
		IoSessionIterator(ESet<ESelectionKey*>* keys) {
			iterator = keys->iterator();
		}

		/**
		 * {@inheritDoc}
		 */
		boolean hasNext() {
			return iterator->hasNext();
		}

		/**
		 * {@inheritDoc}
		 */
		sp<ENioSession> next() {
			ESelectionKey* key = iterator->next();
			SYNCHRONIZED(key) { //!
				sp<ENioSession>* s = (sp<ENioSession>*)(key->attachment());
				return s ? (*s) : null;
            }}
		}

		/**
		 * {@inheritDoc}
		 */
		void remove() {
			iterator->remove();
		}

		sp<ENioSession> moveOut() {
			ESelectionKey* key = iterator->moveOut();
			SYNCHRONIZED(key) { //!
				sp<ENioSession>* s = (sp<ENioSession>*)(key->attachment());
				return s ? (*s) : null;
            }}
		}
	};

	virtual void init(sp<ENioSession>& session) THROWS(EException);

	virtual void destroy(sp<ENioSession>& session) THROWS(EException);

	virtual void doDispose() THROWS(EException);

	virtual int select(llong timeout) THROWS(EException);

	virtual int select() THROWS(EException);

	virtual boolean isSelectorEmpty();

	virtual void wakeup();

	virtual sp<EIterator<sp<ENioSession> > > allSessions();

	virtual sp<EIterator<sp<ENioSession> > > selectedSessions();

	virtual ESessionState getState(sp<ENioSession>& session);

	virtual boolean isWritable(sp<ENioSession>& session);

	virtual boolean isReadable(sp<ENioSession>& session);

	virtual void setInterestedInWrite(sp<ENioSession>& session, boolean isInterested) THROWS(EException);

	virtual void setInterestedInRead(sp<ENioSession>& session, boolean isInterested) THROWS(EException);

	virtual boolean isInterestedInRead(sp<ENioSession>& session);

	virtual boolean isInterestedInWrite(sp<ENioSession>& session);

	virtual int read(sp<ENioSession>& session, EIoBuffer* buf) THROWS(EException);

	virtual int write(sp<ENioSession>& session, EIoBuffer* buf, int length) THROWS(EException);

	virtual int transferFile(sp<ENioSession>& session, EFileRegion* region, int length) THROWS(EException);

	virtual void registerNewSelector() THROWS(EException);

	virtual boolean isBrokenConnection() THROWS(EException);

private:
	/** The selector associated with this processor */
	ESelector* selector;
};

} /* namespace eio */
} /* namespace efc */
#endif /* ENIOPROCESSOR_HH_ */
