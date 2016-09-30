#include "main.hh"
#include "Eio.hh"

#ifdef WIN32
#define LOG ESystem::out->println
#else
#define LOG(fmt,...) ESystem::out->println(fmt, ##__VA_ARGS__)
#endif

class ErrHandler: public EThread::UncaughtExceptionHandler {
public:
	void uncaughtException(EThread* t, EThrowable* e) {
		LOG("This is:%s,Message:%s", t->getName(), e->getMessage());
		e->printStackTrace();
	}
};

class ChainedIoHandler: public EChainedIoHandler {
private:
	EString buf;
public:
	~ChainedIoHandler() {
		EIoHandlerChain* chain = getChain();
		EArrayList<EIoHandlerChain::Entry*> entrys = chain->getAll();
		for (int i=0; i<entrys.size(); i++) {
			// free all TestCommand objects.
			delete entrys.getAt(i)->getCommand();
		}
	}
	void appendString(const char s) {
		buf.append(s);
	}
	const char* getCString() {
		return buf.c_str();
	}
	virtual void messageReceived(sp<EIoSession>& session, sp<EObject>& message) THROWS(EException) {
		buf.append((llong)session->getId());
		EChainedIoHandler::messageReceived(session, message);
	}
};

class DemoIoHandler: public EIoHandler {
	void sessionCreated(sp<EIoSession>& session) THROWS(EException) {
//		LOG("sessionCreated");
        session->getConfig()->setIdleTime(EIdleStatus::BOTH_IDLE, 1);

//        class TestCommand : public EIoHandlerCommand {
//		private:
//			char ch;
//		public:
//			TestCommand(char ch) {
//				this->ch = ch;
//			}
//
//			virtual void execute(sp<NextCommand>& next, sp<EIoSession>& session, sp<EObject>& message) THROWS(EException) {
//				EString id(session->getId());
//				sp<ChainedIoHandler> cih = dynamic_pointer_cast<ChainedIoHandler>(session->getAttribute(&id));
//				cih->appendString(ch);
//				next->execute(session, message);
//			}
//		};
//
//        ChainedIoHandler* cih = new ChainedIoHandler();
//		cih->getChain()->addLast("A", new TestCommand('A'));
//		cih->getChain()->addLast("B", new TestCommand('B'));
//		cih->getChain()->addLast("C", new TestCommand('C'));
//
//        session->setAttribute(new EString(session->getId()), cih);
	}

	void sessionOpened(sp<EIoSession>& session) THROWS(EException) {
//		LOG("sessionOpened");
	}

	void sessionClosed(sp<EIoSession>& session) THROWS(EException) {
//		LOG("sessionClosed");

//		EString id(session->getId());
//		sp<ChainedIoHandler> cih = dynamic_pointer_cast<ChainedIoHandler>(session->getAttribute(&id));
//		LOG("sessionClosed()...buf=%s", cih->getCString());
	}

	void sessionIdle(sp<EIoSession>& session, EIdleStatus status)
			THROWS(EException) {
//		LOG("sessionIdle");
	}

	void exceptionCaught(sp<EIoSession>& session, sp<EThrowableType>& cause)
			THROWS(EException) {
//		LOG("exceptionCaught");
		session->closeNow();
	}

	void messageReceived(sp<EIoSession>& session, sp<EObject>& message)
			THROWS(EException) {
//		LOG("messageReceived, sessionid=%d", session->getId());

//		EString id(session->getId());
//		sp<ChainedIoHandler> cih = dynamic_pointer_cast<ChainedIoHandler>(session->getAttribute(&id));
//		cih->messageReceived(session, message);
//
//		//test 1.
//		EIoBuffer* in = dynamic_cast<EIoBuffer*>(message.get());
////		session->write(in->duplicate());
////		LOG("text line=%s", in->getString().c_str());

		//test 2.
		EIoBuffer *out = EIoBuffer::allocate(512);
		out->buf()->put("HTTP/1.1 200 OK\r\nContent-Length: 11\r\nContent-Type: text/html\r\n\r\nHello,world", 75);
		out->flip();
		session->write(out);

		//test 3.
//		session->write((EObject*)message); //error!

		//test 4.
//		EDefaultFileRegion* dfr = new EDefaultFileRegion(EFileChannel::open("xxx.dat", true, false));
//		session->write(dfr);

		//test 5.
//		EFileChannel* fc = EFileChannel::open("xxx.dat", true, false);
//		session->write(fc);

		//test 6.
//		EFile* file = new EFile("xxx.dat");
//		session->write(file);

		//test 7.
		session->closeOnFlush();
//		session->closeNow();
	}

