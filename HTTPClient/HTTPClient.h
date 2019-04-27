#pragma once
//#define _ITERATOR_DEBUG_LEVEL 2
#include "../CommCore/CommCore.h"
#include "../Message/Message.h"
#include "IHTTPClient.h"

class HTTPClient : CommCore, public IHTTPClient
{
public:
	HTTPClient() : CommCore(&socket) { }
	~HTTPClient() { }

	HTTPMessage<HTTPResponse> postMessage(HTTPMessage<HTTPRequest> msg) override;

	bool connectServer(const std::string& address, size_t port) override;

private:
	Sockets::SocketConnecter socket;
	Sockets::SocketSystem ss;
};

// -----< Factory for returning Publisher as IPublisher* >--------------
IHTTPClient* ObjectFactory::createClient() {
	return new HTTPClient;
}
