/*
 * ENamePreservingRunnable.cpp
 *
 *  Created on: 2016-1-29
 *      Author: cxxjava@163.com
 */

#include "../inc/ENamePreservingRunnable.hh"

namespace efc {
namespace eio {

ENamePreservingRunnable::~ENamePreservingRunnable() {
	//
}

ENamePreservingRunnable::ENamePreservingRunnable(sp<ERunnable> runnable,
		EString& newName) {
	this->runnable = runnable;
	this->newName = newName;
}

void ENamePreservingRunnable::run() {
	EThread* currentThread = EThread::currentThread();
	const char* oldName = currentThread->getName();

	if (!newName.isEmpty()) {
		setName(currentThread, newName.c_str());
	}

	try {
		runnable->run();
	} catch (...) {
		setName(currentThread, oldName);
		throw;
	}
	setName(currentThread, oldName);
}

void ENamePreservingRunnable::setName(EThread* thread,
		const char* name) {
	try {
		thread->setName(name);
	} catch (ESecurityException& se) {
		//...
	}
}

} /* namespace eio */
} /* namespace efc */
