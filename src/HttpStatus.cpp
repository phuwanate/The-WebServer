#include "HttpStatus.hpp"

std::map<int, std::string> initStatusrMsg() {
	std::map<int, std::string> statusMsg;

	statusMsg[200] = "OK"; //no error
	statusMsg[201] = "Created"; // first
	statusMsg[307] = "Temporary Redirect"; 
	statusMsg[400] = "Bad Request";
	statusMsg[404] = "Not Found"; //first
	statusMsg[405] = "Method Not Allowed"; //karn
	statusMsg[413] = "Content Too Large"; // > body max, karn
	statusMsg[422] = "Unprocessable Content";
	statusMsg[502] = "Bad Gateway";
	
	return (statusMsg);
}