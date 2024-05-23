#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <sstream>
#include <iostream>
#include <string>
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
		Client(int new_sd, int listen_sd, std::vector<ServerBlock>  serverBlocks);
		Client &operator=(Client const &cli);
		~Client();
		bool						receiveData(const char *buffer, int rc);
		bool						httpStage();

		Request						request;
		int							sever_socket;
		std::vector<ServerBlock>	server_blocks;
		Cgi							_cgi;

		
	private:
		Response					_response;
		int							_socket;
		HttpStage					_stage;

};

#endif