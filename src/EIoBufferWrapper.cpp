/*
 * EIoBufferWrapper.cpp
 *
 *  Created on: 2016-7-6
 *      Author: cxxjava@lotuseed.com
 */

#include "../inc/EIoBufferWrapper.hh"

namespace efc {
namespace eio {

EIoBufferWrapper::~EIoBufferWrapper() {
	delete buf_;
}

EIoBufferWrapper::EIoBufferWrapper(EIoBuffer* buf) {
	if (buf == null) {
		throw EIllegalArgumentException(__FILE__, __LINE__, "buf");
	}
	this->buf_ = buf;
}

EIoBuffer* EIoBufferWrapper::getParentBuffer() {
	return buf_;
}

EIoBuffer* EIoBufferWrapper::duplicate() {
	return buf_->duplicate();
}

EIoBuffer* EIoBufferWrapper::slice() {
	return buf_->slice();
}

void EIoBufferWrapper::free() {
	buf_->free();
}

EIOByteBuffer* EIoBufferWrapper::buf() {
	return buf_->buf();
}

boolean EIoBufferWrapper::isDerived() {
	return buf_->isDerived();
}

boolean EIoBufferWrapper::isReadOnly() {
	return buf_->isReadOnly();
}

int EIoBufferWrapper::minimumCapacity() {
	return buf_->minimumCapacity();
}

EIoBuffer* EIoBufferWrapper::minimumCapacity(int minimumCapacity) {
	buf_->minimumCapacity(minimumCapacity);
	return this;
}

int EIoBufferWrapper::capacity() {
	return buf_->capacity();
}

EIoBuffer* EIoBufferWrapper::capacity(int newCapacity) {
	buf_->capacity(newCapacity);
	return this;
}

boolean EIoBufferWrapper::isAutoExpand() {
	return buf_->isAutoExpand();
}

EIoBuffer* EIoBufferWrapper::setAutoExpand(boolean autoExpand) {
	buf_->setAutoExpand(autoExpand);
	return this;
}

boolean EIoBufferWrapper::isAutoShrink() {
	return buf_->isAutoShrink();
}

EIoBuffer* EIoBufferWrapper::setAutoShrink(boolean autoShrink) {
	buf_->setAutoShrink(autoShrink);
	return this;
}

EIoBuffer* EIoBufferWrapper::expand(int expectedRemaining) {
	buf_->expand(expectedRemaining);
	return this;
}

EIoBuffer* EIoBufferWrapper::expand(int position, int expectedRemaining) {
	buf_->expand(position, expectedRemaining);
	return this;
}

EIoBuffer* EIoBufferWrapper::shrink() {
	buf_->shrink();
	return this;
}

int EIoBufferWrapper::position() {
	return buf_->position();
}

EIoBuffer* EIoBufferWrapper::position(int newPosition) {
	buf_->position(newPosition);
	return this;
}

int EIoBufferWrapper::limit() {
	return buf_->limit();
}

EIoBuffer* EIoBufferWrapper::limit(int newLimit) {
	buf_->limit(newLimit);
	return this;
}

EIoBuffer* EIoBufferWrapper::mark() {
	buf_->mark();
	return this;
}

int EIoBufferWrapper::markValue() {
	return buf_->markValue();
}

EIoBuffer* EIoBufferWrapper::reset() {
	buf_->reset();
	return this;
}

EIoBuffer* EIoBufferWrapper::clear() {
	buf_->clear();
	return this;
}

EIoBuffer* EIoBufferWrapper::sweep() {
	buf_->sweep();
	return this;
}

EIoBuffer* EIoBufferWrapper::sweep(byte value) {
	buf_->sweep(value);
	return this;
}

EIoBuffer* EIoBufferWrapper::flip() {
	buf_->flip();
	return this;
}

EIoBuffer* EIoBufferWrapper::rewind() {
	buf_->rewind();
	return this;
}

EIoBuffer* EIoBufferWrapper::compact() {
	buf_->compact();
	return this;
}

EIoBuffer* EIoBufferWrapper::skip(int size) {
	buf_->skip(size);
	return this;
}

int EIoBufferWrapper::indexOf(byte b) {
	return buf_->indexOf(b);
}

int EIoBufferWrapper::remaining() {
	return buf_->remaining();
}

boolean EIoBufferWrapper::hasRemaining() {
	return buf_->hasRemaining();
}

EString EIoBufferWrapper::getHexdump(int lengthLimit) {
	return buf_->getHexdump(lengthLimit);
}

int EIoBufferWrapper::compareTo(EIoBuffer* that) {
	return buf_->compareTo(that);
}

EStringBase EIoBufferWrapper::toString() {
	return buf_->toString();
}

void* EIoBufferWrapper::address() {
	return buf_->address();
}

void* EIoBufferWrapper::current() {
	return buf_->current();
}

byte EIoBufferWrapper::get() {
	return buf_->get();
}

EIoBuffer* EIoBufferWrapper::put(byte b) {
	buf_->put(b);
	return this;
}

byte EIoBufferWrapper::get(int index) {
	return buf_->get(index);
}

EIoBuffer* EIoBufferWrapper::put(int index, byte b) {
	buf_->put(index, b);
	return this;
}

EIoBuffer* EIoBufferWrapper::get(void* dst, int size, int length) {
	buf_->get(dst, size, length);
	return this;
}

EIoBuffer* EIoBufferWrapper::get(EA<byte>* dst) {
	buf_->get(dst);
	return this;
}

EIoBuffer* EIoBufferWrapper::getSlice(int index, int length) {
	buf_->getSlice(index, length);
	return this;
}

EIoBuffer* EIoBufferWrapper::getSlice(int length) {
	buf_->getShort(length);
	return this;
}

EIoBuffer* EIoBufferWrapper::put(EIOByteBuffer* src) {
	buf_->put(src);
	return this;
}

EIoBuffer* EIoBufferWrapper::put(EIoBuffer* src) {
	buf_->put(src);
	return this;
}

EIoBuffer* EIoBufferWrapper::put(const void* src, int length) {
	buf_->put(src, length);
	return this;
}

EIoBuffer* EIoBufferWrapper::put(EA<byte>* src) {
	buf_->put(src);
	return this;
}

char EIoBufferWrapper::getChar() {
	return buf_->getChar();
}

EIoBuffer* EIoBufferWrapper::putChar(char value) {
	buf_->putChar(value);
	return this;
}

char EIoBufferWrapper::getChar(int index) {
	return buf_->getChar(index);
}

EIoBuffer* EIoBufferWrapper::putChar(int index, char value) {
	buf_->putChar(index, value);
	return this;
}

short EIoBufferWrapper::getShort() {
	return buf_->getShort();
}

EIoBuffer* EIoBufferWrapper::putShort(short value) {
	buf_->putShort(value);
	return this;
}

short EIoBufferWrapper::getShort(int index) {
	return buf_->getShort(index);
}

EIoBuffer* EIoBufferWrapper::putShort(int index, short value) {
	buf_->putShort(index, value);
	return this;
}

int EIoBufferWrapper::getInt() {
	return buf_->getInt();
}

EIoBuffer* EIoBufferWrapper::putInt(int value) {
	buf_->putInt(value);
	return this;
}

int EIoBufferWrapper::getInt(int index) {
	return buf_->getInt(index);
}

EIoBuffer* EIoBufferWrapper::putInt(int index, int value) {
	buf_->putInt(index, value);
	return this;
}

llong EIoBufferWrapper::getLLong() {
	return buf_->getLLong();
}

EIoBuffer* EIoBufferWrapper::putLLong(llong value) {
	buf_->putLLong(value);
	return this;
}

llong EIoBufferWrapper::getLLong(int index) {
	return buf_->getLLong(index);
}

EIoBuffer* EIoBufferWrapper::putLLong(int index, llong value) {
	buf_->putLLong(index, value);
	return this;
}

float EIoBufferWrapper::getFloat() {
	return buf_->getFloat();
}

EIoBuffer* EIoBufferWrapper::putFloat(float value) {
	buf_->putFloat(value);
	return this;
}

float EIoBufferWrapper::getFloat(int index) {
	return buf_->getFloat(index);
}

EIoBuffer* EIoBufferWrapper::putFloat(int index, float value) {
	buf_->putFloat(index, value);
	return this;
}

double EIoBufferWrapper::getDouble() {
	return buf_->getDouble();
}

EIoBuffer* EIoBufferWrapper::putDouble(double value) {
	buf_->putDouble(value);
	return this;
}

double EIoBufferWrapper::getDouble(int index) {
	return buf_->getDouble(index);
}

EIoBuffer* EIoBufferWrapper::putDouble(int index, double value) {
	buf_->putDouble(index, value);
	return this;
}

EString EIoBufferWrapper::getString() {
	return buf_->getString();
}

EString EIoBufferWrapper::getString(int fieldSize) {
	return buf_->getString(fieldSize);
}

EIoBuffer* EIoBufferWrapper::putString(const char* val) {
	buf_->putString(val);
	return this;
}

EIoBuffer* EIoBufferWrapper::putString(const char* val, int fieldSize) {
	buf_->putString(val, fieldSize);
	return this;
}

EIoBuffer* EIoBufferWrapper::fill(byte value, int size) {
	buf_->fill(value, size);
	return this;
}

EIoBuffer* EIoBufferWrapper::fillAndReset(byte value, int size) {
	buf_->fillAndReset(value, size);
	return this;
}

EInputStream* EIoBufferWrapper::asInputStream() {
	return buf_->asInputStream();
}

EOutputStream* EIoBufferWrapper::asOutputStream() {
	return buf_->asOutputStream();
}

} /* namespace eio */
} /* namespace efc */
