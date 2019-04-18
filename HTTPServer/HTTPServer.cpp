#include "HTTPServer.h"

RequestMsg HTTPServerCore::getMessage() {
	RequestMsg msg = CommCore::getMessage<HTTPRequest>();
	return msg;
}

void HTTPServerCore::postMessage(HTTPMessage<HTTPResponse> reply) {
	CommCore::postMessage<HTTPResponse>(reply);
}

HTTPMessage<HTTPResponse> HTTPServerCore::doProc(HTTPMessage<HTTPRequest>& msg)
{
	HTTPMessage<HTTPRequest>::Key key = "command";

	if (msg.containsKey(key))  // process non-HTTP command
	{
		HTTPMessage<HTTPRequest>::Value cmdValue = msg.getAttributes()[key];
		if (containsKey(cmdValue))
			return dispatcher_[cmdValue](msg);
	}
	else  // process HTTP command
	{
		Key procKey = msg.getType().toString(false);
		if (containsKey(procKey))
		{
			HTTPMessage<HTTPResponse> reply = dispatcher_[procKey](msg);
			return reply;
		}
	}
	// return error message

	HTTPMessage<HTTPResponse> errReply;
	errReply.getType().setStatusCode(400);
	return errReply;
}

void HTTPServerCore::addProc(Key key, MessageProcessType proc)
{
	if (containsKey(key))
		return;
	dispatcher_[key] = proc;
}

bool HTTPServerCore::containsKey(Key key)
{
	return dispatcher_.find(key) != dispatcher_.end();
}

void ClientHandler::operator()(Sockets::Socket&& socket)
{
	pServer_->setSocket(&socket);

	HTTPMessage<HTTPRequest> msg = pServer_->getMessage();
	msg.showMessage();

	HTTPMessage<HTTPResponse> reply = pServer_->doProc(msg);

	pServer_->postMessage(reply);
	reply.showMessage();

	socket.shutDown();
}

#define TEST_SERVER
#ifdef TEST_SERVER
using UTIL = Utilities::StringHelper;
int main() {
	
	try {
		//Renaming console window to HTTP Client
		SetConsoleTitle("HTTP Server");

		//Starting HTTP Client
		UTIL::Title("HTTP Server Initialising");

		//Server started at port 8080 with IPV4
		HTTPServer server(8080, Sockets::Socket::IP4);

		//Adding processing methods for get and post request
		server.addProc("GET", getProc);
		server.addProc("POST", postProc);
		server.addProc("HEAD", headProc);
		server.addProc("PUT", putProc);

		ClientHandler cp(&server);
		server.start<ClientHandler>(cp);
		
		//UTIL::title("Press any key to exit");
		//
		////Flushing the stream for any bad bit
		//std::cout.flush();
		////std::cin.get();
		getchar();
	}

	catch (std::exception & ex) {
		std::cout << "\n Exception occurred:" << ex.what();
	}
}

#endif