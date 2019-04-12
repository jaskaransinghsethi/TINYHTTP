#include "Message.h"


std::string HTTPResponse::getStatusRes()
{
	auto iter = statusRes_.find(statusCode_);
	if (iter != statusRes_.end())
		return iter->second;
	return "";
}
