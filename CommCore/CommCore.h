#pragma once

#include "../Message/Message.h"
#include "../Sockets/Sockets.h"

class CommCore
{
public:
	//Constructors
	CommCore(Sockets::Socket* socket) : pSocket_(socket) { }
	CommCore() : pSocket_(nullptr) { }

	//Destructors
	virtual ~CommCore() { }

	//Setter for socket
	void setSocket(Sockets::Socket* socket) { pSocket_ = socket; }
	
	//Get message from the socket
	template <typename T>
	HTTPMessage<T> getMessage();
	
	//Post message to the socket
	template <typename T>
	void postMessage(HTTPMessage<T> msg);

private:
	Sockets::Socket* pSocket_;
};

//Not yet Implemented
template<typename T>
inline HTTPMessage<T> CommCore::getMessage()
{
	std::string msg;
	while (pSocket_->validState()) {
		std::string temp = pSocket_->recvString('\n');
		msg += temp;
		if (temp.length() < 3 || !pSocket_->validState())
			break;
	}
	
	HTTPMessage<T> httpMsg = HTTPMessage<T>::fromStringMsg(msg);


}

//Not yet Implemented
template<typename T>
inline void CommCore::postMessage(HTTPMessage<T> msg)
{
}
