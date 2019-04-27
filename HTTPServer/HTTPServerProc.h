#pragma once

#include "../Message/Message.h"
#include "../Utilities/Utilities.h"
#include "../MongoDB/MongoDB/MongoDB.h"
#include <functional>
#include <processenv.h>
#include <fstream>

using RequestMsg = HTTPMessage<HTTPRequest>;
using ReplyMsg = HTTPMessage<HTTPResponse>;
using Key = std::string;
using MessageProcessType = std::function<ReplyMsg(RequestMsg&)>;

inline HTTPMessage<HTTPResponse> insertReq(HTTPMessage<HTTPRequest>& msg) {
	HTTPMessage<HTTPResponse> reply;
	MongoDB instance;
	instance.createConnection();
	std::vector<std::string> temp1 = Utilities::StringHelper::split(msg.getAttributes()["db&Collection"], '&');
	if (temp1.size() < 2) {
		reply.getType().setStatusCode(400);
		reply.clear();
		return reply;
	}
	const char* dbName = temp1[0].c_str();
	const char* collectionName = temp1[1].c_str();
	if (instance.createDB(dbName, collectionName)) {
		reply.getType().setStatusCode(400);
		reply.clear();
		return reply;
	}
	std::vector<std::string> temp2 = Utilities::StringHelper::split(msg.getAttributes()["insert"], '&');
	if (temp2.size() < 2) {
		reply.getType().setStatusCode(400);
		reply.clear();
		return reply;
	}
	const char* colmn = temp2[0].c_str();
	const char* value = temp2[1].c_str();
	if (!instance.insertDoc(colmn, value)) {
		reply.getType().setStatusCode(400);
		reply.clear();
		return reply;
	}

	return HTTPMessage<HTTPResponse>();
}

inline HTTPMessage<HTTPResponse> deleteReq(HTTPMessage<HTTPRequest>& msg) {
	HTTPMessage<HTTPResponse> reply;
	MongoDB instance;
	instance.createConnection();
	std::vector<std::string> temp1 = Utilities::StringHelper::split(msg.getAttributes()["db&Collection"], '&');
	if (temp1.size() < 2) {
		reply.getType().setStatusCode(400);
		reply.clear();
		return reply;
	}
	const char* dbName = temp1[0].c_str();
	const char* collectionName = temp1[1].c_str();
	if (instance.createDB(dbName, collectionName)) {
		reply.getType().setStatusCode(400);
		reply.clear();
		return reply;
	}

	std::vector<std::string> temp2 = Utilities::StringHelper::split(msg.getAttributes()["delete"], '&');
	if (temp2.size() < 2) {
		reply.getType().setStatusCode(400);
		reply.clear();
		return reply;
	}
	const char* colmn = temp2[0].c_str();
	const char* value = temp2[1].c_str();
	if (!instance.deleteDoc(colmn, value)) {
		reply.getType().setStatusCode(400);
		reply.clear();
		return reply;
	}

	return HTTPMessage<HTTPResponse>();
}

inline HTTPMessage<HTTPResponse> findReq(HTTPMessage<HTTPRequest>& msg) {
	HTTPMessage<HTTPResponse> reply;
	MongoDB instance;
	instance.createConnection();
	std::vector<std::string> temp1 = Utilities::StringHelper::split(msg.getAttributes()["db&Collection"], '&');
	if (temp1.size() < 2) {
		reply.getType().setStatusCode(400);
		reply.clear();
		return reply;
	}
	const char* dbName = temp1[0].c_str();
	const char* collectionName = temp1[1].c_str();
	if (instance.createDB(dbName, collectionName)) {
		reply.getType().setStatusCode(400);
		reply.clear();
		return reply;
	}

	std::vector<std::string> temp2 = Utilities::StringHelper::split(msg.getAttributes()["find"], '&');
	if (temp2.size() < 2) {
		reply.getType().setStatusCode(400);
		reply.clear();
		return reply;
	}
	const char* colmn = temp2[0].c_str();
	const char* value = temp2[1].c_str();
	std::vector<std::string> result = instance.findDoc(colmn, value);
	if (result.size() == 0) {
		reply.getType().setStatusCode(400);
		reply.clear();
		return reply;
	}
	std::string text;
	for (auto str : result)
		text += str;
	reply.setContentLength(text.size());
	reply.setBody(text.size(), (HTTPBody::byte*) & text[0]);
	reply.getType().setStatusCode(200);

	return reply;
}

inline HTTPMessage<HTTPResponse> findAllReq(HTTPMessage<HTTPRequest>& msg) {
	HTTPMessage<HTTPResponse> reply;
	MongoDB instance;
	instance.createConnection();
	std::vector<std::string> temp1 = Utilities::StringHelper::split(msg.getAttributes()["db&Collection"], '&');
	if (temp1.size() < 2) {
		reply.getType().setStatusCode(400);
		reply.clear();
		return reply;
	}
	const char* dbName = temp1[0].c_str();
	const char* collectionName = temp1[1].c_str();
	if (instance.createDB(dbName, collectionName)) {
		reply.getType().setStatusCode(400);
		reply.clear();
		return reply;
	}

	std::vector<std::string> result = instance.findAllDoc();
	if (result.size() == 0) {
		reply.getType().setStatusCode(400);
		reply.clear();
		return reply;
	}
	std::string text;
	for (auto str : result)
		text += str;
	reply.setContentLength(text.size());
	reply.setBody(text.size(), (HTTPBody::byte*) & text[0]);
	reply.getType().setStatusCode(200);

	return reply;
}

