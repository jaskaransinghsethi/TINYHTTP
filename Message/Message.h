#pragma once

#include <string>
#include <unordered_map>


class HTTPRequest {
public:
	enum HTTPMethod { Get, Post, Delete };				//Only supports get, post and delete methods for now

	//Constructors
	HTTPRequest() : method_(Get), fileSpec_("test.htm") { }
	HTTPRequest(HTTPMethod method, const std::string fileSpec) : method_(method), fileSpec_(fileSpec){ }
	
	//Getter & Setter for http method
	HTTPMethod getMethod() { return method_; }
	void setMethod(HTTPMethod method) { method_ = method; }

	//Getter & Setter for http fileSpec
	std::string getfileSpec() { return fileSpec_; }
	void setfileSpec(std::string fileSpec) { fileSpec_ = fileSpec; }

private:
	HTTPMethod method_;
	std::string fileSpec_;
};


class HTTPResponse {
public:
	using statusRes = std::unordered_map<size_t, std::string>;

	//Constructors
	HTTPResponse(size_t statusCode = 200) : statusCode_(statusCode) { } //by default response is OK, if the requests fails, identify the status code

	//Getter & Setter for status code
	size_t getStatusCode() { return statusCode_; }
	void setStatusCode(size_t statusCode) { statusCode_ = statusCode; }

	//Getter for status response type
	std::string getStatusRes();

private:
	size_t statusCode_;
	statusRes statusRes_{
		{100,"Informational"}, {200,"OK"}, {300, "Redirection"},
		{400, "Client Error"}, {500, "Server Error"}
	};
};