	void messageSent(sp<EIoSession>& session, sp<EObject>& message)
			THROWS(EException) {
//		LOG("messageSent");
	}
    
    void inputClosed(sp<EIoSession>& session) THROWS(EException) {
//    	LOG("inputClosed");
    	session->closeNow();
    }
};

class StreamIoHandler: public EStreamIoHandler {
public:
	StreamIoHandler(EExecutorService* p): pool(p) {
	}

	virtual void sessionOpened(sp<EIoSession>& session) THROWS(EException) {
//		LOG("sessionOpened()");
		EStreamIoHandler::sessionOpened(session);
	}

	virtual void sessionClosed(sp<EIoSession>& session) THROWS(EException) {
//		LOG("sessionClosed()");
		EStreamIoHandler::sessionClosed(session);
	}

	void inputClosed(sp<EIoSession>& session) THROWS(EException) {
//		LOG("inputClosed");
		session->closeNow();
	}

	void exceptionCaught(sp<EIoSession>& session, sp<EThrowableType>& cause)
				THROWS(EException) {
		LOG("exceptionCaught");
		session->closeNow();
	}

	virtual void processStreamIo(sp<EIoSession>& session, sp<EInputStream>& in, sp<EOutputStream>& out) {
		class Work: virtual public ERunnable {
		public:
			Work(sp<EIoSession>& session, sp<EInputStream>& in, sp<EOutputStream>& out) {
				this->session = session;
				this->in = in;
				this->out = out;
			}
			virtual void run() {
				try {
					char buf[4096];
					int readBytes;
					while ((readBytes = in->read(buf, sizeof(buf))) > 0) {
//						LOG("[%s]", buf);

						if (eso_strnstr(buf, readBytes, "bye")) {
							break;
						}
					}

					out->write("HTTP/1.1 200 OK\r\nContent-Length: 11\r\nContent-Type: text/html\r\n\r\nHello,world", 75);
				} catch (...) {
					session->closeNow();
					return;
				}
				session->closeOnFlush();
			}
		private:
			sp<EIoSession> session;
			sp<EInputStream> in;
			sp<EOutputStream> out;
		};

		pool->execute(new Work(session, in, out));
	}

private:
	EExecutorService* pool;
};

static void test_niosocketacceptor() {
	ErrHandler handle;
	EThread::setDefaultUncaughtExceptionHandler(&handle);

	ENioSocketAcceptor *nsa = new ENioSocketAcceptor();
	EExecutorFilter* ef = new EExecutorFilter();
	ELoggingFilter* lf = new ELoggingFilter();
	EBlacklistFilter* bf = new EBlacklistFilter();
	EWhitelistFilter* wf = new EWhitelistFilter();
	EProfilerTimerFilter *ptf = new EProfilerTimerFilter();
	ETextLineCodecFactory* tlcf = new ETextLineCodecFactory("\n");
	EProtocolCodecFilter* pcf = new EProtocolCodecFilter(tlcf);
	DemoIoHandler* dih = new DemoIoHandler();
	EExecutorService* pool = EExecutors::newCachedThreadPool();
	StreamIoHandler* sih = new StreamIoHandler(pool);
	EInetSocketAddress isa(8888);
	nsa->setReuseAddress(true);
//	EInetAddress ia = EInetAddress::getByName("192.168.2.111");
	EInetAddress ia = EInetAddress::getByName("127.0.0.1");
//	bf->block(&ia);
	bf->block("127.0.0.1");
	wf->allow(&ia);
	nsa->getFilterChain()->addLast("executor", ef);
	nsa->getFilterChain()->addLast("logger", lf);
//	nsa->getFilterChain()->addLast("black", bf);
//	nsa->getFilterChain()->addLast("white", wf);
	nsa->getFilterChain()->addLast("profile", ptf);
//	nsa->getFilterChain()->addLast("codec", pcf);
	nsa->setHandler(dih);
//	nsa->setHandler(sih);
	nsa->bind(&isa);

	while (0) {

		//broadcast
		EIoBuffer *ib = EIoBuffer::allocate(1024);
		ib->buf()->put("0123456789\n", 11);
		ib->flip();
		nsa->broadcast(ib);

		//profile
		double averageTime = ptf->getAverageTime(EIoEventType::MESSAGE_RECEIVED);
//		LOG("AverageTime=%lf", averageTime);
		llong totalCalls = ptf->getTotalCalls(EIoEventType::MESSAGE_RECEIVED);
//		LOG("TotalCalls=%ld", totalCalls);

		EThread::sleep(1000);
	}
	EThread::sleep(10000000);

	pool->shutdown();
	delete pool;

	nsa->dispose(true);
	LOG("nsa->dispose(true)");

	delete ef;
	LOG("delete ef");
	delete lf;
	LOG("delete lf");
	delete bf;
	LOG("delete bf");
	delete wf;
	LOG("delete wf");
	delete ptf;
	LOG("delete ptf");
	delete pcf;
	LOG("delete pcf");
	delete tlcf;
	LOG("delete tlcf");

	delete dih;
	LOG("delete dih");
	delete sih;
	LOG("delete sih");

	delete nsa;
	LOG("delete nsa");
}