inline HTTPMessage<HTTPResponse> handleMongoDB(HTTPMessage<HTTPRequest>& msg) {
	if (msg.getAttributes().find("insert") != msg.getAttributes().end())
		return insertReq(msg);
	else if (msg.getAttributes().find("delete") != msg.getAttributes().end())
		return deleteReq(msg);
	else if (msg.getAttributes().find("find") != msg.getAttributes().end())
		return findReq(msg);
	else if (msg.getAttributes().find("findall") != msg.getAttributes().end())
		return findAllReq(msg);

	return HTTPMessage<HTTPResponse>();
}

inline HTTPMessage<HTTPResponse> getProc(HTTPMessage<HTTPRequest> msg) {

	if (msg.getAttributes()["request"] == "mongo")
		return handleMongoDB(msg);

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
		reply.setBody(text.size(), (HTTPBody::byte*) & text[0]);
		reply.getType().setStatusCode(200);
		return reply;
	}

	reply.getType().setStatusCode(400);
	reply.clear();

	return reply;
}

inline HTTPMessage<HTTPResponse> postProc(HTTPMessage<HTTPRequest> msg) {

	if (msg.getAttributes()["command"] == "mongo")
		return handleMongoDB(msg);

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

inline HTTPMessage<HTTPResponse> headProc(HTTPMessage<HTTPRequest> msg) {

	HTTPMessage<HTTPResponse> reply;

	const size_t bufSize = 255;
	char buffer[bufSize];

	GetCurrentDirectoryA(bufSize, buffer);

	std::string fileSpec = msg.getType().getfileSpec();
	if (fileSpec[0] == '/')
		fileSpec.insert(fileSpec.begin(), '.');

	if (fileSpec == "" || fileSpec == "./" || fileSpec == "/") {
		reply.getType().setStatusCode(200);
		return reply;
	}

	std::ifstream in(fileSpec);

	if (in.good()) {
		reply.getType().setStatusCode(200);
		return reply;
	}

	reply.getType().setStatusCode(400);
	reply.clear();

	return reply;
}

inline HTTPMessage<HTTPResponse> putProc(HTTPMessage<HTTPRequest> msg) {

	if (msg.getAttributes()["command"] == "mongo")
		return handleMongoDB(msg);

	HTTPMessage<HTTPResponse> reply;

	const size_t bufSize = 255;
	char buffer[bufSize];

	GetCurrentDirectoryA(bufSize, buffer);

	std::string fileSpec = msg.getType().getfileSpec();
	if (fileSpec[0] == '/' && fileSpec[1] == '?')
		fileSpec = fileSpec.substr(2);

	/*if (fileSpec == "/") {
		reply.getType().setStatusCode(200);
		return reply;
	}*/

	if (fileSpec[0] == '/')
		fileSpec.insert(fileSpec.begin(), '.');
	std::string name;
	size_t pos = fileSpec.find("/");
	if (pos != fileSpec.npos) {
		name = fileSpec.substr(pos + 1, fileSpec.size() - pos);
	}
	std::string path(buffer);
	path = path + "\\Storage\\";

	std::ifstream in(fileSpec);

	if (in.good()) {
		std::stringstream out;
		std::ofstream fs;
		out << in.rdbuf();
		std::string text = out.str();
		fs.open(path + name);
		if (fs.good()) {
			fs << text;
		}
		name = "fileCreated.html";
		in.close();
		in.open(path + name);
		out.str(std::string());
		text.clear();
		if (in.good()) {
			out << in.rdbuf();
			text = out.str();
		}
		reply.setContentLength(text.size());
		reply.setBody(text.size(), (HTTPBody::byte*) & text[0]);
		reply.getType().setStatusCode(200);
		return reply;

	}

	reply.getType().setStatusCode(400);
	reply.clear();

	return reply;
}


inline HTTPMessage<HTTPResponse> deleteProc(HTTPMessage<HTTPRequest> msg) {

	if (msg.getAttributes()["command"] == "mongo")
		return handleMongoDB(msg);

	HTTPMessage<HTTPResponse> reply;

	const size_t bufSize = 255;
	char buffer[bufSize];

	GetCurrentDirectoryA(bufSize, buffer);

	std::string fileSpec = msg.getType().getfileSpec();
	if (fileSpec[0] == '/' && fileSpec[1] == '?')
		fileSpec = fileSpec.substr(2);

	/*if (fileSpec == "/") {
		reply.getType().setStatusCode(200);
		return reply;
	}*/

	if (fileSpec[0] == '/')
		fileSpec.insert(fileSpec.begin(), '.');

	std::string name;
	size_t pos = fileSpec.find("/");
	if (pos != fileSpec.npos) {
		name = fileSpec.substr(pos + 1, fileSpec.size() - pos);
	}
	std::string path(buffer);
	path = path + "\\Storage\\";

	if (std::remove(fileSpec.c_str()) == 0)
	{
		std::ifstream in;
		std::stringstream out;
		std::ofstream fs;
		name = "fileDeleted.html";
		in.open(path + name);
		std::string text;
		if (in.good()) {
			out << in.rdbuf();
			text = out.str();
		}
		reply.setContentLength(text.size());
		reply.setBody(text.size(), (HTTPBody::byte*) & text[0]);
		reply.getType().setStatusCode(200);
		return reply;
	}

	reply.getType().setStatusCode(400);
	reply.clear();

	return reply;
}

