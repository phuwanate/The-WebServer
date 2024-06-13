#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <sstream>
#include <iostream>
#include <string>
#include <ctime>
#include "Request.hpp"
#include "ServerHandler.hpp"
#include "HttpStatus.hpp"
#include "Response.hpp"
#include "Cgi.hpp"

class Request;
class Response;
class ServerBlock;
class Cgi;

class Client {
	public:
		Client();
		Client(Client const &inst);
		Client(int new_sd, int listen_sd, std::vector<ServerBlock>  &serverBlocks);
		Client &operator=(Client const &cli);
		~Client();

		bool						httpStage();
		time_t 						getWorkingTime();
		void 						updateWorkingTime();

		Request*					request;
		int							sever_socket;
		std::vector<ServerBlock>	server_blocks;
		Cgi							_cgi;
		int							_socket;
		HttpStage					_stage;

		
	private:
		Response					_response;
		time_t						_workingTime;

};

#endif