/*
 * EHttpDigestAuthLogicHandler.cpp
 *
 *  Created on: 2016-6-17
 *      Author: cxxjava@lotuseed.com
 */

#include "EHttpDigestAuthLogicHandler.hh"
#include "EHttpProxyConstants.hh"
#include "ERuntimeException.hh"
#include "EAuthenticationException.hh"
#include "EStringUtilities.hh"
#include "EProxyIoSession.hh"

#include <iconv.h> //iconv_xxx

namespace efc {
namespace eio {

//=============================================================================

class Converter {
public:
	Converter(const EString& out_encode, const EString& in_encode,
			bool ignore_error = false, size_t buf_size = 1024) :
			ignore_error_(ignore_error), buf_size_(buf_size) {
		if (buf_size == 0) {
			throw ERuntimeException(__FILE__, __LINE__, "buffer size must be greater than zero");
		}

		iconv_t conv = ::iconv_open(out_encode.c_str(), in_encode.c_str());
		if (conv == (iconv_t) - 1) {
            if (errno == EINVAL) {
            	EString msg("not supported from " + in_encode + " to "
						+ out_encode);
				throw ERuntimeException(__FILE__, __LINE__, msg.c_str());
            }
            else
				throw ERuntimeException(__FILE__, __LINE__, "unknown error");
		}
		iconv_ = conv;
	}

	~Converter() {
		::iconv_close(iconv_);
	}

	EString convert(const EString& input) const {
		// copy the string to a buffer as iconv function requires a non-const char
		// pointer.
		size_t src_size = input.length();
		EA<char> in_buf(src_size);
		char* src_ptr = in_buf.address();

		EString output;
		EA<char> buf(buf_size_);
		while (0 < src_size) {
			char* dst_ptr = buf.address();
			size_t dst_size = buf.length();
			size_t res = ::iconv(iconv_, &src_ptr, &src_size, &dst_ptr,
					&dst_size);
			if (res == (size_t) -1) {
				if (errno == E2BIG) {
					// ignore this error
				} else if (ignore_error_) {
					// skip character
					++src_ptr;
					--src_size;
				} else {
					check_convert_error();
				}
			}
			output.append(buf.address(), buf.length() - dst_size);
		}
		return output;
	}

private:
	void check_convert_error() const {
		switch (errno) {
		case EILSEQ:
			throw ERuntimeException(__FILE__, __LINE__, "invalid multibyte chars");
		case EINVAL:
			throw ERuntimeException(__FILE__, __LINE__, "invalid multibyte chars");
		default:
			throw ERuntimeException(__FILE__, __LINE__, "unknown error");
		}
	}

	iconv_t iconv_;
	boolean ignore_error_;
	const size_t buf_size_;
};

//=============================================================================

/**
 * Used to convert username-value, passwd or realm to 8859_1 encoding
 * if all chars in string are within the 8859_1 (Latin 1) encoding range.
 *
 * @param str a non-null String
 * @return a non-null String containing the 8859_1 encoded string
 * @throws UnsupportedEncodingException if we weren't able to decode using the ISO 8859_1 encoding
 */
static EString StringTo8859_1(EString utf8_str) THROWS(EUnsupportedEncodingException) {
	if (utf8_str.isEmpty()) {
		return "";
	}
	Converter iconv("8859_1", "UTF8");
	return iconv.convert(utf8_str);
}

//=============================================================================

/**
 * DigestUtilities.java - A class supporting the HTTP DIGEST authentication (see RFC 2617).
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since MINA 2.0.0-M3
 */
class DigestUtilities {
public:
	static sp<EString> SESSION_HA1;// = DigestUtilities.class + ".SessionHA1";

