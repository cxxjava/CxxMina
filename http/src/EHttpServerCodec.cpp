/*
 * EHttpServerCodec.cpp
 *
 *  Created on: 2017-1-4
 *      Author: cxxjava@163.com
 */

#include "../inc/EHttpServerCodec.hh"
#include "../inc/EHttpServerEncoder.hh"
#include "../inc/EHttpServerDecoder.hh"

namespace efc {
namespace eio {

sp<EString> EHttpServerCodec::DECODER_STATE_ATT(new EString("http.ds"));
sp<EString> EHttpServerCodec::PARTIAL_HEAD_ATT(new EString("http.ph"));
EProtocolEncoder* EHttpServerCodec::encoder = new EHttpServerEncoder();
EProtocolDecoder* EHttpServerCodec::decoder = new EHttpServerDecoder();

} /* namespace eio */
} /* namespace efc */
