/*
 * EHttpStatus.hh
 *
 *  Created on: 2017-1-4
 *      Author: cxxjava@163.com
 */

#ifndef EHTTPSTATUS_HH_
#define EHTTPSTATUS_HH_

#include "Efc.hh"

namespace efc {
namespace eio {

/**
 * An <code>Enumeration</code> of all known HTTP status codes.
 */

class EHttpStatus {
public:
	/**
	 * 100 - Continue
	 */
	static EHttpStatus INFORMATIONAL_CONTINUE;//(100, "HTTP/1.1 100 Continue"),

	/**
	 * 101 - Switching Protocols
	 */
	static EHttpStatus INFORMATIONAL_SWITCHING_PROTOCOLS;//(101, "HTTP/1.1 101 Swtiching Protocols"),
	/**
	 * 200 - OK
	 */
	static EHttpStatus SUCCESS_OK;//(200, "HTTP/1.1 200 OK"),
	/**
	 * 201 - Created
	 */
	static EHttpStatus SUCCESS_CREATED;//(201, "HTTP/1.1 201 Created"),
	/**
	 * 202 - Accepted
	 */
	static EHttpStatus SUCCESS_ACCEPTED;//(202, "HTTP/1.1 202 Accepted"),
	/**
	 * 203 - Non-Authoritative Information
	 */
	static EHttpStatus SUCCESS_NON_AUTHORATIVE_INFORMATION;//(203, "HTTP/1.1 203 Non-Authoritative Information"),
	/**
	 * 204 - No Content
	 */
	static EHttpStatus SUCCESS_NO_CONTENT;//(204, "HTTP/1.1 204 No Content"),
	/**
	 * 205 - Reset Content
	 */
	static EHttpStatus SUCCESS_RESET_CONTENT;//(205, "HTTP/1.1 205 Reset Content"),
	/**
	 * 206 - Created
	 */
	static EHttpStatus SUCCESS_PARTIAL_CONTENT;//(206, "HTTP/1.1 206 Partial Content"),

	/**
	 * 300 - Multiple Choices
	 */
	static EHttpStatus REDIRECTION_MULTIPLE_CHOICES;//(300, "HTTP/1.1 300 Multiple Choices"),
	/**
	 * 301 - Moved Permanently
	 */
	static EHttpStatus REDIRECTION_MOVED_PERMANENTLY;//(301, "HTTP/1.1 301 Moved Permanently"),
	/**
	 * 302 - Found / Moved Temporarily
	 */
	static EHttpStatus REDIRECTION_FOUND;//(302, "HTTP/1.1 302 Found"),
	/**
	 * 303 - See Others
	 */
	static EHttpStatus REDIRECTION_SEE_OTHER;//(303, "HTTP/1.1 303 See Other"),
	/**
	 * 304 - Not Modified
	 */
	static EHttpStatus REDIRECTION_NOT_MODIFIED;//(304, "HTTP/1.1 304 Not Modified"),
	/**
	 * 305 - Use Proxy
	 */
	static EHttpStatus REDIRECTION_USE_PROXY;//(305, "HTTP/1.1 305 Use Proxy"),
	/**
	 * 307 - Temporary Redirect
	 */
	static EHttpStatus REDIRECTION_TEMPORARILY_REDIRECT;//(307, "HTTP/1.1 307 Temporary Redirect"),

