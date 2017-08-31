/*
 * EProtocolDecoderException.cpp
 *
 *  Created on: 2016-5-20
 *      Author: cxxjava@163.com
 */

#include "../inc/EProtocolDecoderException.hh"

namespace efc {
namespace eio {

EProtocolDecoderException::~EProtocolDecoderException() {
	delete hexDump;
	delete message;
}

EProtocolDecoderException::EProtocolDecoderException(const EException& that) :
		EProtocolCodecException(((EException*)&that)->getSourceFile(),
				((EException*)&that)->getSourceLine(),
				((EException*)&that)->getErrno()), hexDump(null), message(null) {
	//
}

EProtocolDecoderException::EProtocolDecoderException(const EProtocolDecoderException& that) :
		EProtocolCodecException(((EProtocolDecoderException*)&that)->getSourceFile(),
				((EProtocolDecoderException*)&that)->getSourceLine(),
				((EProtocolDecoderException*)&that)->getErrno()), hexDump(null), message(null) {
	EProtocolDecoderException* t = (EProtocolDecoderException*)&that;

	const char* ts = t->getHexdump();
	if (ts) {
		hexDump = new EString(ts);
	}

	message = new EString((dynamic_cast<EProtocolCodecException*>(t))->getMessage());
}

EProtocolDecoderException::EProtocolDecoderException(const char *_file_, int _line_, int errn) :
		EProtocolCodecException(_file_, _line_, errn), hexDump(null), message(null) {

}

EProtocolDecoderException::EProtocolDecoderException(const char *_file_, int _line_, const char *s, int errn) :
		EProtocolCodecException(_file_, _line_, s, errn), hexDump(null), message(null) {

}

EProtocolDecoderException::EProtocolDecoderException(const char *_file_, int _line_, EThrowable* cause) :
    EProtocolCodecException(_file_, _line_, cause), hexDump(null), message(null) {
        
}
    
EProtocolDecoderException::EProtocolDecoderException(const char *_file_, int _line_, const char *s, EThrowable* cause) :
    EProtocolCodecException(_file_, _line_, s, cause), hexDump(null), message(null) {
        
}

const char* EProtocolDecoderException::getHexdump() {
	return hexDump ? hexDump->c_str() : null;
}

void EProtocolDecoderException::setHexdump(const char* hexdump) {
	ES_ASSERT(!this->hexDump);
	this->hexDump = new EString(hexdump);

	if (!message) {
		message = new EString(EProtocolCodecException::getMessage());
	}

	(*message) << (message->length()>0 ? " " : "") << "(Hexdump: " << hexDump->c_str() << ")";
}

const char* EProtocolDecoderException::getMessage() {
	if (!message) {
		message = new EString(EProtocolCodecException::getMessage());
	}

	return message->c_str();
}

} /* namespace eio */
} /* namespace efc */
