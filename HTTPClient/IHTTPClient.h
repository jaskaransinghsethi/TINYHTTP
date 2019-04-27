#pragma once
#define IN_DLL

#include <string>
#include "../Message/Message.h"

#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif


class IHTTPClient {
public:
	virtual bool connectServer(const std::string& address, size_t port) = 0;
	virtual HTTPMessage<HTTPResponse> postMessage(HTTPMessage<HTTPRequest> msg) = 0;
	virtual ~IHTTPClient() { };
};

//Defining object factory in C code
extern "C" {
	// Instantiates Publisher class as IPublisher*
	struct ObjectFactory {
		DLL_DECL IHTTPClient* createClient();
	};
}
