#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include "../Utilities/Utilities.h"

class EndPoint {
public:
	using Address = std::string;
	using Port = size_t;

	//Constructors
	EndPoint() = default;
	EndPoint(Address address, Port port) : address_(address), port_(port) { }

	//Getter & Setter for Address
	Address& getAddress() { return address_; }
	void setAddress(Address& address) { address_ = address; }

	//Getter & Setter for Port
	Port& getPort() { return port_; }
	void setPort(Port port) { port_ = port; }

	//Conversion from and to string
	std::string toString() const;
	static EndPoint fromString(const std::string& endPoint);
private:
	Address address_;
	Port port_;
};

//toString & fromString function might need exception handling
class HTTPRequest {
public:
	//Type of HTTP Request
	enum HTTPMethod { GET, POST, DELETE, HEAD, PUT };				

	//Constructors
	HTTPRequest() : method_(GET), fileSpec_("test.htm") { }			//By default, send a get request to access test.html
	HTTPRequest(HTTPMethod method, const std::string fileSpec) : method_(method), fileSpec_(fileSpec){ }
	
	//Getter & Setter for http method
	HTTPRequest::HTTPMethod getMethod() { return method_; }
	void setMethod(HTTPMethod method) { method_ = method; }

	//Getter & Setter for http fileSpec
	std::string getfileSpec() { return fileSpec_; }
	void setfileSpec(std::string fileSpec) { fileSpec_ = fileSpec; }

	//toString & fromString functions
	std::string toString(bool full = true) const;
	static HTTPRequest fromString(const std::string& httprequest);

private:
	HTTPMethod method_;
	std::string fileSpec_;
};

//toString & fromString function might need exception handling
class HTTPResponse {
public:
	using statusRes = std::unordered_map<size_t, std::string>;

	//Constructors
	HTTPResponse(size_t statusCode = 200) : statusCode_(statusCode) { } //by default response is OK, if the requests fails, identify the status code

	//Getter & Setter for status code
	size_t getStatusCode() { return statusCode_; }
	void setStatusCode(size_t statusCode) { statusCode_ = statusCode; }

	//Getter for status response type
	std::string getStatusRes() const;

	//toString & fromString function
	std::string toString(bool full = true) const;
	static HTTPResponse fromString(const std::string& response);

private:
	size_t statusCode_;
	statusRes statusRes_{
		{100,"Informational"}, {200,"OK"}, {300, "Redirection"},
		{400, "Client Error"}, {500, "Server Error"}
	};
};

class HTTPBody {
public:
	using byte = unsigned char;
	using iterator = std::vector<byte>::iterator;

	//Constructors
	HTTPBody() = default;
	HTTPBody(size_t size) { body_.reserve(size); }
	HTTPBody(size_t size, byte* body) { body_.reserve(size); std::memcpy(&body_[0], body, size); }
	HTTPBody(const std::string& body) { body_.reserve(0); body_.insert(body_.end(), body.begin(), body.end()); }

	//Operator =
	HTTPBody& operator=(const std::string& body);

	//Operator indexer[]
	byte& operator[](size_t i);
	byte operator[](size_t i) const;

	//Getter & Setter for body's size
	size_t getSize() const { return body_.size(); }
	void setSize(size_t size) { body_.resize(size); }

	//Starting & Ending of the body
	iterator begin() { return body_.begin(); }
	iterator end() { return body_.end(); }

	//Setter & getter for body
	void setBody(size_t size, byte* body) { body_.resize(size); std::memcpy(&body_[0], body, size); }
	void setBody(std::string& body) { body_.insert(body_.end(), body.begin(), body.end());}
	std::vector<byte>& getBody() { return body_; }
	std::vector<byte> getBody() const { return body_; }
	void clear() { body_.clear(); }

	//Display body to output stream
	void show(std::ostream& out = std::cout ) const;

	//toString and fromString functions
	std::string toString() const;
	static HTTPBody fromString(const std::string& strBody) { HTTPBody body(strBody); return body; }
protected:
	std::vector<byte> body_;
};

//Inherited HTTPbody rather than making it as private member
template <typename T>
class HTTPMessage : public HTTPBody {
public:
	using Key = std::string;
	using Value = std::string;
	using Attribute = std::string;
	using Attributes = std::unordered_map<Key, Value>;
	using Keys = std::vector<Key>;
	using FileSpec = std::string;

	//Constructors
	HTTPMessage() = default;
	
