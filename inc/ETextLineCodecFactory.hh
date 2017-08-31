/*
 * ETextLineCodecFactory.hh
 *
 *  Created on: 2016-5-24
 *      Author: cxxjava@163.com
 */

#ifndef ETEXTLINECODECFACTORY_HH_
#define ETEXTLINECODECFACTORY_HH_

#include "./EProtocolCodecFactory.hh"
#include "./ETextLineEncoder.hh"
#include "./ETextLineDecoder.hh"

namespace efc {
namespace eio {

/**
 * A {@link ProtocolCodecFactory} that performs encoding and decoding between
 * a text line data and a Java string object.  This codec is useful especially
 * when you work with a text-based protocols such as SMTP and IMAP.
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */

class ETextLineCodecFactory: virtual public EProtocolCodecFactory {
public:
	virtual ~ETextLineCodecFactory();

	/**
	 * Creates a new instance with the current default {@link Charset}.
	 */
	ETextLineCodecFactory(const char* delimiter);
	ETextLineCodecFactory(const char* encodingDelimiter, const char* decodingDelimiter);

	/**
	 * {@inheritDoc}
	 */
	virtual EProtocolEncoder* getEncoder(sp<EIoSession>& session) THROWS(EException);

	/**
	 * {@inheritDoc}
	 */
	virtual EProtocolDecoder* getDecoder(sp<EIoSession>& session) THROWS(EException);

private:
	ETextLineEncoder* encoder;
	ETextLineDecoder* decoder;
};

} /* namespace eio */
} /* namespace efc */
#endif /* ETEXTLINECODECFACTORY_HH_ */
