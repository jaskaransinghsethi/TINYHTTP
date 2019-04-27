#include "pch.h"

#include "Translator.h"

Translator::Translator()
{
	ObjectFactory factory;
	client = factory.createClient();
}

Translator::~Translator()
{
	delete client;
}

bool Translator::connectServer(String^ address, int port)
{
	address_ = address;
	port_ = port;
	std::string add = sysStrToStdStr(address);
	size_t p = (size_t)port;
	bool connect = client->connectServer(add, p);
	return connect;
}

List<String^>^ Translator::postMessage(List<String^>^ msg)
{
	//connectServer(address_, port_);
	HTTPMessage<HTTPRequest> requestMsg;
	HTTPRequest request = HTTPRequest::fromString(sysStrToStdStr(msg[0]));
	requestMsg.getType() = request;
	if (msg->Count > 1) {
		for (int i = 1; i < msg->Count; i++)
		{
			std::string temp = sysStrToStdStr(msg[i]);
			std::string key = HTTPMessage<HTTPRequest>::getKeyFromAtt(temp);
			std::string value = HTTPMessage<HTTPRequest>::getValueFromAtt(temp);
			if (key == "body") {
				/*size_t find = value.find('=');
				if (find != value.npos) {

				}*/
				requestMsg.setBody(value);
			}
			else
				requestMsg.setAttribute(key, value);
		}
	}
	HTTPMessage<HTTPResponse> response = client->postMessage(requestMsg);
	std::string responseHeader = response.toHeaderStr();
	List<String^>^ responseMsg = gcnew List<String^>();
	responseMsg->Add(stdStrToSysStr(responseHeader));
	if (response.getContentLength() > 0)
		responseMsg->Add(stdStrToSysStr(response.toString()));

	return responseMsg;
}

//A function to convert System::String to STD::String
std::string Translator::sysStrToStdStr(String^ str)
{
	std::string temp;
	for (int i = 0; i < str->Length; ++i)
		temp += char(str[i]);
	return temp;
}

//A function to convert STD::String to System::String
String^ Translator::stdStrToSysStr(const std::string & str)
{
	return gcnew String(str.c_str());
}