	//Getters & Setters for type, attributes and different (key,value) pairs
	T& getType();
	Attributes& getAttributes();
	void setAttribute(const Key& key, const Value& value);
	Keys getKeys() const;
	bool containsKey(const Key& key) const;
	static Key getKeyFromAtt(const Attribute& attribute);
	static Value getValueFromAtt(const Attribute& attribute);
	size_t getContentLength();
	void setContentLength(const size_t& size);
	std::string getName();
	void setName(const std::string& name);
	std::string getAction();
	void setAction(const std::string& action);
	EndPoint getTo();
	void setTo(EndPoint epTo);
	EndPoint getFrom();
	void setFrom(EndPoint epFrom);
	void clearAttributes();
	void clearMsg();

	//fromString & toString functions
	std::string toHeaderStr();
	std::string toFullStr();
	static HTTPMessage<T> fromStringMsg(const std::string& str);

	//Display message to ouput stream
	void showMessage(std::ostream& out = std::cout);

private:
	T type_;
	Attributes attributes_;
};

//Creating HTTP Request Messages
inline HTTPMessage<HTTPRequest> createHTTPRequestMessage(const HTTPRequest::HTTPMethod& method, const std::string& fileSpec){
	HTTPMessage<HTTPRequest> msg;
	HTTPRequest request(method, fileSpec);
	msg.getType() = request;
	return msg;
}

//Creating HTTP Response Messages
inline HTTPMessage<HTTPResponse> createHTTPResponseMessage(size_t statusCode) {
	HTTPMessage<HTTPResponse> msg;
	HTTPResponse response(statusCode);
	msg.getType() = statusCode;
	return msg;
}


template<typename T>
inline T& HTTPMessage<T>::getType(){
	return type_;
}

template<typename T>
inline typename HTTPMessage<T>::Attributes& HTTPMessage<T>::getAttributes() {
	return attributes_;
}

template<typename T>
inline void HTTPMessage<T>::setAttribute(const Key& key, const Value& value)
{
	attributes_[key] = value;
}

template<typename T>
inline typename HTTPMessage<T>::Keys HTTPMessage<T>::getKeys() const
{
	Keys keys;
	auto iter = attributes_.begin();
	while (iter != attributes_.end()) {
		keys.push_back(iter->first);
	}
	return keys;
}

template<typename T>
inline bool HTTPMessage<T>::containsKey(const Key& key) const
{
	auto iter = attributes_.find(key);
	if (iter == attributes_.end())
		return false;
	return true;
}

template<typename T>
inline typename HTTPMessage<T>::Key HTTPMessage<T>::getKeyFromAtt(const Attribute& attribute)
{
	size_t pos = attribute.find_first_of(':');
	if (0 <= pos && pos < attribute.length())
		return attribute.substr(0, pos);
	return "";
}

template<typename T>
inline typename HTTPMessage<T>::Value HTTPMessage<T>::getValueFromAtt(const Attribute& attribute)
{
	size_t pos = attribute.find_first_of(':');
    if (0 <= pos && pos < attribute.length())
      return attribute.substr(pos + 1, attribute.length() - pos);
    return "";
}

template<typename T>
inline size_t HTTPMessage<T>::getContentLength()
{
	if (containsKey("content-length"))
	{
		std::string lenStr = attributes_["content-length"];
		return Utilities::Converter<size_t>::toValue(lenStr);
	}
	else {
		if (containsKey("Content-Length")) {
			std::string lenStr = attributes_["Content-Length"];
			return Utilities::Converter<size_t>::toValue(lenStr);
		}
	}
	return 0;
}

template<typename T>
inline void HTTPMessage<T>::setContentLength(const size_t& size)
{
	attributes_["content-length"] = std::to_string(size);
}

template<typename T>
inline std::string HTTPMessage<T>::getName()
{
	auto iter = attributes_.find("name");
	if (iter != attributes_.end()) {
		return iter->second();
	}
	return "";
}

template<typename T>
inline void HTTPMessage<T>::setName(const std::string& name)
{
	attributes_["name"] = name;
}

template<typename T>
inline std::string HTTPMessage<T>::getAction()
{
	auto iter = attributes_.find("action");
	if (iter != attributes_.end()) {
		return std::stoi(iter->second());
	}
	return "";
}

template<typename T>
inline void HTTPMessage<T>::setAction(const std::string& action)
{
	attributes_["action"] = action;
}

