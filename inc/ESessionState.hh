/*
 * ESessionState.hh
 *
 *  Created on: 2016-3-10
 *      Author: cxxjava@163.com
 */

#ifndef ESESSIONSTATE_HH_
#define ESESSIONSTATE_HH_

namespace efc {
namespace eio {

/**
 * The session state. A session can be in three different state :
 * <ul>
 *   <li>OPENING : The session has not been fully created</li>
 *   <li>OPENED : The session is opened</li>
 *   <li>CLOSING :  The session is closing</li>
 * </ul>
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */
enum ESessionState {
    OPENING, OPENED, CLOSING
};

} /* namespace eio */
} /* namespace efc */
#endif /* ESESSIONSTATE_HH_ */
