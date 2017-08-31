/*
 * EExpiringSessionRecycler.cpp
 *
 *  Created on: 2016-8-29
 *      Author: cxxjava@163.com
 */

#include "../inc/EExpiringSessionRecycler.hh"

namespace efc {
namespace eio {

EExpiringSessionRecycler::~EExpiringSessionRecycler() {
	//
}

EExpiringSessionRecycler::EExpiringSessionRecycler(int timeToLive,
		int expirationInterval): sessionMap(timeToLive, expirationInterval) {
	sessionMap.addExpirationListener(new DefaultExpirationListener());
}

void EExpiringSessionRecycler::put(sp<EIoSession>& session) {
	sessionMap.getExpirer()->startExpiringIfNotStarted();

	EInetSocketAddress* key = session->getRemoteAddress();

	if (!sessionMap.containsKey(key)) {
		sessionMap.put(new EInetSocketAddress(*key), session);
	}
}

sp<EIoSession> EExpiringSessionRecycler::recycle(
		EInetSocketAddress* remoteAddress) {
	return sessionMap.get(remoteAddress);
}

void EExpiringSessionRecycler::remove(sp<EIoSession>& session) {
	sessionMap.remove(session->getRemoteAddress());
}

void EExpiringSessionRecycler::setTimeToLive(int timeToLive) {
	sessionMap.setTimeToLive(timeToLive);
}

int EExpiringSessionRecycler::getTimeToLive() {
	return sessionMap.getTimeToLive();
}

void EExpiringSessionRecycler::setExpirationInterval(int expirationInterval) {
	sessionMap.setExpirationInterval(expirationInterval);
}

int EExpiringSessionRecycler::getExpirationInterval() {
	return sessionMap.getExpirationInterval();
}

void EExpiringSessionRecycler::stopExpiring() {
	sessionMap.getExpirer()->stopExpiring();
}

} /* namespace eio */
} /* namespace efc */
