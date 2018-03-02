/*
 * EHttpResponseImpl.hh
 *
 *  Created on: 2017-1-4
 *      Author: cxxjava@163.com
 */

#ifndef EHTTPRESPONSEIMPL_HH_
#define EHTTPRESPONSEIMPL_HH_

#include "./EHttpResponse.hh"
#include "./EHttpVersion.hh"

namespace efc {
namespace eio {

/**
 * An HTTP message, the ancestor of HTTP request &amp; response.
 *
 * @author The Apache MINA Project (dev@mina.apache.org)
 */

class EHttpResponseImpl: virtual public EHttpResponse {
public:
	virtual ~EHttpResponseImpl();

	EHttpResponseImpl(EString* version, int status, sp<EMap<EString*, EString*> > headers);

	virtual const char* getProtocolVersion();

	virtual sp<EString> getContentType();

	virtual boolean isKeepAlive();

	virtual sp<EString> getHeader(const char* name);

	virtual boolean containsHeader(const char* name);

	virtual sp<EMap<EString*, EString*> > getHeaders();

	virtual int getStatus();

	virtual EString toString();

private:
	EString version;

	int status;

	sp<EMap<EString*, EString*> > headers;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EHTTPRESPONSEIMPL_HH_ */
