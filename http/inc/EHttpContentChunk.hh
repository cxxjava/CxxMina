/*
 * EHttpContentChunk.hh
 *
 *  Created on: 2017-1-4
 *      Author: cxxjava@163.com
 */

#ifndef EHTTPCONTENTCHUNK_HH_
#define EHTTPCONTENTCHUNK_HH_

#include "Efc.hh"

namespace efc {
namespace eio {

/**
 * An HTTP message, the ancestor of HTTP request &amp; response.
 *
 * @author The Apache MINA Project (dev@mina.apache.org)
 */

interface EHttpContentChunk: virtual public EObject {
	virtual ~EHttpContentChunk(){}

	virtual EList<EIoBuffer*>* getContent() = 0;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EHTTPCONTENTCHUNK_HH_ */
