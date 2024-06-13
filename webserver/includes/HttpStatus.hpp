#ifndef HTTPSTATUS_HPP
# define HTTPSTATUS_HPP

#include <map>
#include <string>

enum HttpStage {
	FIRST_LINE,
	HEADER,
	BODY,
	RESPONSED,
	ROUTER,
	ERROR,
};

std::map<int, std::string> initStatusrMsg();

#endif