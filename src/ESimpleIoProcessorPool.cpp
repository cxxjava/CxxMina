/*
 * ESimpleIoProcessorPool.cpp
 *
 *  Created on: 2016-1-27
 *      Author: cxxjava@163.com
 */

#include "ESimpleIoProcessorPool.hh"
#include "ENioProcessor.hh"

namespace efc {
namespace eio {

sp<ELogger> ESimpleIoProcessorPool::LOGGER = ELoggerManager::getLogger("ESimpleIoProcessorPool");
int ESimpleIoProcessorPool::DEFAULT_SIZE =  ERuntime::getRuntime()->availableProcessors() + 1;
sp<EAttributeKey> ESimpleIoProcessorPool::PROCESSOR = new EAttributeKey(typeid(ESimpleIoProcessorPool).name(), "processor");

ESimpleIoProcessorPool::~ESimpleIoProcessorPool() throw() {
	if (executor) {
		executor->shutdownNow();
		executor->awaitTermination();
	}
	delete executor;
	delete pool;
}

void ESimpleIoProcessorPool::ctor(int size) {
	disposing = false;
	disposed = false;

	this->executor = EExecutors::newCachedThreadPool();
	// Set a default reject handler
	(dynamic_cast<EThreadPoolExecutor*>(this->executor))->setRejectedExecutionHandler(
			new EThreadPoolExecutor::CallerRunsPolicy());

	pool = new EA<sp<EIoProcessor> >(size);

	boolean success = false;
	ON_FINALLY_NOTHROW(
		if (!success) {
			dispose();
		}
	) {

		// Constructor found now use it for all subsequent instantiations
		for (int i = 0; i < pool->length(); i++) {
			try {
				(*pool)[i] = new ENioProcessor(executor);
			} catch (EException& e) {
				// Won't happen because it has been done previously
			}
		}

		success = true;
    }}
}

ESimpleIoProcessorPool::ESimpleIoProcessorPool() {
	ctor(DEFAULT_SIZE);
}

ESimpleIoProcessorPool::ESimpleIoProcessorPool(int size) {
	ctor(size);
}

boolean ESimpleIoProcessorPool::isDisposing() {
	return disposing;
}

boolean ESimpleIoProcessorPool::isDisposed() {
	return disposed;
}

void ESimpleIoProcessorPool::dispose() {
	if (disposed) {
		return;
	}

	SYNCBLOCK(&disposalLock) {
		if (!disposing) {
			disposing = true;

			for (int i=0; i<pool->length(); i++) {
				sp<EIoProcessor> ioProcessor = pool->getAt(i);
				if (ioProcessor == null) {
					// Special case if the pool has not been initialized properly
					continue;
				}

				if (ioProcessor->isDisposing()) {
					continue;
				}

				try {
					ioProcessor->dispose();
				} catch (EException& e) {
						LOGGER->warn(__FILE__, __LINE__,
								EString::formatOf(
										"Failed to dispose the %s IoProcessor.",
										typeid(ioProcessor.get()).name()).c_str(),
								e);
				}
			}

			executor->shutdown();
		}

		// cxxjava@163.com
		executor->awaitTermination();

		//@see: Arrays.fill(pool, null);
		pool->clear();
		disposed = true;
    }}
}

void ESimpleIoProcessorPool::add(sp<EIoSession> session) {
	getProcessor(session)->add(session);
}

void ESimpleIoProcessorPool::flush(sp<EIoSession>& session) {
	getProcessor(session)->flush(session);
}

void ESimpleIoProcessorPool::write(sp<EIoSession>& session,
		sp<EWriteRequest>& writeRequest) {
	getProcessor(session)->write(session, writeRequest);
}

void ESimpleIoProcessorPool::remove(sp<EIoSession>& session) {
	getProcessor(session)->remove(session);
}

void ESimpleIoProcessorPool::updateTrafficControl(sp<EIoSession>& session) {
	getProcessor(session)->updateTrafficControl(session);
}

sp<EIoProcessor> ESimpleIoProcessorPool::getProcessor(sp<EIoSession>& session) {
	sp<EIoProcessor> processor = dynamic_pointer_cast<EIoProcessor>(session->getAttribute(PROCESSOR.get()));

	if (processor == null) {
		if (disposed || disposing) {
			throw EIllegalStateException(__FILE__,  __LINE__, "A disposed processor cannot be accessed.");
		}

		processor = pool->getAt(EMath::abs((int) session->getId()) % pool->length());

		if (processor == null) {
			throw EIllegalStateException(__FILE__,  __LINE__, "A disposed processor cannot be accessed.");
		}

		session->setAttributeIfAbsent(PROCESSOR, processor);
	}

	return processor;
}

} /* namespace eio */
} /* namespace efc */
