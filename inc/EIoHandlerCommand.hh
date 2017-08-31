/*
 * EIoHandlerCommand.hh
 *
 *  Created on: 2016-5-31
 *      Author: cxxjava@163.com
 */

#ifndef EIOHANDLERCOMMAND_HH_
#define EIOHANDLERCOMMAND_HH_

#include "./EIoSession.hh"

namespace efc {
namespace eio {

/**
 * <p>A {@link IoHandlerCommand} encapsulates a unit of processing work to be
 * performed, whose purpose is to examine and/or modify the state of a
 * transaction that is represented by custom attributes provided by
 * {@link IoSession}.  Individual {@link IoHandlerCommand}s can be assembled into
 * a {@link IoHandlerChain}, which allows them to either complete the
 * required processing or delegate further processing to the next
 * {@link IoHandlerCommand} in the {@link IoHandlerChain}.</p>
 *
 * <p>{@link IoHandlerCommand} implementations typically retrieve and store state
 * information in the {@link IoSession} that is passed as a parameter to
 * the {@link #execute(NextCommand,IoSession,Object)} method, using custom
 * session attributes.  If you think getting attributes is tedious process,
 * you can create a bean which contains getters and setters of all properties
 * and store the bean as a session attribute:</p>
 *
 * <pre>
 * public class MyContext {
 *   public String getPropertyX() { ... };
 *   public void setPropertyX(String propertyX) { ... };
 *   public int getPropertyZ() { ... };
 *   public void setPropertyZ(int propertyZ) { ... };
 * }
 *
 * public class MyHandlderCommand implements IoHandlerCommand {
 *   public void execute( NextCommand next, IoSession session, Object message ) throws Exception {
 *     MyContext ctx = session.getAttribute( "mycontext" );
 *     ...
 *   }
 * }
 * </pre>
 *
 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
 */
interface EIoHandlerCommand: virtual public EObject {
	virtual ~EIoHandlerCommand(){}

	/**
	 * Represents an indirect reference to the next {@link IoHandlerCommand} of
	 * the {@link IoHandlerChain}.  This interface provides a way to forward
	 * the request to the next {@link IoHandlerCommand}.
	 *
	 * @author <a href="http://mina.apache.org">Apache MINA Project</a>
	 */
	interface NextCommand: virtual public EObject {
		virtual ~NextCommand(){}

		/**
		 * Forwards the request to the next {@link IoHandlerCommand} in the
		 * {@link IoHandlerChain}.
		 *
		 * @param session The current session
		 * @param message The message to pass on
		 * @throws Exception If anything went wrong
		 */
		virtual void execute(sp<EIoSession>& session, sp<EObject>& message) THROWS(EException) = 0;
	};

	/**
	 * <p>Execute a unit of processing work to be performed.  This
	 * {@link IoHandlerCommand} may either complete the required processing
	 * and just return to stop the processing, or delegate remaining
	 * processing to the next {@link IoHandlerCommand} in a {@link IoHandlerChain}
	 * containing this {@link IoHandlerCommand} by calling
	 * {@link NextCommand#execute(IoSession,Object)}.
	 *
	 * @param next an indirect reference to the next {@link IoHandlerCommand} that
	 *             provides a way to forward the request to the next {@link IoHandlerCommand}.
	 * @param session the {@link IoSession} which is associated with
	 *                this request
	 * @param message the message object of this request
	 *
	 * @exception Exception general purpose exception return
	 *                      to indicate abnormal termination
	 */
	virtual void execute(sp<NextCommand>& next, sp<EIoSession>& session, sp<EObject>& message) THROWS(EException) = 0;
};

} /* namespace eio */
} /* namespace efc */

#endif /* EIOHANDLERCOMMAND_HH_ */
