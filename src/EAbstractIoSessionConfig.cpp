/*
 * EAbstractIoSessionConfig.cpp
 *
 *  Created on: 2013-8-19
 *      Author: cxxjava@163.com
 */

#include "EAbstractIoSessionConfig.hh"
#include "EIllegalArgumentException.hh"

namespace efc {
namespace eio {

EAbstractIoSessionConfig::EAbstractIoSessionConfig() :
		minReadBufferSize(64), readBufferSize(2048), maxReadBufferSize(65536), idleTimeForRead(
				0), idleTimeForWrite(0), idleTimeForBoth(0), writeTimeout(60), useReadOperation(
				false ), throughputCalculationInterval(3) {
	// Do nothing
}

void EAbstractIoSessionConfig::setAll(EIoSessionConfig* config) {
	if (config == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "config");
	}

	setReadBufferSize(config->getReadBufferSize());
	setMinReadBufferSize(config->getMinReadBufferSize());
	setMaxReadBufferSize(config->getMaxReadBufferSize());
	setIdleTime(BOTH_IDLE, config->getIdleTime(BOTH_IDLE));
	setIdleTime(READER_IDLE, config->getIdleTime(READER_IDLE));
	setIdleTime(WRITER_IDLE, config->getIdleTime(WRITER_IDLE));
	setWriteTimeout(config->getWriteTimeout());
	setUseReadOperation(config->isUseReadOperation());
	setThroughputCalculationInterval(config->getThroughputCalculationInterval());

	doSetAll(config);
}

int EAbstractIoSessionConfig::getReadBufferSize() {
	return readBufferSize;
}

void EAbstractIoSessionConfig::setReadBufferSize(int readBufferSize) {
	if (readBufferSize <= 0) {
		throw EIllegalArgumentException(__FILE__, __LINE__, EString::formatOf("readBufferSize: %d (expected: 1+)", readBufferSize).c_str());
	}
	this->readBufferSize = readBufferSize;
}

int EAbstractIoSessionConfig::getMinReadBufferSize() {
	return minReadBufferSize;
}

void EAbstractIoSessionConfig::setMinReadBufferSize(int minReadBufferSize) {
	if (minReadBufferSize <= 0) {
		throw EIllegalArgumentException(__FILE__, __LINE__, EString::formatOf("minReadBufferSize: %d (expected: 1+)", minReadBufferSize).c_str());
	}
	if (minReadBufferSize > maxReadBufferSize) {
		throw EIllegalArgumentException(__FILE__, __LINE__, EString::formatOf("minReadBufferSize: %d (expected: smaller than %d)", minReadBufferSize, maxReadBufferSize).c_str());

	}
	this->minReadBufferSize = minReadBufferSize;
}

int EAbstractIoSessionConfig::getMaxReadBufferSize() {
	return maxReadBufferSize;
}

void EAbstractIoSessionConfig::setMaxReadBufferSize(int maxReadBufferSize) {
	if (maxReadBufferSize <= 0) {
		throw EIllegalArgumentException(__FILE__, __LINE__, EString::formatOf("maxReadBufferSize: %d (expected: 1+)",
				maxReadBufferSize).c_str());
	}

	if (maxReadBufferSize < minReadBufferSize) {
		throw EIllegalArgumentException(__FILE__, __LINE__, EString::formatOf("maxReadBufferSize: %d (expected: greater than %d)",
				maxReadBufferSize, minReadBufferSize).c_str());

	}
	this->maxReadBufferSize = maxReadBufferSize;
}

int EAbstractIoSessionConfig::getIdleTime(EIdleStatus status) {
	if (status == BOTH_IDLE) {
		return idleTimeForBoth;
	}

	if (status == READER_IDLE) {
		return idleTimeForRead;
	}

	if (status == WRITER_IDLE) {
		return idleTimeForWrite;
	}

	throw EIllegalArgumentException(__FILE__, __LINE__, EString::formatOf("Unknown idle status: %d", status).c_str());
}

llong EAbstractIoSessionConfig::getIdleTimeInMillis(EIdleStatus status) {
	return getIdleTime(status) * 1000L;
}

void EAbstractIoSessionConfig::setIdleTime(EIdleStatus status, int idleTime) {
	if (idleTime < 0) {
		throw EIllegalArgumentException(__FILE__, __LINE__, EString::formatOf("Illegal idle time: %d", idleTime).c_str());
	}

	if (status == BOTH_IDLE) {
		idleTimeForBoth = idleTime;
	} else if (status == READER_IDLE) {
		idleTimeForRead = idleTime;
	} else if (status == WRITER_IDLE) {
		idleTimeForWrite = idleTime;
	} else {
		throw EIllegalArgumentException(__FILE__, __LINE__, EString::formatOf("Unknown idle status: %d", status).c_str());
	}
}

int EAbstractIoSessionConfig::getBothIdleTime() {
	return getIdleTime(BOTH_IDLE);
}

llong EAbstractIoSessionConfig::getBothIdleTimeInMillis() {
	return getIdleTimeInMillis(BOTH_IDLE);
}

int EAbstractIoSessionConfig::getReaderIdleTime() {
	return getIdleTime(READER_IDLE);
}

llong EAbstractIoSessionConfig::getReaderIdleTimeInMillis() {
	return getIdleTimeInMillis(READER_IDLE);
}

int EAbstractIoSessionConfig::getWriterIdleTime() {
	return getIdleTime(WRITER_IDLE);
}

llong EAbstractIoSessionConfig::getWriterIdleTimeInMillis() {
	return getIdleTimeInMillis(WRITER_IDLE);
}

void EAbstractIoSessionConfig::setBothIdleTime(int idleTime) {
	setIdleTime(BOTH_IDLE, idleTime);
}

void EAbstractIoSessionConfig::setReaderIdleTime(int idleTime) {
	setIdleTime(READER_IDLE, idleTime);
}

void EAbstractIoSessionConfig::setWriterIdleTime(int idleTime) {
	setIdleTime(WRITER_IDLE, idleTime);
}

int EAbstractIoSessionConfig::getWriteTimeout() {
	return writeTimeout;
}

llong EAbstractIoSessionConfig::getWriteTimeoutInMillis() {
	return writeTimeout * 1000L;
}

void EAbstractIoSessionConfig::setWriteTimeout(int writeTimeout) {
	if (writeTimeout < 0) {
		throw EIllegalArgumentException(__FILE__, __LINE__, EString::formatOf("Illegal write timeout: %d", writeTimeout).c_str());
	}
	this->writeTimeout = writeTimeout;
}

boolean EAbstractIoSessionConfig::isUseReadOperation() {
	return useReadOperation;
}

void EAbstractIoSessionConfig::setUseReadOperation(boolean useReadOperation) {
	this->useReadOperation = useReadOperation;
}

int EAbstractIoSessionConfig::getThroughputCalculationInterval() {
	return throughputCalculationInterval;
}

void EAbstractIoSessionConfig::setThroughputCalculationInterval(int throughputCalculationInterval) {
	if (throughputCalculationInterval < 0) {
		throw EIllegalArgumentException(__FILE__, __LINE__, EString::formatOf("throughputCalculationInterval: %d", throughputCalculationInterval).c_str());
	}

	this->throughputCalculationInterval = throughputCalculationInterval;
}

llong EAbstractIoSessionConfig::getThroughputCalculationIntervalInMillis() {
	return throughputCalculationInterval * 1000L;
}

} /* namespace eio */
} /* namespace efc */
