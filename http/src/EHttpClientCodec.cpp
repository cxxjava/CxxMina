/*
 * EHttpClientCodec.cpp
 *
 *  Created on: 2017-1-4
 *      Author: cxxjava@163.com
 */

#include "../inc/EHttpClientCodec.hh"
#include "../inc/EHttpClientEncoder.hh"
#include "../inc/EHttpClientDecoder.hh"

namespace efc {
namespace eio {

sp<EString> EHttpClientCodec::DECODER_STATE_ATT(new EString("http.ds"));
sp<EString> EHttpClientCodec::PARTIAL_HEAD_ATT(new EString("http.ph"));
EProtocolEncoder* EHttpClientCodec::encoder = new EHttpClientEncoder();
EProtocolDecoder* EHttpClientCodec::decoder = new EHttpClientDecoder();

} /* namespace eio */
} /* namespace efc */
