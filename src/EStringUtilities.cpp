/*
 * EStringUtilities.cpp
 *
 *  Created on: 2016-7-7
 *      Author: cxxjava@163.com
 */

#include "../inc/EStringUtilities.hh"

namespace efc {
namespace eio {


EString EStringUtilities::getDirectiveValue(EHashMap<EString*, EString*>& directivesMap, const char* directive, boolean mandatory)
			THROWS(EAuthenticationException) {
	EString k(directive);
	EString* value = directivesMap.get(&k);
	if (value == null) {
		if (mandatory) {
			EString msg("\"");
			msg << directive << "\" mandatory directive is missing";
			throw EAuthenticationException(__FILE__, __LINE__, msg.c_str());
		}

		return "";
	}

	return EString(value);
}

void EStringUtilities::copyDirective(sp<EHashMap<EString*, EString*> >& directives, EString& sb, const char* directive) {
	EString k(directive);
	EString* directiveValue = directives->get(&k);
	if (directiveValue != null) {
		sb.append(directive).append(" = \"").append(directiveValue).append("\", ");
	}
}

EString EStringUtilities::copyDirective(sp<EHashMap<EString*, EString*> >& directives, EHashMap<EString*, EString*>& dst, const char* directive) {
	EString k(directive);
	EString* directiveValue = directives->get(&k);
	if (directiveValue != null) {
		dst.put(new EString(directive), new EString(directiveValue));
	}

	return EString(directiveValue);
}

void EStringUtilities::extractDirective(sp<EHashMap<EString*, EString*> >& map, EString key, EString value) THROWS(ESaslException) {
	EString* skey = new EString(key);
	if (map->get(skey) != null) {
		delete skey; //!
		EString msg("Peer sent more than one ");  msg << key << " directive";
		throw ESaslException(__FILE__, __LINE__, msg.c_str());
	}

	map->put(new EString(key), new EString(value));
}

sp<EHashMap<EString*, EString*> > EStringUtilities::parseDirectives(byte* buf, int size) THROWS(ESaslException) {
	sp<EHashMap<EString*, EString*> > map(new EHashMap<EString*, EString*>());
	boolean gettingKey = true;
	boolean gettingQuotedValue = false;
	boolean expectSeparator = false;
	byte bch;

	EByteArrayOutputStream key(10);
	EByteArrayOutputStream value(10);

	int i = skipLws(buf, 0, size);
	while (i < size) {
		bch = buf[i];

		if (gettingKey) {
			if (bch == ',') {
				if (key.size() != 0) {
					EString msg("Directive key contains a ',':"); msg << key.toString();
					throw ESaslException(__FILE__, __LINE__, msg.c_str());
				}

				// Empty element, skip separator and lws
				i = skipLws(buf, i + 1, size);
			} else if (bch == '=') {
				if (key.size() == 0) {
					throw ESaslException(__FILE__, __LINE__, "Empty directive key");
				}

				gettingKey = false; // Termination of key
				i = skipLws(buf, i + 1, size); // Skip to next non whitespace

				// Check whether value is quoted
				if (i < size) {
					if (buf[i] == '"') {
						gettingQuotedValue = true;
						++i; // Skip quote
					}
				} else {
					EString msg("Valueless directive found: "); msg << key.toString();
					throw ESaslException(__FILE__, __LINE__,  msg.c_str());
				}
			} else if (isLws(bch)) {
				// LWS that occurs after key
				i = skipLws(buf, i + 1, size);

				// Expecting '='
				if (i < size) {
					if (buf[i] != '=') {
						EString msg("'=' expected after key: "); msg << key.toString();
						throw ESaslException(__FILE__, __LINE__,  msg.c_str());
					}
				} else {
					EString msg("'=' expected after key: "); msg << key.toString();
					throw ESaslException(__FILE__, __LINE__,  msg.c_str());
				}
			} else {
				key.write(bch); // Append to key
				++i; // Advance
			}
		} else if (gettingQuotedValue) {
			// Getting a quoted value
			if (bch == '\\') {
				// quoted-pair = "\" CHAR ==> CHAR
				++i; // Skip escape
				if (i < size) {
					value.write(buf[i]);
					++i; // Advance
				} else {
					// Trailing escape in a quoted value
					EString msg("Unmatched quote found for directive: ");
					msg << key.toString() << " with value: " << value.toString();
					throw ESaslException(__FILE__, __LINE__,  msg.c_str());
				}
			} else if (bch == '"') {
				// closing quote
				++i; // Skip closing quote
				gettingQuotedValue = false;
				expectSeparator = true;
			} else {
				value.write(bch);
				++i; // Advance
			}
		} else if (isLws(bch) || bch == ',') {
			// Value terminated
			extractDirective(map, key.toString(), value.toString());
			key.reset();
			value.reset();
			gettingKey = true;
			gettingQuotedValue = expectSeparator = false;
			i = skipLws(buf, i + 1, size); // Skip separator and LWS
		} else if (expectSeparator) {
			EString msg("Expecting comma or linear whitespace after quoted string: \"");
					msg << value.toString() << "\"";
			throw ESaslException(__FILE__, __LINE__,  msg.c_str());
		} else {
			value.write(bch); // Unquoted value
			++i; // Advance
		}
	}

	if (gettingQuotedValue) {
		EString msg("Unmatched quote found for directive: ");
				msg << key.toString() << " with value: " << value.toString();
		throw ESaslException(__FILE__, __LINE__,  msg.c_str());
	}

	// Get last pair
	if (key.size() > 0) {
		extractDirective(map, key.toString(), value.toString());
	}

	return map;
}

boolean EStringUtilities::isLws(byte b) {
	switch (b) {
	case 13: // US-ASCII CR, carriage return
	case 10: // US-ASCII LF, line feed
	case 32: // US-ASCII SP, space
	case 9: // US-ASCII HT, horizontal-tab
		return true;
	}

	return false;
}

int EStringUtilities::skipLws(byte* buf, int start, int size) {
	int i;

	for (i = start; i < size; i++) {
		if (!isLws(buf[i])) {
			return i;
		}
	}

	return i;
}

} /* namespace eio */
} /* namespace efc */
