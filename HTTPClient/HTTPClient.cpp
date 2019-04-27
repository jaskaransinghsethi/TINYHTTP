#include "HTTPClient.h"

#include <Windows.h>
#include <iostream>
#include "../Utilities/Utilities.h"
//#define _ITERATOR_DEBUG_LEVEL 2
using namespace Utilities;

using Util = Utilities::StringHelper;

HTTPMessage<HTTPResponse> HTTPClient::postMessage(HTTPMessage<HTTPRequest> msg)
{
	CommCore::postMessage<HTTPRequest>(msg);
	HTTPMessage<HTTPResponse> reply = CommCore::getMessage<HTTPResponse>();
	return reply;
}

bool HTTPClient::connectServer(const std::string& address, size_t port)
{
	socket.shutDown();
	return socket.connect(address, port);
}

int main() {

	//Renaming console window to HTTP Client
	SetConsoleTitle("HTTP Client");

	//Starting HTTP Client
	Util::Title("HTTPClient Initialising");
	std::cout << "--waiting for server";
	getchar();

	HTTPClient client;

	if (client.connectServer("localhost", 8080)) {
		std::cout << "\n Successfully connected to the Server";
	}
	else {
		std::cout << "\n Failed to connect to the server";
		return -1;
	}

	//GET request
	std::cout << "\n Sending Get Request to the server";
	HTTPMessage<HTTPRequest> getMsg;
	getMsg.getType().setMethod(HTTPRequest::GET);
	getMsg.getType().setfileSpec("debug/Hello1.html");
	getMsg.showMessage();

	HTTPMessage<HTTPResponse> reply = client.postMessage(getMsg);
	std::cout << "\n Reply recieved from the server";
	reply.showMessage();

	//POST request
	std::cout << "\n Sending Post Request to the server";
	HTTPMessage<HTTPRequest> postReq;
	postReq.getType().setMethod(HTTPRequest::POST);
	std::string msgBody = "fileSpec=foobar.html";
	postReq.setBody(msgBody);
	postReq.setContentLength(5);
	client.connectServer("localhost", 8080);
	HTTPMessage<HTTPResponse> postRes = client.postMessage(postReq);
	std::cout << "\n Reply from the server";
	postRes.showMessage();


	//Head request
	std::cout << "\n Sending Head Request to the server";
	HTTPMessage<HTTPRequest> headMsg;
	headMsg.getType().setMethod(HTTPRequest::HEAD);
	headMsg.getType().setfileSpec("debug/Hello1.html");
	headMsg.showMessage();
	client.connectServer("localhost", 8080);
	HTTPMessage<HTTPResponse> headReply = client.postMessage(headMsg);
	std::cout << "\n Reply from the server";
	headReply.showMessage();

	//Put request
	std::cout << "\n Sending PUT Request to the server";
	HTTPMessage<HTTPRequest> putMsg;
	putMsg.getType().setMethod(HTTPRequest::PUT);
	putMsg.getType().setfileSpec("debug/Hello1.html");
	putMsg.showMessage();
	client.connectServer("localhost", 8080);
	HTTPMessage<HTTPResponse> putReply = client.postMessage(putMsg);
	std::cout << "\n Reply from the server";
	putReply.showMessage();
}


