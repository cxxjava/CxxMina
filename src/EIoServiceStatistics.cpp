/*
 * EIoServiceStatistics.cpp
 *
 *  Created on: 2014-9-15
 *      Author: cxxjava@163.com
 */

#include "../inc/EIoServiceStatistics.hh"
#include "../inc/EAbstractIoService.hh"

namespace efc {
namespace eio {

EIoServiceStatistics::EIoServiceStatistics(EAbstractIoService* service) :
		service(service),
		readBytesThroughput(0.0),
		writtenBytesThroughput(0.0),
		readMessagesThroughput(0.0),
		writtenMessagesThroughput(0.0),
		largestReadBytesThroughput(0.0),
		largestWrittenBytesThroughput(0.0),
		largestReadMessagesThroughput(0.0),
		largestWrittenMessagesThroughput(0.0),
		readBytes(0L),
		writtenBytes(0L),
		readMessages(0L),
		writtenMessages(0L),
		lastReadTime(0L),
		lastWriteTime(0L),
		lastReadBytes(0L),
		lastWrittenBytes(0L),
		lastReadMessages(0L),
		lastWrittenMessages(0L),
		lastThroughputCalculationTime(0L),
		scheduledWriteBytes(0),
		scheduledWriteMessages(0),
		throughputCalculationInterval(3) {
}

int EIoServiceStatistics::getLargestManagedSessionCount() {
	return service->getListeners()->getLargestManagedSessionCount();
}

long EIoServiceStatistics::getCumulativeManagedSessionCount() {
	return service->getListeners()->getCumulativeManagedSessionCount();
}

llong EIoServiceStatistics::getLastIoTime() {
	SYNCBLOCK(&throughputCalculationLock) {
		return ES_MAX(lastReadTime, lastWriteTime);
    }}
}

llong EIoServiceStatistics::getLastReadTime() {
	SYNCBLOCK(&throughputCalculationLock) {
		return lastReadTime;
    }}
}

llong EIoServiceStatistics::getLastWriteTime() {
	SYNCBLOCK(&throughputCalculationLock) {
		return lastWriteTime;
    }}
}

long EIoServiceStatistics::getReadBytes() {
	SYNCBLOCK(&throughputCalculationLock) {
		return readBytes;
    }}
}

long EIoServiceStatistics::getWrittenBytes() {
	SYNCBLOCK(&throughputCalculationLock) {
		return writtenBytes;
    }}
}

long EIoServiceStatistics::getReadMessages() {
	SYNCBLOCK(&throughputCalculationLock) {
		return readMessages;
    }}
}

long EIoServiceStatistics::getWrittenMessages() {
	SYNCBLOCK(&throughputCalculationLock) {
		return writtenMessages;
    }}
}

double EIoServiceStatistics::getReadBytesThroughput() {
	SYNCBLOCK(&throughputCalculationLock) {
		resetThroughput();
		return readBytesThroughput;
    }}
}

double EIoServiceStatistics::getWrittenBytesThroughput() {
	SYNCBLOCK(&throughputCalculationLock) {
		resetThroughput();
		return writtenBytesThroughput;
    }}
}

double EIoServiceStatistics::getReadMessagesThroughput() {
	SYNCBLOCK(&throughputCalculationLock) {
		resetThroughput();
		return readMessagesThroughput;
    }}
}

double EIoServiceStatistics::getWrittenMessagesThroughput() {
	SYNCBLOCK(&throughputCalculationLock) {
		resetThroughput();
		return writtenMessagesThroughput;
    }}
}

double EIoServiceStatistics::getLargestReadBytesThroughput() {
	SYNCBLOCK(&throughputCalculationLock) {
		return largestReadBytesThroughput;
    }}
}

double EIoServiceStatistics::getLargestWrittenBytesThroughput() {
	SYNCBLOCK(&throughputCalculationLock) {
		return largestWrittenBytesThroughput;
    }}
}

double EIoServiceStatistics::getLargestReadMessagesThroughput() {
	SYNCBLOCK(&throughputCalculationLock) {
		return largestReadMessagesThroughput;
    }}
}

double EIoServiceStatistics::getLargestWrittenMessagesThroughput() {
	SYNCBLOCK(&throughputCalculationLock) {
		return largestWrittenMessagesThroughput;
    }}
}

int EIoServiceStatistics::getThroughputCalculationInterval() {
	return throughputCalculationInterval.get();
}

long EIoServiceStatistics::getThroughputCalculationIntervalInMillis() {
	return throughputCalculationInterval.get() * 1000L;
}