	/**
	 * 400 - Bad Request
	 */
	static EHttpStatus CLIENT_ERROR_BAD_REQUEST;//(400, "HTTP/1.1 400 Bad Request"),
	/**
	 * 401 - Unauthorized
	 */
	static EHttpStatus CLIENT_ERROR_UNAUTHORIZED;//(401, "HTTP/1.1 401 Unauthorized"),
	/**
	 * 403 - Forbidden
	 */
	static EHttpStatus CLIENT_ERROR_FORBIDDEN;//(403, "HTTP/1.1 403 Forbidden"),
	/**
	 * 404 - Not Found
	 */
	static EHttpStatus CLIENT_ERROR_NOT_FOUND;//(404, "HTTP/1.1 404 Not Found"),
	/**
	 * 405 - Method Not Allowed
	 */
	static EHttpStatus CLIENT_ERROR_METHOD_NOT_ALLOWED;//(405, "HTTP/1.1 405 Method Not Allowed"),
	/**
	 * 406 - Not Acceptable
	 */
	static EHttpStatus CLIENT_ERROR_NOT_ACCEPTABLE;//(406, "HTTP/1.1 406 Not Acceptable"),
	/**
	 * 407 - Proxy Authentication Required
	 */
	static EHttpStatus CLIENT_ERROR_PROXY_AUTHENTICATION_REQUIRED;//(407, "HTTP/1.1 407 Proxy Authentication Required"),
	/**
	 * 408 - Request Timeout
	 */
	static EHttpStatus CLIENT_ERROR_REQUEST_TIMEOUT;//(408, "HTTP/1.1 408 Request Timeout"),
	/**
	 * 409 - Conflict
	 */
	static EHttpStatus CLIENT_ERROR_CONFLICT;//(409, "HTTP/1.1 409 Conflict"),
	/**
	 * 410 - Gone
	 */
	static EHttpStatus CLIENT_ERROR_GONE;//(410, "HTTP/1.1 410 Gone"),
	/**
	 * 411 - Length Required
	 */
	static EHttpStatus CLIENT_ERROR_LENGTH_REQUIRED;//(411, "HTTP/1.1 411 Length Required"),
	/**
	 * 412 - Precondition Failed
	 */
	static EHttpStatus CLIENT_ERROR_PRECONDITION_FAILED;//(412, "HTTP/1.1 412 Precondition Failed"),
	/**
	 * 413 - Request Entity Too Large
	 */
	static EHttpStatus CLIENT_ERROR_REQUEST_ENTITY_TOO_LARGE;//(413, "HTTP/1.1 413 Request Entity Too Large"),
	/**
	 * 414 - Bad Request
	 */
	static EHttpStatus CLIENT_ERROR_REQUEST_URI_TOO_LONG;//(414, "HTTP/1.1 414 Request-URI Too Long"),
	/**
	 * 415 - Unsupported Media Type
	 */
	static EHttpStatus CLIENT_ERROR_UNSUPPORTED_MEDIA_TYPE;//(415, "HTTP/1.1 415 Unsupported Media Type"),
	/**
	 * 416 - Requested Range Not Satisfiable
	 */
	static EHttpStatus CLIENT_ERROR_REQUESTED_RANGE_NOT_SATISFIABLE;//(416, "HTTP/1.1 416 Requested Range Not Satisfiable"),
	/**
	 * 417 - Expectation Failed
	 */
	static EHttpStatus CLIENT_ERROR_EXPECTATION_FAILED;//(417, "HTTP/1.1 417 Expectation Failed"),

	/**
	 * 500 - Internal Server Error
	 */
	static EHttpStatus SERVER_ERROR_INTERNAL_SERVER_ERROR;//(500, "HTTP/1.1 500 Internal Server Error"),
	/**
	 * 501 - Not Implemented
	 */
	static EHttpStatus SERVER_ERROR_NOT_IMPLEMENTED;//(501, "HTTP/1.1 501 Not Implemented"),
	/**
	 * 502 - Bad Gateway
	 */
	static EHttpStatus SERVER_ERROR_BAD_GATEWAY;//(502, "HTTP/1.1 502 Bad Gateway"),
	/**
	 * 503 - Service Unavailable
	 */
	static EHttpStatus SERVER_ERROR_SERVICE_UNAVAILABLE;//(503, "HTTP/1.1 503 Service Unavailable"),
	/**
	 * 504 - Gateway Timeout
	 */
	static EHttpStatus SERVER_ERROR_GATEWAY_TIMEOUT;//(504, "HTTP/1.1 504 Gateway Timeout"),
	/**
	 * 505 - HTTP Version Not Supported
	 */
	static EHttpStatus SERVER_ERROR_HTTP_VERSION_NOT_SUPPORTED;//(505, "HTTP/1.1 505 HTTP Version Not Supported");

public:
	/**
	 * Retrieve the status code for this instance.
	 *
	 * @return the status code.
	 */
	int code() {
		return code_;
	}

