#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <sstream>
#include <sys/socket.h>
#include <map>
#include <fstream>
#include <iostream>
#include "HttpStatus.hpp"

class Response {
	public:
		Response();
		void byStatus(int socket, int status);
		void byStringstream(int socket, int status, std::stringstream &file, const std::string &content_type);
		void byFile(int socket, int status, std::string const &location,  std::string const &content_type);
		void byRedirect(int socket, int status, std::string const &location);
		void error404(int socket, std::string location404);
	private:
		std::map<int, std::string>		_status;

		std::string						createFirstLine(int status);
		std::string						createBodyByStatus(int status);
		std::string 					createResponse(std::string firstLine, std::string body, std::string contentType);
		bool							readFile(std::string file_location, std::string& buffer);
};

#endif