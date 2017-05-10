/*
 * EDecoderState.hh
 *
 *  Created on: 2017-1-4
 *      Author: cxxjava@163.com
 */

#ifndef EDECODERSTATE_HH_
#define EDECODERSTATE_HH_

#include "Efc.hh"

namespace efc {
namespace eio {

/**
 *
 */

enum EDecoderState {
	NEW, // waiting for a new HTTP requests, the session is new of last request was completed
	HEAD, // accumulating the HTTP request head (everything before the body)
	BODY // receiving HTTP body slices
};

} /* namespace eio */
} /* namespace efc */
#endif /* EDECODERSTATE_HH_ */