	/**
	 * Retrieve the status line for this instance.
	 *
	 * @return the status line.
	 */
	EString line() {
		return line_ + "\r\n";
	}

	/**
	 *
	 */
	static EHttpStatus* valueOf(int code) {
		switch (code) {
		case 100:
			return &INFORMATIONAL_CONTINUE;
		case 101:
			return &INFORMATIONAL_SWITCHING_PROTOCOLS;
		case 200:
			return &SUCCESS_OK;
		case 201:
			return &SUCCESS_CREATED;
		case 202:
			return &SUCCESS_ACCEPTED;
		case 203:
			return &SUCCESS_NON_AUTHORATIVE_INFORMATION;
		case 204:
			return &SUCCESS_NO_CONTENT;
		case 205:
			return &SUCCESS_RESET_CONTENT;
		case 206:
			return &SUCCESS_PARTIAL_CONTENT;
		case 300:
			return &REDIRECTION_MULTIPLE_CHOICES;
		case 301:
			return &REDIRECTION_MOVED_PERMANENTLY;
		case 302:
			return &REDIRECTION_FOUND;
		case 303:
			return &REDIRECTION_SEE_OTHER;
		case 304:
			return &REDIRECTION_NOT_MODIFIED;
		case 305:
			return &REDIRECTION_USE_PROXY;
		case 307:
			return &REDIRECTION_TEMPORARILY_REDIRECT;
		case 400:
			return &CLIENT_ERROR_BAD_REQUEST;
		case 401:
			return &CLIENT_ERROR_UNAUTHORIZED;
		case 403:
			return &CLIENT_ERROR_FORBIDDEN;
		case 404:
			return &CLIENT_ERROR_NOT_FOUND;
		case 405:
			return &CLIENT_ERROR_METHOD_NOT_ALLOWED;
		case 406:
			return &CLIENT_ERROR_NOT_ACCEPTABLE;
		case 407:
			return &CLIENT_ERROR_PROXY_AUTHENTICATION_REQUIRED;
		case 408:
			return &CLIENT_ERROR_REQUEST_TIMEOUT;
		case 409:
			return &CLIENT_ERROR_CONFLICT;
		case 410:
			return &CLIENT_ERROR_GONE;
		case 411:
			return &CLIENT_ERROR_LENGTH_REQUIRED;
		case 412:
			return &CLIENT_ERROR_PRECONDITION_FAILED;
		case 413:
			return &CLIENT_ERROR_REQUEST_ENTITY_TOO_LARGE;
		case 414:
			return &CLIENT_ERROR_REQUEST_URI_TOO_LONG;
		case 415:
			return &CLIENT_ERROR_UNSUPPORTED_MEDIA_TYPE;
		case 416:
			return &CLIENT_ERROR_REQUESTED_RANGE_NOT_SATISFIABLE;
		case 417:
			return &CLIENT_ERROR_EXPECTATION_FAILED;
		case 500:
			return &SERVER_ERROR_INTERNAL_SERVER_ERROR;
		case 501:
			return &SERVER_ERROR_NOT_IMPLEMENTED;
		case 502:
			return &SERVER_ERROR_BAD_GATEWAY;
		case 503:
			return &SERVER_ERROR_SERVICE_UNAVAILABLE;
		case 504:
			return &SERVER_ERROR_GATEWAY_TIMEOUT;
		case 505:
			return &SERVER_ERROR_HTTP_VERSION_NOT_SUPPORTED;
		default:
			return &SERVER_ERROR_INTERNAL_SERVER_ERROR;
		}
	}

private:
	/** The code associated with this status, for example "404" for "Not Found". */
	int code_;

	/**
	 * The line associated with this status, "HTTP/1.1 501 Not Implemented".
	 */
	EString line_;

	/**
	 * Create an instance of this type.
	 *
	 * @param code the status code.
	 * @param phrase the associated phrase.
	 */
	EHttpStatus(int code, const char* phrase) {
		this->code_ = code;
		this->line_ = phrase;
	}
};

} /* namespace eio */
} /* namespace efc */
#endif /* EHTTPSTATUS_HH_ */
