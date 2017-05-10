/*
 * EHttpStatus.cpp
 *
 *  Created on: 2017-1-4
 *      Author: cxxjava@163.com
 */

#include "EHttpStatus.hh"

namespace efc {
namespace eio {

EHttpStatus EHttpStatus::INFORMATIONAL_CONTINUE(100, "HTTP/1.1 100 Continue");
EHttpStatus EHttpStatus::INFORMATIONAL_SWITCHING_PROTOCOLS(101, "HTTP/1.1 101 Swtiching Protocols");
EHttpStatus EHttpStatus::SUCCESS_OK(200, "HTTP/1.1 200 OK");
EHttpStatus EHttpStatus::SUCCESS_CREATED(201, "HTTP/1.1 201 Created");
EHttpStatus EHttpStatus::SUCCESS_ACCEPTED(202, "HTTP/1.1 202 Accepted");
EHttpStatus EHttpStatus::SUCCESS_NON_AUTHORATIVE_INFORMATION(203, "HTTP/1.1 203 Non-Authoritative Information");
EHttpStatus EHttpStatus::SUCCESS_NO_CONTENT(204, "HTTP/1.1 204 No Content");
EHttpStatus EHttpStatus::SUCCESS_RESET_CONTENT(205, "HTTP/1.1 205 Reset Content");
EHttpStatus EHttpStatus::SUCCESS_PARTIAL_CONTENT(206, "HTTP/1.1 206 Partial Content");
EHttpStatus EHttpStatus::REDIRECTION_MULTIPLE_CHOICES(300, "HTTP/1.1 300 Multiple Choices");
EHttpStatus EHttpStatus::REDIRECTION_MOVED_PERMANENTLY(301, "HTTP/1.1 301 Moved Permanently");
EHttpStatus EHttpStatus::REDIRECTION_FOUND(302, "HTTP/1.1 302 Found");
EHttpStatus EHttpStatus::REDIRECTION_SEE_OTHER(303, "HTTP/1.1 303 See Other");
EHttpStatus EHttpStatus::REDIRECTION_NOT_MODIFIED(304, "HTTP/1.1 304 Not Modified");
EHttpStatus EHttpStatus::REDIRECTION_USE_PROXY(305, "HTTP/1.1 305 Use Proxy");
EHttpStatus EHttpStatus::REDIRECTION_TEMPORARILY_REDIRECT(307, "HTTP/1.1 307 Temporary Redirect");
EHttpStatus EHttpStatus::CLIENT_ERROR_BAD_REQUEST(400, "HTTP/1.1 400 Bad Request");
EHttpStatus EHttpStatus::CLIENT_ERROR_UNAUTHORIZED(401, "HTTP/1.1 401 Unauthorized");
EHttpStatus EHttpStatus::CLIENT_ERROR_FORBIDDEN(403, "HTTP/1.1 403 Forbidden");
EHttpStatus EHttpStatus::CLIENT_ERROR_NOT_FOUND(404, "HTTP/1.1 404 Not Found");
EHttpStatus EHttpStatus::CLIENT_ERROR_METHOD_NOT_ALLOWED(405, "HTTP/1.1 405 Method Not Allowed");
EHttpStatus EHttpStatus::CLIENT_ERROR_NOT_ACCEPTABLE(406, "HTTP/1.1 406 Not Acceptable");
EHttpStatus EHttpStatus::CLIENT_ERROR_PROXY_AUTHENTICATION_REQUIRED(407, "HTTP/1.1 407 Proxy Authentication Required");
EHttpStatus EHttpStatus::CLIENT_ERROR_REQUEST_TIMEOUT(408, "HTTP/1.1 408 Request Timeout");
EHttpStatus EHttpStatus::CLIENT_ERROR_CONFLICT(409, "HTTP/1.1 409 Conflict");
EHttpStatus EHttpStatus::CLIENT_ERROR_GONE(410, "HTTP/1.1 410 Gone");
EHttpStatus EHttpStatus::CLIENT_ERROR_LENGTH_REQUIRED(411, "HTTP/1.1 411 Length Required");
EHttpStatus EHttpStatus::CLIENT_ERROR_PRECONDITION_FAILED(412, "HTTP/1.1 412 Precondition Failed");
EHttpStatus EHttpStatus::CLIENT_ERROR_REQUEST_ENTITY_TOO_LARGE(413, "HTTP/1.1 413 Request Entity Too Large");
EHttpStatus EHttpStatus::CLIENT_ERROR_REQUEST_URI_TOO_LONG(414, "HTTP/1.1 414 Request-URI Too Long");
EHttpStatus EHttpStatus::CLIENT_ERROR_UNSUPPORTED_MEDIA_TYPE(415, "HTTP/1.1 415 Unsupported Media Type");
EHttpStatus EHttpStatus::CLIENT_ERROR_REQUESTED_RANGE_NOT_SATISFIABLE(416, "HTTP/1.1 416 Requested Range Not Satisfiable");
EHttpStatus EHttpStatus::CLIENT_ERROR_EXPECTATION_FAILED(417, "HTTP/1.1 417 Expectation Failed");
EHttpStatus EHttpStatus::SERVER_ERROR_INTERNAL_SERVER_ERROR(500, "HTTP/1.1 500 Internal Server Error");
EHttpStatus EHttpStatus::SERVER_ERROR_NOT_IMPLEMENTED(501, "HTTP/1.1 501 Not Implemented");
EHttpStatus EHttpStatus::SERVER_ERROR_BAD_GATEWAY(502, "HTTP/1.1 502 Bad Gateway");
EHttpStatus EHttpStatus::SERVER_ERROR_SERVICE_UNAVAILABLE(503, "HTTP/1.1 503 Service Unavailable");
EHttpStatus EHttpStatus::SERVER_ERROR_GATEWAY_TIMEOUT(504, "HTTP/1.1 504 Gateway Timeout");
EHttpStatus EHttpStatus::SERVER_ERROR_HTTP_VERSION_NOT_SUPPORTED(505, "HTTP/1.1 505 HTTP Version Not Supported");

} /* namespace eio */
} /* namespace efc */
