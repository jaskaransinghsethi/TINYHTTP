#include "Message.h"
#include <conio.h>

using namespace Utilities;

std::string HTTPResponse::getStatusRes() const
{
	auto iter = statusRes_.find(statusCode_);
	if (iter != statusRes_.end())
		return iter->second;
	return "";
}

std::string HTTPResponse::toString() const
{
	return "HTTP/1.1 " + std::to_string(statusCode_) + " " + getStatusRes();
}

HTTPResponse HTTPResponse::fromString(const std::string& response)
{
	HTTPResponse reply;
	auto find1 = response.find_first_of(' ');
	auto find2 = response.find_last_of(' ');
	if (find1 != response.npos && find2 != response.npos) {
		reply.statusCode_ = stoi(response.substr(find1, find2-find1));
	}
	else {
		reply.statusCode_ = 400;
	}

	return reply;
}

std::string EndPoint::toString() const
{
	return address_ + ":" + std::to_string(port_);
}

EndPoint EndPoint::fromString(const std::string& endPoint)
{
	EndPoint ep;
	size_t pos = endPoint.find(":");
	if (pos != endPoint.npos) {
		ep.address_ = endPoint.substr(0, pos);
		ep.port_ = stoi(endPoint.substr(pos + 1));
	}
	return ep;
}

std::string HTTPRequest::toString() const
{
	std::string commandString;
	switch (method_)
	{
	case GET:
		commandString = "GET";
		break;
	case POST:
		commandString = "POST";
		break;
	case DELETE:
		commandString = "DELETE";
		break;
	case HEAD:
		commandString = "HEAD";
		break;
	default:
		commandString = "GET";
	}

	commandString += " ";
	commandString += fileSpec_;
	commandString += " HTTP/1.1";

	return commandString;
}

HTTPRequest HTTPRequest::fromString(const std::string& httprequest)
{
	HTTPRequest request;
	size_t pos = httprequest.find(" ");
	if (pos != httprequest.npos) {
		auto method = httprequest.substr(0, pos);
		if (method == "GET") request.method_ = GET;
		if (method == "POST") request.method_ = POST;
		if (method == "DELETE") request.method_ = DELETE;
		if (method == "HEAD") request.method_ = HEAD;
		request.fileSpec_ = httprequest.substr(pos + 1);
	}
	else {
		request.method_ = GET;
		request.fileSpec_ = "test.htm";
	}
	return request;
}

HTTPBody& HTTPBody::operator=(const std::string& body)
{
	body_.reserve(0);
	body_.insert(body_.end(), body.begin(), body.end());
	return *this;
}

HTTPBody::byte& HTTPBody::operator[](size_t i)
{
	if (i < 0 || body_.size() <= i)
		throw std::invalid_argument("Index out of bound");
	return body_[i];
}

HTTPBody::byte HTTPBody::operator[](size_t i) const
{
	if (i < 0 || body_.size() <= i)
		throw std::invalid_argument("Index out of bound");
	return body_[i];
}

void HTTPBody::show(std::ostream& out) const{
	out << "\nBody:";
	for (size_t i = 0; i < body_.size(); i++) {
		out << body_[i];
	}
}

std::string HTTPBody::toString() const {
	std::string temp;
	for (size_t i = 0; i < body_.size(); i++) {
		temp += body_[i];
	}
	return temp;
}


int main(int argc, char** argv) {
	using UTIL = StringHelper;
	
	//Testing EndPoint class
	UTIL::Title("TESTING ENDPOINT CLASS");
	EndPoint ep("localhost",27015);
	std::cout << "\n Endpoint:" << ep.toString();
	ep = ep.fromString("198.168.1.1:27015");
	std::cout << "\n Address: " << ep.getAddress();
	std::cout << "\n Port: " << ep.getPort();
	std::cout << "\n Endpoint: " << ep.toString();
	std::cout << "\n";

	//Testing HTTP Request class
	UTIL::Title("TESTING HTTP Request CLASS");
	HTTPRequest request(HTTPRequest::HTTPMethod::GET,"test.htm");
	std::cout << "\n Request method: " << request.getMethod();
	std::cout << "\n Filespec: " << request.getfileSpec();
	std::cout << "\n Header: " << request.toString();
	std::cout << "Testing fromString function";
	request = HTTPRequest::fromString("POST test.htm");
	std::cout << "\n Header: " << request.toString();

	//Testing HTTP Response CLASS
	UTIL::Title("TESTING HTTP Response CLASS");



	getchar();
	return 0;
}
