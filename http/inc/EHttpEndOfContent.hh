/*
 * EHttpEndOfContent.hh
 *
 *  Created on: 2017-1-4
 *      Author: cxxjava@163.com
 */

#ifndef EHTTPENDOFCONTENT_HH_
#define EHTTPENDOFCONTENT_HH_

#include "Efc.hh"

namespace efc {
namespace eio {

class EHttpEndOfContent: public EObject {
public:
	virtual EString toString() {
        return "HttpEndOfContent";
    }
};

} /* namespace eio */
} /* namespace efc */
#endif /* EHTTPENDOFCONTENT_HH_ */
