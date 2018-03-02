/*
 * EStringUtilities.hh
 *
 *  Created on: 2016-7-7
 *      Author: cxxjava@163.com
 */

#ifndef ESTRINGUTILITIES_HH_
#define ESTRINGUTILITIES_HH_

#include "Efc.hh"

namespace efc {
namespace eio {

/**
 * StringUtilities.java - Various methods to handle strings.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 * @since MINA 2.0.0-M3
 */

class EStringUtilities {
public:

	/**
	 * A directive is a parameter of the digest authentication process.
	 * Returns the value of a directive from the map. If mandatory is true and the
	 * value is null, then it throws an {@link AuthenticationException}.
	 *
	 * @param directivesMap the directive's map
	 * @param directive the name of the directive we want to retrieve
	 * @param mandatory is the directive mandatory
	 * @return the mandatory value as a String
	 * @throws AuthenticationException if mandatory is true and if
	 * directivesMap.get(directive) == null
	 */
	static EString getDirectiveValue(EHashMap<EString*, EString*>& directivesMap, const char* directive, boolean mandatory)
			THROWS(EAuthenticationException);

	/**
	 * Copy the directive to the {@link StringBuilder} if not null.
	 * (A directive is a parameter of the digest authentication process.)
	 *
	 * @param directives the directives map
	 * @param sb the output buffer
	 * @param directive the directive name to look for
	 */
	static void copyDirective(sp<EHashMap<EString*, EString*> >& directives, EString& sb, const char* directive);

	/**
	 * Copy the directive from the source map to the destination map, if it's
	 * value isn't null.
	 * (A directive is a parameter of the digest authentication process.)
	 *
	 * @param src the source map
	 * @param dst the destination map
	 * @param directive the directive name
	 * @return the value of the copied directive
	 */
	static EString copyDirective(sp<EHashMap<EString*, EString*> >& directives, EHashMap<EString*, EString*>& dst, const char* directive);

	/**
	 * Processes directive/value pairs from the digest-challenge and
	 * fill out the provided map.
	 *
	 * @param key A non-null String challenge token name.
	 * @param value A non-null String token value.
	 * @throws SaslException if either the key or the value is null or
	 * if the key already has a value.
	 */
	static void extractDirective(sp<EHashMap<EString*, EString*> >& map, EString key, EString value) THROWS(ESaslException);

	/**
	 * Parses digest-challenge string, extracting each token and value(s). Each token
	 * is a directive.
	 *
	 * @param buf A non-null digest-challenge string.
	 * @return A Map containing the aprsed directives
	 * @throws SaslException if the String cannot be parsed according to RFC 2831
	 */
	static sp<EHashMap<EString*, EString*> > parseDirectives(byte* buf, int size) THROWS(ESaslException);

	/**
	 * Is character a linear white space ?
	 * LWS            = [CRLF] 1*( SP | HT )
	 * Note that we're checking individual bytes instead of CRLF
	 *
	 * @param b the byte to check
	 * @return <tt>true</tt> if it's a linear white space
	 */
	static boolean isLws(byte b);

	/**
	 * Skip all linear white spaces
	 *
	 * @param buf the buf which is being scanned for lws
	 * @param start the offset to start at
	 * @return the next position in buf which isn't a lws character
	 */
	static int skipLws(byte* buf, int start, int size);
};

} /* namespace eio */
} /* namespace efc */
#endif /* ESTRINGUTILITIES_HH_ */
