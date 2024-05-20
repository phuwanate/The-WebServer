#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <sstream>
#include <iostream>
#include <string>
#include "Request.hpp"
#include "ServerHandler.hpp"
#include "HttpStatus.hpp"
#include "Response.hpp"

class Request;
class Response;
class ServerBlock;

class Client {
	public:
		Client();
		Client(Client const &inst);
		Client(int new_sd, int listen_sd, std::vector<ServerBlock>  serverBlocks);
		Client &operator=(Client const &cli);
		~Client();
		bool						receiveData(const char *buffer, int rc);

		Request						request;
		int							sever_socket;
		std::vector<ServerBlock>	server_blocks;
		
	private:
		Response					_response;
		int							_socket;
		HttpStage					_stage;

		bool						httpStage();
};

#endif