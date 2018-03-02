/*
 * EIoBufferWrapper.hh
 *
 *  Created on: 2016-7-6
 *      Author: cxxjava@lotuseed.com
 */

#ifndef EIOBUFFERWRAPPER_HH_
#define EIOBUFFERWRAPPER_HH_

#include "./EIoBuffer.hh"

namespace efc {
namespace eio {

/**
 * A {@link IoBuffer} that wraps a buffer and proxies any operations to it.
 * <p>
 * You can think this class like a {@link FilterOutputStream}.  All operations
 * are proxied by default so that you can extend this class and override existing
 * operations selectively.  You can introduce new operations, too.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class EIoBufferWrapper: public EIoBuffer {
public:
	virtual ~EIoBufferWrapper();

	/**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* duplicate();

	/**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* slice();

	/**
	 * {@inheritDoc}
	 */
	virtual void free();

	/**
	 * {@inheritDoc}
	 */
	virtual EIOByteBuffer* buf();

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isDerived();

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isReadOnly();

	/**
	 * {@inheritDoc}
	 */
	virtual int minimumCapacity();

	/**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* minimumCapacity(int minimumCapacity);

	/**
	 * {@inheritDoc}
	 */
	virtual int capacity();

	/**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* capacity(int newCapacity);

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isAutoExpand();

	/**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* setAutoExpand(boolean autoExpand);

	/**
	 * {@inheritDoc}
	 */
	virtual boolean isAutoShrink();

	/**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* setAutoShrink(boolean autoShrink);

	/**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* expand(int expectedRemaining);

	/**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* expand(int position, int expectedRemaining);

	/**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* shrink();

	/**
	 * {@inheritDoc}
	 */
	virtual int position();

	/**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* position(int newPosition);

	/**
	 * {@inheritDoc}
	 */
	virtual int limit();

	/**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* limit(int newLimit);

	/**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* mark();

	/**
	 * {@inheritDoc}
	 */
	virtual int markValue();

	/**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* reset();

	/**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* clear();

	/**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* sweep();

	/**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* sweep(byte value);

	/**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* flip();

	/**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* rewind();

	/**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* compact();

	/**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* skip(int size);

	/**
	 * {@inheritDoc}
	 */
	virtual int indexOf(byte b);

	/**
	 * {@inheritDoc}
	 */
	virtual int remaining();

	/**
	 * {@inheritDoc}
	 */
	virtual boolean hasRemaining();

	/**
	 * {@inheritDoc}
	 */
	virtual EString getHexdump(int lengthLimit=16);

	/**
	 * {@inheritDoc}
	 */
	virtual int compareTo(EIoBuffer* that);

	/**
	 * {@inheritDoc}
	 */
	virtual EString toString();

	/**
	 * {@inheritDoc}
	 */
	virtual void* address();

	/**
	 * {@inheritDoc}
	 */
	virtual void* current();

public:
	/**
	 * {@inheritDoc}
	 */
	virtual byte get();

    /**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* put(byte b);

    /**
	 * {@inheritDoc}
	 */
	virtual byte get(int index);

    /**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* put(int index, byte b);

    /**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* get(void* dst, int size, int length);

    /**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* get(EA<byte>* dst);

    /**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* getSlice(int index, int length); //!

    /**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* getSlice(int length); //!

    /**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* put(EIOByteBuffer* src);

    /**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* put(EIoBuffer* src);

    /**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* put(const void* src, int length);

    /**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* put(EA<byte>* src);

    /**
	 * {@inheritDoc}
	 */
	virtual char getChar();

    /**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* putChar(char value);

    /**
	 * {@inheritDoc}
	 */
	virtual char getChar(int index);

    /**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* putChar(int index, char value);

    /**
	 * {@inheritDoc}
	 */
	virtual short getShort();

    /**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* putShort(short value);

    /**
	 * {@inheritDoc}
	 */
	virtual short getShort(int index);

    /**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* putShort(int index, short value);

    /**
	 * {@inheritDoc}
	 */
	virtual int getInt();

    /**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* putInt(int value);

    /**
	 * {@inheritDoc}
	 */
	virtual int getInt(int index);

    /**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* putInt(int index, int value);

    /**
	 * {@inheritDoc}
	 */
	virtual llong getLLong();

    /**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* putLLong(llong value);

    /**
	 * {@inheritDoc}
	 */
	virtual llong getLLong(int index);

    /**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* putLLong(int index, llong value);

    /**
	 * {@inheritDoc}
	 */
	virtual float getFloat();

    /**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* putFloat(float value);

    /**
	 * {@inheritDoc}
	 */
	virtual float getFloat(int index);

    /**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* putFloat(int index, float value);

    /**
	 * {@inheritDoc}
	 */
	virtual double getDouble();

    /**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* putDouble(double value);

    /**
	 * {@inheritDoc}
	 */
	virtual double getDouble(int index);

    /**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* putDouble(int index, double value);

    /**
	 * {@inheritDoc}
	 */
	virtual EString getString();

    /**
	 * {@inheritDoc}
	 */
	virtual EString getString(int fieldSize);

    /**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* putString(const char* val);

    /**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* putString(const char* val, int fieldSize);

	/**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* fill(byte value, int size);

	/**
	 * {@inheritDoc}
	 */
	virtual EIoBuffer* fillAndReset(byte value, int size);

public:
    /**
	 * {@inheritDoc}
	 */
	virtual EInputStream* asInputStream();

    /**
	 * {@inheritDoc}
	 */
	virtual EOutputStream* asOutputStream();

protected:
	/**
	 * Create a new instance.
	 * @param buf the buffer to be proxied
	 */
	EIoBufferWrapper(EIoBuffer* buf);

	/**
	 * @return the parent buffer that this buffer wrapped.
	 */
	EIoBuffer* getParentBuffer();

private:
	/**
	 * The buffer proxied by this proxy.
	 */
	EIoBuffer* buf_;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EIOBUFFERWRAPPER_HH_ */
