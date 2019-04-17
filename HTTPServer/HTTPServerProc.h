#pragma once

#include "../Message/Message.h"
#include "../Utilities/Utilities.h"
#include <functional>
#include <processenv.h>
#include <fstream>

using RequestMsg = HTTPMessage<HTTPRequest>;
using ReplyMsg = HTTPMessage<HTTPResponse>;
using Key = std::string;
using MessageProcessType = std::function<ReplyMsg(RequestMsg&)>;

inline HTTPMessage<HTTPResponse> getProc(HTTPMessage<HTTPRequest> msg) {
	
	HTTPMessage<HTTPResponse> reply;
	
	const size_t bufSize = 255;
	char buffer[bufSize];

	GetCurrentDirectoryA(bufSize, buffer);

	std::string fileSpec = msg.getType().getfileSpec();
	if (fileSpec[0] == '/')
		fileSpec.insert(fileSpec.begin(), '.');

	std::ifstream in(fileSpec);

	if (in.good()) {
		std::stringstream out;
		out << in.rdbuf();
		std::string text = out.str();
		reply.setContentLength(text.size());
		reply.setBody(text.size(),(HTTPBody::byte*) &text[0]);
		reply.getType().setStatusCode(200);
		return reply;
	}
	
	reply.getType().setStatusCode(400);
	reply.clear();

	return reply;	
}

inline HTTPMessage<HTTPResponse> postProc(HTTPMessage<HTTPRequest> msg) {
	
	HTTPMessage<HTTPResponse> reply;

	const size_t bufSize = 255;
	char buffer[bufSize];

	GetCurrentDirectoryA(bufSize, buffer);

	std::string msgBody = msg.toString();

	std::vector<std::string> splits = Utilities::StringHelper::split(msgBody, '&');
	std::vector<std::string> temp;
	std::unordered_map<std::string, std::string> params;
	for (size_t i = 0; i < splits.size(); i++) {
		temp = Utilities::StringHelper::split(splits[i], '=');
		auto iter = params.find(temp[0]);
		if (iter == params.end()) {
			params.insert(std::pair<std::string, std::string>(temp[0], temp[1]));
		}
	}
	std::string fileSpec = params["fileSpec"];
	if (fileSpec[0] == '/')
		fileSpec.insert(fileSpec.begin(), '.');

	std::ifstream in(fileSpec);

	if (in.good()) {
		std::stringstream out;
		out << in.rdbuf();
		std::string text = out.str();
		reply.setContentLength(text.size());
		reply.setBody(text.size(), (HTTPBody::byte*) & text[0]);
		reply.getType().setStatusCode(200);
		return reply;
	}

	reply.getType().setStatusCode(400);
	reply.clear();

	return reply;
}

