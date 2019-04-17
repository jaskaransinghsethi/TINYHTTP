#include "HTTPClient.h"

#include <Windows.h>
#include <iostream>
#include "../Utilities/Utilities.h"

using namespace Utilities;

using Util = Utilities::StringHelper;

HTTPMessage<HTTPResponse> HTTPClient::postMessage(HTTPMessage<HTTPRequest> msg)
{
	CommCore::postMessage<HTTPRequest>(msg);
	HTTPMessage<HTTPResponse> reply = CommCore::getMessage<HTTPResponse>();
	return reply;
}

bool HTTPClient::connect(const std::string& address, size_t port)
{
	socket.shutDown();
	return socket.connect(address, port);
}

int main() {
	
	//Renaming console window to HTTP Client
	SetConsoleTitle("HTTP Client");

	//Starting HTTP Client
	Util::Title("HTTPClient Initialising");

	HTTPMessage<HTTPRequest> getMsg;
	getMsg.getType().setMethod(HTTPRequest::GET);
	getMsg.getType().setfileSpec("debug/Hello1.html");

	std::cout << "--waiting for server";
	getchar();
	HTTPClient client;

	client.connect("localhost", 8080);
	std::cout << "\n Successfully connected to the Server";

	std::cout << "\n Sending Get Request to the server";
	getMsg.showMessage();

	client.connect("localhost", 8080);

	std::cout << "\n--posting message to localhost:8080:";
	getMsg.showMessage();
	Utilities::putline();

	HTTPMessage<HTTPResponse> reply = client.postMessage(getMsg);
	std::cout << "\n--received reply:";
	reply.show();
	
	//POST request not working as of now
	HTTPMessage<HTTPRequest> postReq;
	postReq.getType().setMethod(HTTPRequest::POST);
	std::string msgBody = "fileSpec=foobar.html";
	postReq.setBody(msgBody);
	postReq.setContentLength(postReq.getBody().size());
	client.connect("localhost", 8080);
	HTTPMessage<HTTPResponse> postRes = client.postMessage(postReq);
	std::cout << "\n Reply from the server";
	postRes.showMessage();

	//std::string temp = "quit";

	//while (true) {
	//	std::string input;
	//	std::cin >> input;
	//	if (input == temp)
	//		break;
	//	
	//	else {
	//		//implementation pending
	//	}
	//}

}


