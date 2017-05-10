/*
 * EHttpServerCodec.cpp
 *
 *  Created on: 2017-1-4
 *      Author: cxxjava@163.com
 */

#include "EHttpServerCodec.hh"
#include "EHttpServerEncoder.hh"
#include "EHttpServerDecoder.hh"

namespace efc {
namespace eio {

sp<EString> EHttpServerCodec::DECODER_STATE_ATT(new EString("http.ds"));
sp<EString> EHttpServerCodec::PARTIAL_HEAD_ATT(new EString("http.ph"));
EProtocolEncoder* EHttpServerCodec::encoder = new EHttpServerEncoder();
EProtocolDecoder* EHttpServerCodec::decoder = new EHttpServerDecoder();

} /* namespace eio */
} /* namespace efc */
