/*
 * EHttpRequestImpl.hh
 *
 *  Created on: 2017-1-4
 *      Author: cxxjava@163.com
 */

#ifndef EHTTPREQUESTIMPL_HH_
#define EHTTPREQUESTIMPL_HH_

#include "./EHttpRequest.hh"

namespace efc {
namespace eio {

class EHttpRequestImpl: public EHttpRequest {
public:
	virtual ~EHttpRequestImpl();

	EHttpRequestImpl(EString* version, EString* method,
			EString* requestedPath, EString* queryString,
			sp<EMap<EString*, EString*> > headers);

	virtual const char* getProtocolVersion();
	virtual sp<EString> getContentType();
	virtual boolean isKeepAlive();
	virtual sp<EString> getHeader(const char* name);
	virtual boolean containsHeader(const char* name);
	virtual sp<EMap<EString*, EString*> > getHeaders();
	virtual boolean containsParameter(const char* name);
	virtual sp<EString> getParameter(const char* name);
	virtual sp<EString> getQueryString();
	virtual sp<EMap<EString*, EList<EString*>*> > getParameters();
	virtual const char* getMethod();
	virtual sp<EString> getRequestPath();

	virtual EString toString();

private:
	EString version;

	EString method;

	sp<EString> requestedPath;

	sp<EString> queryString;

	sp<EMap<EString*, EString*> > headers;
};

} /* namespace eio */
} /* namespace efc */
#endif /* EHTTPREQUESTIMPL_HH_ */
