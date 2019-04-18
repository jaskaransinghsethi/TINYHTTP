#pragma once

#include "../CommCore/CommCore.h"
#include "../Message/Message.h"
#include "../Sockets/Sockets.h"
#include "HTTPServerProc.h"
#include <functional>

class HTTPServerCore : public CommCore {
public:
	HTTPServerCore() = default;
	HTTPServerCore(Sockets::Socket* pSocket) : CommCore(pSocket) { }
	virtual ~HTTPServerCore() { }
	
	HTTPMessage<HTTPRequest> getMessage();
	void postMessage(HTTPMessage<HTTPResponse> msg);

	HTTPMessage<HTTPResponse> doProc(HTTPMessage<HTTPRequest>& msg);

	void addProc(Key key, MessageProcessType proc);
	bool containsKey(Key key);
private:
	std::unordered_map<std::string, MessageProcessType> dispatcher_;
};


class HTTPServer : public HTTPServerCore
{
public:
	HTTPServer(size_t port, Sockets::Socket::IpVer ipv) : socketListner(port, ipv) { }
	//~HTTPServer(){ }

	template <typename ClientHandlerType>
	bool start(ClientHandlerType& co);

private:
	Sockets::SocketSystem ss;
	Sockets::SocketListener socketListner;
};

class ClientHandler {
public:
	ClientHandler(HTTPServer *pServer) : pServer_(pServer) { }
	void operator()(Sockets::Socket&& socket);

private:
	HTTPServer* pServer_;
};

template<typename ClientHandlerType>
inline bool HTTPServer::start(ClientHandlerType& co)
{
	std::cout << "\n Starting server listener";
	return socketListner.start(co);
}