//=============================================================================

class AbstractProxyIoHandler : public EIoHandlerAdapter {
public:
	static sp<EString> OTHER_IO_SESSION;

	virtual void sessionCreated(sp<EIoSession>& session) THROWS(EException) {
		session->suspendRead();
		session->suspendWrite();
	}

	virtual void sessionClosed(sp<EIoSession>& session) THROWS(EException) {
		if (session->getAttribute( OTHER_IO_SESSION.get() ) != null) {
			sp<EIoSession> sess = dynamic_pointer_cast<EIoSession>(session->getAttribute(OTHER_IO_SESSION.get()));
			sess->setAttribute(OTHER_IO_SESSION, null);
			sess->close(false);
			session->setAttribute(OTHER_IO_SESSION, null);
		}
	}

	virtual void messageReceived(sp<EIoSession>& session, sp<EObject>& message) THROWS(EException) {
		sp<EIoBuffer> rb = dynamic_pointer_cast<EIoBuffer>( message );
		sp<EIoBuffer> wb = EIoBuffer::allocate(rb->remaining());
		rb->mark();
		wb->put(rb.get());
		wb->flip();
		sp<EIoSession> sess = dynamic_pointer_cast<EIoSession>(session->getAttribute(OTHER_IO_SESSION.get()));
		if (sess != null) {
			sess->write(wb);
		}
		rb->reset();
		LOGGER->info(__FILE__, __LINE__, rb->getString().c_str());
	}
private:
	static sp<ELogger> LOGGER;
};

class ServerToProxyIoHandler : public AbstractProxyIoHandler {
};

class ClientToProxyIoHandler : public AbstractProxyIoHandler {
public:
	virtual ~ClientToProxyIoHandler() {
		delete remoteAddress;
		delete connectorHandler;
	}
	ClientToProxyIoHandler(EIoConnector* connector,
			EInetSocketAddress* remoteAddress) {
		this->connectorHandler = new ServerToProxyIoHandler();
		this->connector = connector;
		this->remoteAddress = new EInetSocketAddress(*remoteAddress);
		connector->setHandler(connectorHandler);
	}

	virtual void sessionOpened(sp<EIoSession>& session) THROWS(EException) {
		class IoFutureListener : public EIoFutureListener {
		private:
			ClientToProxyIoHandler* handler;
			sp<EIoSession> session;
		public:
			~IoFutureListener() {
//				printf("~IoFutureListener(%p)\n", this);
			}
			IoFutureListener(ClientToProxyIoHandler* h, sp<EIoSession>& s) : handler(h), session(s) {
			}
			virtual void operationComplete(EIoFuture* future) {
				ON_FINALLY_NOTHROW(
					session->resumeRead();
					session->resumeWrite();
				) {
					try {
						sp<EIoSession> session2 = future->getSession();
						session2->setAttribute(OTHER_IO_SESSION, session);
						session->setAttribute(handler->OTHER_IO_SESSION, session2);
						session2->resumeRead();
						session2->resumeWrite();
					} catch (ERuntimeIoException& e) {
						// Connect failed
						session->closeNow();
					}
                }}
			}
		};
		connector->connect(remoteAddress)->addListener(new IoFutureListener(this, session));
	}