void EIoServiceStatistics::setThroughputCalculationInterval(
		int throughputCalculationInterval) {
	if (throughputCalculationInterval < 0) {
		EString msg("throughputCalculationInterval: ");
		msg += throughputCalculationInterval;
		throw EIllegalArgumentException(__FILE__, __LINE__, msg.c_str());
	}

	this->throughputCalculationInterval.set(throughputCalculationInterval);
}

void EIoServiceStatistics::updateThroughput(llong currentTime) {
	SYNCBLOCK(&throughputCalculationLock) {
		int interval = (int) (currentTime - lastThroughputCalculationTime);
		long minInterval = getThroughputCalculationIntervalInMillis();

		if ((minInterval == 0) || (interval < minInterval)) {
			return;
		}

		long readBytes = this->readBytes;
		long writtenBytes = this->writtenBytes;
		long readMessages = this->readMessages;
		long writtenMessages = this->writtenMessages;

		readBytesThroughput = (readBytes - lastReadBytes) * 1000.0 / interval;
		writtenBytesThroughput = (writtenBytes - lastWrittenBytes) * 1000.0 / interval;
		readMessagesThroughput = (readMessages - lastReadMessages) * 1000.0 / interval;
		writtenMessagesThroughput = (writtenMessages - lastWrittenMessages) * 1000.0 / interval;

		if (readBytesThroughput > largestReadBytesThroughput) {
			largestReadBytesThroughput = readBytesThroughput;
		}

		if (writtenBytesThroughput > largestWrittenBytesThroughput) {
			largestWrittenBytesThroughput = writtenBytesThroughput;
		}

		if (readMessagesThroughput > largestReadMessagesThroughput) {
			largestReadMessagesThroughput = readMessagesThroughput;
		}

		if (writtenMessagesThroughput > largestWrittenMessagesThroughput) {
			largestWrittenMessagesThroughput = writtenMessagesThroughput;
		}

		lastReadBytes = readBytes;
		lastWrittenBytes = writtenBytes;
		lastReadMessages = readMessages;
		lastWrittenMessages = writtenMessages;

		lastThroughputCalculationTime = currentTime;
    }}
}

void EIoServiceStatistics::increaseReadBytes(long increment, llong currentTime) {
	SYNCBLOCK(&throughputCalculationLock) {
		readBytes += increment;
		lastReadTime = currentTime;
    }}
}

void EIoServiceStatistics::increaseReadMessages(llong currentTime) {
	SYNCBLOCK(&throughputCalculationLock) {
		readMessages++;
		lastReadTime = currentTime;
    }}
}

void EIoServiceStatistics::increaseWrittenBytes(int increment,
		llong currentTime) {
	SYNCBLOCK(&throughputCalculationLock) {
		writtenBytes += increment;
		lastWriteTime = currentTime;
    }}
}

void EIoServiceStatistics::increaseWrittenMessages(llong currentTime) {
	SYNCBLOCK(&throughputCalculationLock) {
		writtenMessages++;
		lastWriteTime = currentTime;
    }}
}

int EIoServiceStatistics::getScheduledWriteBytes() {
	SYNCBLOCK(&throughputCalculationLock) {
		return scheduledWriteBytes;
    }}
}

void EIoServiceStatistics::increaseScheduledWriteBytes(int increment) {
	SYNCBLOCK(&throughputCalculationLock) {
		scheduledWriteBytes += increment;
    }}
}

int EIoServiceStatistics::getScheduledWriteMessages() {
	SYNCBLOCK(&throughputCalculationLock) {
		return scheduledWriteMessages;
    }}
}

void EIoServiceStatistics::increaseScheduledWriteMessages() {
	SYNCBLOCK(&throughputCalculationLock) {
		scheduledWriteMessages++;
    }}
}

void EIoServiceStatistics::decreaseScheduledWriteMessages() {
	SYNCBLOCK(&throughputCalculationLock) {
		scheduledWriteMessages--;
    }}
}

void EIoServiceStatistics::setLastReadTime(llong lastReadTime) {
	SYNCBLOCK(&throughputCalculationLock) {
		this->lastReadTime = lastReadTime;
    }}
}

void EIoServiceStatistics::setLastWriteTime(llong lastWriteTime) {
	SYNCBLOCK(&throughputCalculationLock) {
		this->lastWriteTime = lastWriteTime;
    }}
}

void EIoServiceStatistics::setLastThroughputCalculationTime(
		llong lastThroughputCalculationTime) {
	SYNCBLOCK(&throughputCalculationLock) {
		this->lastThroughputCalculationTime = lastThroughputCalculationTime;
    }}
}

void EIoServiceStatistics::resetThroughput() {
	if (service->getManagedSessionCount() == 0) {
		readBytesThroughput = 0;
		writtenBytesThroughput = 0;
		readMessagesThroughput = 0;
		writtenMessagesThroughput = 0;
	}
}

} /* namespace eio */
} /* namespace efc */
