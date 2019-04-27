#pragma once

#include <string>

#include "../HTTPClient/IHTTPClient.h"
#include "../Message/Message.h"

using namespace System;
using namespace System::Collections::Generic;

public ref class Translator
{
public:
	Translator();
	~Translator();
	bool connectServer(String^ address, int port);
	List<String^>^ postMessage(List<String^>^ msg);

private:
	std::string sysStrToStdStr(String^ str);
	String^ stdStrToSysStr(const std::string& str);
	//HTTPRequest::HTTPMethod findMethod(String^ msg);
	IHTTPClient* client;
	String^ address_;
	int port_;
};
