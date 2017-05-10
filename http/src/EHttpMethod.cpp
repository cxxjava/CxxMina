/*
 * EHttpMethod.cpp
 *
 *  Created on: 2017-1-4
 *      Author: cxxjava@163.com
 */

#include "EHttpMethod.hh"

namespace efc {
namespace eio {

EHttpMethod EHttpMethod::GET("GET");
EHttpMethod EHttpMethod::HEAD("HEAD");
EHttpMethod EHttpMethod::POST("POST");
EHttpMethod EHttpMethod::PUT("PUT");
EHttpMethod EHttpMethod::DELETE("DELETE");
EHttpMethod EHttpMethod::OPTIONS("OPTIONS");
EHttpMethod EHttpMethod::TRACE("TRACE");
EHttpMethod EHttpMethod::CONNECT("CONNECT");

} /* namespace eio */
} /* namespace efc */
