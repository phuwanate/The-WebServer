#ifndef SERVERHANDLER_HPP
# define SERVERHANDLER_HPP

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>


#include "Config.hpp"
#include "LocationBlock.hpp"
#include "ServerBlock.hpp"
#include "Client.hpp"
#include "Request.hpp"
#include "Response.hpp"
// #include "mock.hpp"

#define SERVER_PORT  12345

#define TRUE	1
#define FALSE	0
#define TIMEOUT 6

class Config;
class ServerBlock;
class Client;
class ServerHandler {

	public:
		ServerHandler();
		ServerHandler(std::vector<ServerBlock> serverBlocks);
		ServerHandler(ServerHandler const &inst);
		ServerHandler &operator= (ServerHandler const &inst);
		~ServerHandler();

		void						initHandler();
		void						startServerHandler();
		void						listenNewConnection();
		void						addMasterSet(int socket, fd_set *masterset);
		void						clearSocket();
		void						checkStates(int *s_ready, fd_set &working_read);
		void						readytoAccept(int read_sd);
		bool						httpManage(int read_sd);
		void						clearMasterSet(int socket, fd_set *master_set);
		void						closeConn(int socket);
		void						gracefulShutdown();
		void						clientTimeout();
		void						updateLocationPath(int read_sd);

		std::map<int, ServerBlock>	_servers_map;

	private:
		std::vector<ServerBlock>	_serverBlocks;
		int							_max_sd;
		fd_set						_listen_set;
		fd_set						_read_set;
		std::map<int, Client*>		_clients_map;
		Response					_response;
		std::string                 _currpath;
		std::string					_newpath;
};

#endif