template<typename T>
inline EndPoint HTTPMessage<T>::getTo()
{
	auto iter = attributes_.find("to");
	if (iter != attributes_.end()) {
		std::string temp = (iter->second());
		return EndPoint::fromString(temp);
	}
	return EndPoint();
}

template<typename T>
inline void HTTPMessage<T>::setTo(EndPoint epTo)
{
	attributes_["to"] = epTo.toString();
}

template<typename T>
inline EndPoint HTTPMessage<T>::getFrom()
{
	auto iter = attributes_.find("from");
	if (iter != attributes_.end()) {
		std::string temp = (iter->second());
		return EndPoint::fromString(temp);
	}
	return EndPoint();
}

template<typename T>
inline void HTTPMessage<T>::setFrom(EndPoint epFrom)
{
	attributes_["from"] = epFrom.toString();
}

template<typename T>
inline void HTTPMessage<T>::clearAttributes()
{
	attributes_.clear();
}

template<typename T>
inline void HTTPMessage<T>::clearMsg()
{
	attributes_.clear();
	body_.clear();
}

template<typename T>
inline std::string HTTPMessage<T>::toHeaderStr()
{
	std::string str = type_.toString(true);

	if (attributes_.size() > 0)
		str += "\n";

	for (auto kv : attributes_)
	{
		str += kv.first + ":" + kv.second + "\n";
	}

	str += "\n";

	return str;
}

template<typename T>
inline std::string HTTPMessage<T>::toFullStr()
{
	std::string temp = toHeaderStr();
	if (body_.size() > 0) {
		temp += (toString() + "\n");
	}
	
	return temp;
}

template<>
inline HTTPMessage<HTTPRequest> HTTPMessage<HTTPRequest>::fromStringMsg(const std::string& str)
{
	std::vector<std::string> splits = Utilities::StringHelper::split(str, '\n');
	if (splits.size() >= 1)
	{
		std::vector<std::string> cmdSplits = Utilities::StringHelper::split(splits[0], ' ');
		HTTPMessage<HTTPRequest> msg;
		if (cmdSplits.size() > 1)
		{
			
			msg.getType() = HTTPRequest::fromString(splits[0]);
			msg.getType().setfileSpec(cmdSplits[1]);
		}
		for (size_t i = 1; i < splits.size(); ++i)
		{
			if (splits[i].size() == 0)
				continue;
			Key key = getKeyFromAtt(splits[i]);
			Value value = getValueFromAtt(splits[i]);
			if (key.size() > 0)
			{
				msg.attributes_[key] = value;
			}
			else
			{
				HTTPBody body = HTTPBody::fromString(splits[i]);
				msg.body_ = body.getBody();
				msg.attributes_["content-length"] = Utilities::Converter<size_t>::toString(splits[i].size());
			}
		}
		return msg;
	}
	return HTTPMessage<HTTPRequest>();
}

template <>
inline HTTPMessage<HTTPResponse> HTTPMessage<HTTPResponse>::fromStringMsg(const std::string& str)
{
	std::vector<std::string> splits = Utilities::StringHelper::split(str, '\n');
	if (splits.size() > 1)
	{
		std::vector<std::string> cmdSplits = Utilities::StringHelper::split(splits[0], ' ');
		HTTPMessage<HTTPResponse> msg;
		if (cmdSplits.size() > 1)
		{
			msg.getType() = HTTPResponse::fromString(splits[0]);
			size_t statusNum = Utilities::Converter<size_t>::toValue(cmdSplits[1]);
			msg.getType().setStatusCode(statusNum);
		}
		for (size_t i = 1; i < splits.size(); ++i)
		{
			if (splits[i].size() == 0)
				continue;
			Key key = getKeyFromAtt(splits[i]);
			Value value = getValueFromAtt(splits[i]);
			if (key.size() > 0)
			{
				msg.attributes_[key] = value;
			}
			else
			{
				HTTPBody body = HTTPBody::fromString(splits[i]);
				msg.body_ = body.getBody();
				msg.attributes_["content-length"] = Utilities::Converter<size_t>::toString(splits[i].size());
			}
		}
		return msg;
	}
	return HTTPMessage<HTTPResponse>();
}

template<typename T>
inline void HTTPMessage<T>::showMessage(std::ostream& out)
{
	std::string temp = toFullStr();  // convert this HttpRequestMessage to string
	size_t pos = temp.find_last_of('\n');
	if (pos < temp.size())
	{
		temp = Utilities::StringHelper::trim(temp);
	}
	out << "\n" << temp;
}