private:
	ServerToProxyIoHandler* connectorHandler;
	EIoConnector* connector;
	EInetSocketAddress* remoteAddress;
};

sp<EString> AbstractProxyIoHandler::OTHER_IO_SESSION = new EString("AbstractProxyIoHandler.OtherIoSession");
sp<ELogger> AbstractProxyIoHandler::LOGGER = ELoggerManager::getLogger("AbstractProxyIoHandler");

static void test_proxy() {
	ErrHandler handle;
	EThread::setDefaultUncaughtExceptionHandler(&handle);

	// Create TCP/IP acceptor.
	ENioSocketAcceptor *nsa = new ENioSocketAcceptor();

	// Create TCP/IP connector.
	EIoConnector* connector = new ENioSocketConnector();

	// Set connect timeout.
	connector->setConnectTimeoutMillis( 30 * 1000L );

	ClientToProxyIoHandler* handler = new ClientToProxyIoHandler( connector,
//            new EInetSocketAddress("192.168.2.173", 8888) );
    		new EInetSocketAddress("localhost", 8096) );

	ELoggingFilter* lf = new ELoggingFilter();
	EInetSocketAddress isa(8888);
	nsa->setReuseAddress(true);
	EInetAddress ia = EInetAddress::getByName("127.0.0.1");
	nsa->getFilterChain()->addLast("logger", lf);
	nsa->setHandler(handler);
	nsa->bind(&isa);

	EThread::sleep(10000000);

	nsa->dispose(true);
	connector->dispose(true);

	delete nsa;
	delete connector;
	delete handler;
	delete lf;
}

//=============================================================================

class MinaClientHandler : public EIoHandlerAdapter {
public:
	void exceptionCaught(sp<EIoSession>& session, EThrowableType* cause) THROWS(EException) {
		LOG("exceptionCaught");
		cause->getThrowable()->printStackTrace();
		session->closeNow();
	}

	void messageReceived(sp<EIoSession>& session, const void* message)THROWS(EException) {
		LOG("messageReceived");
	}

	void messageSent(sp<EIoSession>& session, const void* message) THROWS(EException) {
		LOG("messageSent");
	}

	void sessionClosed(sp<EIoSession>& session) THROWS(EException) {
		LOG("sessionClosed");
	}

	void sessionCreated(sp<EIoSession>& session) THROWS(EException) {
		LOG("sessionCreated");
		session->getConfig()->setIdleTime(EIdleStatus::BOTH_IDLE, 1);
	}

	void sessionIdle(sp<EIoSession>& session, EIdleStatus status) THROWS(EException) {
		LOG("sessionIdle");
	}

	void sessionOpened(sp<EIoSession>& session) THROWS(EException) {
		LOG("sessionOpened");
	}

	 void inputClosed(sp<EIoSession>& session) THROWS(EException) {
		LOG("inputClosed");
		session->closeNow();
	}
};

static void test_niosocketconnector() {
	EIoConnector* conn = new ENioSocketConnector();
	MinaClientHandler* mch = new MinaClientHandler();
	conn->setConnectTimeoutMillis(30000L);
//	conn.getFilterChain().addLast("codec",
//					new ProtocolCodecFilter(new TextLineCodecFactory(Charset
//					.forName("UTF-8"), LineDelimiter.WINDOWS.getValue(),
//					LineDelimiter.WINDOWS.getValue())));
	conn->setHandler(mch);
	sp<EIoSession> session =null;
	try {
		EInetSocketAddress isa("localhost", 8888);
		sp<EConnectFuture> future = conn->connect(&isa);
		future->awaitUninterruptibly();
		session = future->getSession();

		EIoBuffer *ib = EIoBuffer::allocate(1024);
		ib->buf()->put("0123456789\n", 11);
		ib->flip();
		int i = 0;
//		while (i++ < 9) {
			session->write(ib);
//		}
	} catch (EException& e) {
		LOG("exception...");
		e.printStackTrace();
	}

//	EThread::sleep(5000);

	if (session != null) {
		session->getCloseFuture()->awaitUninterruptibly();
	}
	conn->dispose(true);

//	EThread::sleep(10000);

	delete conn;
	delete mch;
}

