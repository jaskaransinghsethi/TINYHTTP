#pragma once

#include "../CommCore/CommCore.h"
#include "../Message/Message.h"

class HTTPClient : CommCore
{
public:
	HTTPClient() : CommCore(&socket) { }
	~HTTPClient() { }

	HTTPMessage<HTTPResponse> postMessage(HTTPMessage<HTTPRequest> msg);

	bool connect(const std::string& address, size_t port);
	
private:
	Sockets::SocketConnecter socket;
	Sockets::SocketSystem ss;
};