    /**
     * Computes the response to the DIGEST challenge.
     *
     * @param session the current session
     * @param map the map holding the directives sent by the proxy
     * @param method the HTTP verb
     * @param pwd the password
     * @param charsetName the name of the charset used for the challenge
     * @param body the html body to be hashed for integrity calculations
     * @return The response
     * @throws AuthenticationException if we weren't able to find a directive value in the map
     * @throws UnsupportedEncodingException If we weren't able to encode to ISO 8859_1 the username or realm,
     * or if we weren't able to encode the charsetName
     */
    static EString computeResponseValue(sp<EIoSession>& session, EHashMap<EString*, EString*>& map, const char* method,
    		const char* pwd, /*const char* charsetName,*/ const char* body) THROWS2(EAuthenticationException, EUnsupportedEncodingException) {

        sp<EA<byte> > hA1(new EA<byte>(ES_MD5_DIGEST_LEN));
        EString sb;
        boolean isMD5Sess = EStringUtilities::getDirectiveValue(map, "algorithm", false).equalsIgnoreCase("md5-sess");
        MD5_CTX c;

        if (!isMD5Sess || (session->getAttribute(SESSION_HA1.get()) == null)) {
            // Build A1
            sb.append(StringTo8859_1(EStringUtilities::getDirectiveValue(map, "username", true))).append(
                    ':');

            EString realm = StringTo8859_1(EStringUtilities::getDirectiveValue(map, "realm", false));

            if (!realm.isEmpty()) {
                sb.append(realm);
            }

            sb.append(':').append(pwd);

            if (isMD5Sess) {
                byte prehA1[ES_MD5_DIGEST_LEN] = {0};

                /* @see:
                synchronized (md5) {
                    md5.reset();
                    prehA1 = md5.digest(sb.toString().getBytes(charsetName));
                }
                */
                MD5_Init(&c);
                MD5_Update(&c, (const unsigned char *)sb.c_str(), sb.length());
				MD5_Final((es_uint8_t*)prehA1, &c);

                sb.setLength(0);// = new StringBuilder();
                sb.append(EString::toHexString(prehA1, sizeof(prehA1))); //sb.append(ByteUtilities.asHex(prehA1));
                sb.append(':').append(
                		StringTo8859_1(EStringUtilities::getDirectiveValue(map, "nonce", true)));
                sb.append(':').append(
                		StringTo8859_1(EStringUtilities::getDirectiveValue(map, "cnonce", true)));

                /*
                synchronized (md5) {
                    md5.reset();
                    hA1 = md5.digest(sb.toString().getBytes(charsetName));
                }
                */
                MD5_Init(&c);
				MD5_Update(&c, (const unsigned char *)sb.c_str(), sb.length());
				MD5_Final((es_uint8_t*)hA1->address(), &c);

                session->setAttribute(DigestUtilities::SESSION_HA1, hA1);
            } else {
            	/*
                synchronized (md5) {
                    md5.reset();
                    hA1 = md5.digest(sb.toString().getBytes(charsetName));
                }
                */
            	MD5_Init(&c);
				MD5_Update(&c, (const unsigned char *)sb.c_str(), sb.length());
				MD5_Final((es_uint8_t*)hA1->address(), &c);
            }
        } else {
            hA1 = dynamic_pointer_cast<EA<byte> >(session->getAttribute(SESSION_HA1.get()));
        }

        sb.setLength(0); sb.append(method); // = new StringBuilder(method);
        sb.append(':');
        sb.append(EStringUtilities::getDirectiveValue(map, "uri", false));

        EString qop = EStringUtilities::getDirectiveValue(map, "qop", false);

        if (qop.equalsIgnoreCase("auth-int")) {
            sp<EProxyIoSession> proxyIoSession = dynamic_pointer_cast<EProxyIoSession>(session->getAttribute(EProxyIoSession::PROXY_SESSION.get()));
            byte hEntity[ES_MD5_DIGEST_LEN] = {0};

            /*
            synchronized (md5) {
                md5.reset();
                hEntity = md5.digest(body.getBytes(proxyIoSession.getCharsetName()));
            }
            */
            MD5_Init(&c);
			MD5_Update(&c, (const unsigned char *)body, eso_strlen(body));
			MD5_Final((es_uint8_t*)hEntity, &c);

			sb.append(EString::toHexString(hEntity, sizeof(hEntity))); //sb.append(':').append(hEntity); //bug?
        }

        byte hA2[ES_MD5_DIGEST_LEN] = {0};
        /*
        synchronized (md5) {
            md5.reset();
            hA2 = md5.digest(sb.toString().getBytes(charsetName));
        }
        */
        MD5_Init(&c);
		MD5_Update(&c, (const unsigned char *)sb.c_str(), sb.length());
		MD5_Final((es_uint8_t*)hA2, &c);

        sb.setLength(0);// = new StringBuilder();
        sb.append(EString::toHexString(hA1->address(), hA1->length()));
        sb.append(':').append(EStringUtilities::getDirectiveValue(map, "nonce", true));
        sb.append(":00000001:");

        sb.append(EStringUtilities::getDirectiveValue(map, "cnonce", true));
        sb.append(':').append(qop).append(':');
        sb.append(EString::toHexString(hA2, sizeof(hA2)));

        byte hFinal[ES_MD5_DIGEST_LEN] = {0};

        /*
        synchronized (md5) {
            md5.reset();
            hFinal = md5.digest(sb.toString().getBytes(charsetName));
        }
        */
        MD5_Init(&c);
		MD5_Update(&c, (const unsigned char *)sb.c_str(), sb.length());
		MD5_Final((es_uint8_t*)hFinal, &c);

        return EString::toHexString(hFinal, sizeof(hFinal)); //ByteUtilities.asHex(hFinal);
    }
};

sp<EString> DigestUtilities::SESSION_HA1(new EString("DigestUtilities.SessionHA1"));

//=============================================================================

sp<ELogger> EHttpDigestAuthLogicHandler::logger = ELoggerManager::getLogger("EHttpDigestAuthLogicHandler");
ESecureRandom EHttpDigestAuthLogicHandler::rnd;

EHttpDigestAuthLogicHandler::EHttpDigestAuthLogicHandler(sp<EProxyIoSession>& proxyIoSession):
		EAbstractAuthLogicHandler(proxyIoSession) {
	(dynamic_pointer_cast<EHttpProxyRequest>(request))->checkRequiredProperties(EHttpProxyConstants_USER_PROPERTY,
			EHttpProxyConstants_PWD_PROPERTY, NULL);
}

void EHttpDigestAuthLogicHandler::doHandshake(EIoFilter::NextFilter* nextFilter) {
	logger->debug(__FILE__, __LINE__, " doHandshake()");

	if (step > 0 && directives == null) {
		throw EProxyAuthException(__FILE__, __LINE__, "Authentication challenge not received");
	}

	sp<EHttpProxyRequest> req = dynamic_pointer_cast<EHttpProxyRequest>(request);
	EMap<EString*, EList<EString*>*>* headers = req->getHeaders();

	if (step > 0) {
		logger->debug(__FILE__, __LINE__, "  sending DIGEST challenge response");

		// Build a challenge response
		EHashMap<EString*, EString*> map;
		EString k(EHttpProxyConstants_USER_PROPERTY);
		map.put(new EString("username"), new EString(req->getProperties()->get(&k)));
		EStringUtilities::copyDirective(directives, map, "realm");
		EStringUtilities::copyDirective(directives, map, "uri");
		EStringUtilities::copyDirective(directives, map, "opaque");
		EStringUtilities::copyDirective(directives, map, "nonce");
		EString algorithm = EStringUtilities::copyDirective(directives, map, "algorithm");

		// Check for a supported algorithm
		if (!algorithm.equalsIgnoreCase("md5") && !algorithm.equalsIgnoreCase("md5-sess")) {
			throw EProxyAuthException(__FILE__, __LINE__, "Unknown algorithm required by server");
		}

		// Check for a supported qop
        k = "qop";
		EString* qop = directives->get(&k);
		if (qop != null) {
			EStringTokenizer st(qop->c_str(), ",");
			EString token;// = null;

			while (st.hasMoreTokens()) {
				EString tk = st.nextToken();
				if (token.equalsIgnoreCase("auth")) {
					break;
				}

				/* @see:
				int pos = Arrays.binarySearch(DigestUtilities.SUPPORTED_QOPS, tk);
				if (pos > -1) {
					token = tk;
				}
				*/
				if (tk.equalsIgnoreCase("auth") || tk.equalsIgnoreCase("auth-int")) {
					token = tk;
				}
			}

			if (!token.isEmpty()) {
				map.put(new EString("qop"), new EString(token));

				byte nonce[8] = {0};
				rnd.nextBytes(nonce, 8);

				try {
					EString* cnonce = new EString(EBase64::encodeBase64(nonce, 8)/*, proxyIoSession.getCharsetName()*/);
					map.put(new EString("cnonce"), cnonce);
				} catch (EUnsupportedEncodingException& e) {
					throw EProxyAuthException(__FILE__, __LINE__, "Unable to encode cnonce", &e);
				}
			} else {
				throw EProxyAuthException(__FILE__, __LINE__, "No supported qop option available");
			}
		}

		map.put(new EString("nc"), new EString("00000001"));
		map.put(new EString("uri"), new EString(req->getHttpURI()));

		// Compute the response
		try {
            sp<EIoSession> session = proxyIoSession->getSession();
			EString k(EHttpProxyConstants_PWD_PROPERTY);
			map.put(new EString("response"), new EString(DigestUtilities::computeResponseValue(session, map, req
					->getHttpVerb(), req->getProperties()->get(&k)->c_str(), response->getBody())));

		} catch (EException& e) {
			throw EProxyAuthException(__FILE__, __LINE__, "Digest response computing failed", &e);
		}

		// Prepare the challenge response header and add it to the
		// request we will send
		EString sb("Digest ");// = new StringBuilder("Digest ");
		boolean addSeparator = false;

		sp<EIterator<EMapEntry<EString*, EString*>*> > iter = map.entrySet()->iterator();
		while (iter->hasNext()) {
			EMapEntry<EString*, EString*>* entry = iter->next();
			EString* key = entry->getKey();

			if (addSeparator) {
				sb.append(", ");
			} else {
				addSeparator = true;
			}

			boolean quotedValue = !key->equals("qop") && !key->equals("nc");
			sb.append(key);

			if (quotedValue) {
				sb.append("=\"").append(entry->getValue()).append('\"');
			} else {
				sb.append('=').append(entry->getValue());
			}
		}

		addValueToHeader(headers, "Proxy-Authorization", sb.toString().c_str(), true);
	}

	addKeepAliveHeaders(headers);
	//req->setHeaders(headers); ?

	writeRequest(nextFilter, req);
	step++;
}

void EHttpDigestAuthLogicHandler::handleResponse(sp<EHttpProxyResponse>& response) {
	this->response = response;

	if (step == 0) {
		if (response->getStatusCode() != 401 && response->getStatusCode() != 407) {
			EString msg("Received unexpected response code (");
			msg << response->getStatusLine() << ").";
			throw EProxyAuthException(__FILE__, __LINE__, msg.c_str());
		}

		// Header should look like this
		// Proxy-Authenticate: Digest still_some_more_stuff
		EString k("Proxy-Authenticate");
		EList<EString*>* values = response->getHeaders()->get(&k);
		EString* challengeResponse = null;

		sp<EIterator<EString*> > iter = values->iterator();
		while (iter->hasNext()) {
			EString* s = iter->next();
			if (s->startsWith("Digest")) {
				challengeResponse = s;
				break;
			}
		}

		if (challengeResponse == null) {
			throw EProxyAuthException(__FILE__, __LINE__, "Server doesn't support digest authentication method !");
		}

		try {
			EString s = challengeResponse->substring(7);
			directives = EStringUtilities::parseDirectives((byte*)s.c_str(), s.length());
		} catch (EException& e) {
			throw EProxyAuthException(__FILE__, __LINE__, "Parsing of server digest directives failed", &e);
		}
		step = 1;
	} else {
		EString msg("Received unexpected response code (");
		msg << response->getStatusLine() << ").";
		throw EProxyAuthException(__FILE__, __LINE__,  msg.c_str());
	}
}

} /* namespace eio */
} /* namespace efc */