static void test_niodatagramserver() {
	EExpiringSessionRecycler esr(15 * 1000);
	EInetSocketAddress isa(9001);
	DemoIoHandler handler;

	ENioDatagramAcceptor* acceptor = new ENioDatagramAcceptor();
	acceptor->setHandler(&handler);
	acceptor->setSessionRecycler(&esr);

	EDatagramSessionConfig* dcfg = acceptor->getSessionConfig();
	dcfg->setReuseAddress(true);
	dcfg->setReceiveBufferSize(1024);
	dcfg->setSendBufferSize(1024);

	acceptor->bind(&isa);

	while (1) {
		//broadcast is't work on udp mode.
		EIoBuffer *ib = EIoBuffer::allocate(1024);
		ib->buf()->put("0123456789\n", 11);
		ib->flip();
		acceptor->broadcast(ib);

		EThread::sleep(1000);
	}

	EThread::sleep(10000000);

	acceptor->dispose(true);

	delete acceptor;
}

static void test_niodatagramclient() {
	sp<EThread> cxxthread = EThread::c_init();

	class IoFutureListener: public EIoFutureListener {
	public:
		virtual void operationComplete(EIoFuture* future) {
			EConnectFuture* connFuture = dynamic_cast<EConnectFuture*>(future);
			if( connFuture->isConnected() ){
				sp<EIoSession> session = future->getSession();
				try {

					EIoBuffer *out = EIoBuffer::allocate(512);
					out->buf()->put("HTTP/1.1 200 OK\r\nContent-Length: 11\r\nContent-Type: text/html\r\n\r\nHello,world", 75);
					out->flip();
					session->write(out);

				} catch (EInterruptedException& e) {
					e.printStackTrace();
				}
			} else {
				LOG("Not connected...exiting");
			}
		}
	};

	EInetSocketAddress isa(9001);
	DemoIoHandler handler;

	ENioDatagramConnector connector;
	connector.setHandler(&handler);

	EDatagramSessionConfig* dcfg = connector.getSessionConfig();
	dcfg->setReuseAddress(true);
	dcfg->setReceiveBufferSize(1024);
	dcfg->setSendBufferSize(1024);

	sp<EConnectFuture> connFuture = connector.connect(&isa);
	connFuture->addListener(new IoFutureListener());
	connFuture->await();

	while (!connFuture->getSession()->isDestroyed()) {
		EThread::sleep(500);
	}

	connector.dispose();

	LOG("end of test_niodatagramclient()...");
}

static void test_multi_niodatagramclient() {
	int size = 50;
	EArray<EThreadX*> threads;
	EThreadX* tx;

	for (int i=0; i<size; i++) {
		tx = EThreadX::execute([&](){
			for (int j=0; j<5000; j++) {
				test_niodatagramclient();
			}
		});
		threads.add(tx);
	}

	for (int i=0; i<size; i++) {
		tx = threads.getAt(i);
		tx->join();
	}
}

//=============================================================================

MAIN_IMPL(testeio) {
	ESystem::init(argc, argv);
	ELoggerManager::init("log4e.properties");

	try {
		do {
			LOG("===========begin=============");

			test_niosocketacceptor();
//			test_niosocketconnector();
//			test_proxy();

//			EThread::sleep(100);


//			EThread::sleep(100);

			LOG("===========end=============");
		} while (1);
	}
	catch (ERuntimeIoException& e) {
		e.printStackTrace();
	}
	catch (EException& e) {
		ESystem::out->println("e=%s", e.toString().c_str());
		e.printStackTrace();
	}
	catch (EThrowable& e) {
		e.printStackTrace();
	}
	catch (...) {
		ESystem::out->println("catch all...");
	}

	ESystem::exit(0);

	return 0;
}
