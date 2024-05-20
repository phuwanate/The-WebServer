#ifndef HTTPSTATUS_HPP
# define HTTPSTATUS_HPP

#include <map>

enum HttpStage {
	FIRST_LINE,
	HEADER,
	BODY,
	RESPONSED,
	ROUTER
};

std::map<int, std::string> initStatusrMsg();

#endif