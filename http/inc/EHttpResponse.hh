/*
 * EHttpResponse.hh
 *
 *  Created on: 2017-1-4
 *      Author: cxxjava@163.com
 */

#ifndef EHTTPRESPONSE_HH_
#define EHTTPRESPONSE_HH_

#include "./EHttpMessage.hh"
#include "./EHttpStatus.hh"

namespace efc {
namespace eio {

/**
 * An HTTP response to an HTTP request
 *
 * @author The Apache MINA Project (dev@mina.apache.org)
 *
 */

interface EHttpResponse: virtual public EHttpMessage {
	virtual ~EHttpResponse(){}

	/**
	 * The HTTP status code for the HTTP response (e.g. 200 for OK, 404 for not found, etc..)
	 *
	 * @return the status of the HTTP response
	 */
	virtual int getStatus() = 0;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EHTTPRESPONSE_HH_ */
