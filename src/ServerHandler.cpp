#include "ServerHandler.hpp"

ServerHandler::ServerHandler(){
	return ;
}

ServerHandler::ServerHandler(std::vector<ServerBlock> serverBlocks){
	this->_serverBlocks = serverBlocks;
	this->_max_sd = 0;
}

ServerHandler::ServerHandler(ServerHandler const &inst){
	*this = inst;
}

ServerHandler&	ServerHandler::operator=(ServerHandler const &inst) {
	if (this != &inst) {

		this->_serverBlocks = inst._serverBlocks;
		this->_listen_set = inst._listen_set;
		this->_max_sd = inst._max_sd;
		this->_read_set = inst._read_set;
		this->_servers_map = inst._servers_map;
		this->_clients_map = inst._clients_map;
	}
	return *this;
}

ServerHandler::~ServerHandler() {
	return ;
}

void	ServerHandler::initHandler() {
	// std::cout << "initiate servers" << std::endl;
	std::vector<ServerBlock>::iterator s_it = this->_serverBlocks.begin();
	for (; s_it != this->_serverBlocks.end(); s_it++) {
		if (s_it->manageSocket() == false) {
			clearSocket();
		}
	}
}

void	ServerHandler::startServerHandler() {
		fd_set working_read;
		// struct timeval timeout;
		int s_ready;
		
		// timeout.tv_sec = 60;
		// timeout.tv_usec = 0;b

		listenNewConnection();
		
		do {
			std::memcpy(&working_read, &_read_set, sizeof(_read_set));

			std::cout << RED << "Waiting on select..." << DEFAULT << std::endl;
			s_ready = select(_max_sd + 1, &working_read, NULL, NULL, NULL);
			if (s_ready < 0) {
				std::cerr << RED << "Error: select failed..." << DEFAULT << std::endl;
				break;
			}
			if (s_ready == 0) {
				std::cerr << GREEN << "select timeout: server is shuting down..." << DEFAULT << std::endl;
				gracefulShutdown();
				exit(0);
			}
			checkStates(&s_ready, working_read);
			clientTimeout();
		}while (true);

}

void	ServerHandler::checkStates(int *s_ready, fd_set &working_read) {
	for (int i = 0; i <= _max_sd; i++)
	{
		if (FD_ISSET(i, &working_read)) {
			if (FD_ISSET(i, &_listen_set))
				readytoAccept(i);
			else{
				if (httpManage(i) == true)
					closeConn(i);
				(*s_ready)--;
			}
		} 
	}
	(void)s_ready;
		//   checkTimeout();
}

void	ServerHandler::readytoAccept(int listen_sd) {
	int new_sd;
	struct sockaddr_in c_addr;
	socklen_t c_addr_len = sizeof(c_addr);

	new_sd = accept(listen_sd, (struct sockaddr *)&c_addr, &c_addr_len);
	if (new_sd < 0)
		return ;
		
	if (fcntl(new_sd ,F_SETFL, O_NONBLOCK) < 0) {
		std::cerr << RED << "Error: cannot set socket [" << new_sd << "]" << "to be non-blocking..." << DEFAULT << std::endl;
		clearMasterSet(new_sd, &_read_set);
		close(new_sd);
		return ;
	}

	_clients_map[new_sd] = new Client (new_sd, listen_sd, _serverBlocks);
	// _clients_map[new_sd] = new_client;
	std::cout << RED << "socket at init proc: " << _clients_map[new_sd]->_socket << DEFAULT << std::endl;
	std::cout << RED << "state at init proc: " << _clients_map[new_sd]->request->_stage << DEFAULT << std::endl;


	addMasterSet(new_sd, &_read_set);
	usleep(200);
	std::cout << GREEN << "Accept new connection on socket: [" << new_sd << "]" << std::endl;
}

bool    ServerHandler::httpManage(int read_sd) {
        char buffer[READ_BUFF];
		int rc = 0;


		while(true){
			rc = recv(read_sd, buffer, sizeof(buffer), 0);
			if (rc < 0) {
				break ;
				// return (false);
			}
			if (rc == 0) {
				std::cout << "Close conn at read request" << std::endl;
				ft_memset(buffer, 0, sizeof(buffer));//Clear buffer
				return (true);
				// break ;
			}
			buffer[rc] = 0;
			// _clients_map[read_sd]->request->data.write(buffer, rc);
			_clients_map[read_sd]->request->working_data.write(buffer, rc);
			std::cout << YELLOW << "Read request on socket [" << read_sd << "]" << std::endl;
			std::cout << YELLOW << "----- Request -----\n" << buffer << DEFAULT << std::endl;
		}
		_clients_map[read_sd]->request->server_blocks = &_serverBlocks;
		ft_memset(buffer, 0, sizeof(buffer));//Clear buffer
		std::cout << "Client [" << read_sd << "] " << std::endl;
		// std::cout << "Full Request: \n" << _clients_map[read_sd]->request->working_data.str() << std::endl;
		_clients_map[read_sd]->request->data << _clients_map[read_sd]->request->working_data.rdbuf(); 
		if(_clients_map[read_sd]->httpStage() == false) {
			return (false);
		}
        return (true);
}

void	ServerHandler::updateLocationPath(int read_sd) {

	if(_clients_map[read_sd]->_cgi.currpath != this->_currpath)	
		this->_currpath = _clients_map[read_sd]->_cgi.currpath;
	_clients_map[read_sd]->_cgi.currpath.clear();
}

void	ServerHandler::listenNewConnection() {
	FD_ZERO(&_listen_set);
	FD_ZERO(&_read_set);

	std::vector<ServerBlock>::iterator s_it = this->_serverBlocks.begin();
	for (; s_it != this->_serverBlocks.end(); s_it++) {
		if (listen(s_it->getSocket(), MAX_CON) < 0) {
			std::cerr << RED << "Error: listening on socket [";
			std::cout << s_it->getSocket() << "]" << DEFAULT << std::endl;
			clearSocket();
		}

		// if (fcntl(s_it->getSocket(), F_SETFL, O_NONBLOCK) < 0) {
		// 	std::cerr << RED << "Error: listening on socket [";
		// 	std::cout << s_it->getSocket() << "]" << DEFAULT << std::endl;
		// 	clearSocket();
		// }

		if (this->_servers_map.count(s_it->getSocket()) == 0) {
			this->_servers_map.insert(std::pair<int, ServerBlock>(s_it->getSocket(), *s_it));
		}
		addMasterSet(s_it->getSocket(), &_listen_set);
		addMasterSet(s_it->getSocket(), &_read_set);
		std::cout << GREEN << "Starting server on : port[" << s_it->getPortNumb() << "] "; 
		std::cout << "socket [" << s_it->getSocket() << "]" << DEFAULT << std::endl;
	}
	_max_sd = _serverBlocks.back().getSocket();
	// std::cout << "max_sd: " << _max_sd << std::endl;
}

//************************************************
//Manage Socket, Fd, Connections
//************************************************

void	ServerHandler::addMasterSet(int new_sock, fd_set *masterset) {
	FD_SET(new_sock, masterset);
	if (new_sock > _max_sd)
		_max_sd = new_sock;
}

void	ServerHandler::clearMasterSet(int socket, fd_set *master_set) {
	FD_CLR(socket, master_set);
	int new_max = 0;

	 if (socket == _max_sd) {
		  for (int i = 0; i < _max_sd; i++) {
				if (FD_ISSET(i, &_read_set))
					 new_max = i;
		  }
		  _max_sd = new_max;
	 }
	 delete _clients_map[socket]->request;
	 delete _clients_map[socket];
}

void	ServerHandler::clearSocket() {
	std::vector<ServerBlock>::iterator s_it = this->_serverBlocks.begin();
	for (; s_it != this->_serverBlocks.end(); s_it++) {
		close (s_it->getSocket());
	}
	std::cout << "Shut down...\n" << std::endl;
	exit(-1);
}

void	ServerHandler::closeConn(int socket){
	bool  on = false;
	if (FD_ISSET(socket, &_listen_set)) {
		clearMasterSet(socket, &_listen_set);
		on = true;
	}
	if (FD_ISSET(socket, &_read_set)) {
		clearMasterSet(socket, &_read_set);
		on = true;
	}
	if (on == true) {
		close(socket);
		std::cout << GREEN << "Close client connection on : socket [" << socket << "] !";
		std::cout << DEFAULT << std::endl;
	}
	//  _clients_map.erase(fd);
}

void	ServerHandler::gracefulShutdown(){
	std::vector<ServerBlock>::iterator s_it = _serverBlocks.begin();
	int client_sd;
	for (; s_it != _serverBlocks.end(); s_it++) {
		close (s_it->getSocket());
		std::cout << GREEN << "Close listening connection on : socket [";
		std::cout << s_it->getSocket() << "] !" << DEFAULT << std::endl;
		client_sd = s_it->getSocket();
	}

	client_sd += 1;
	for (; client_sd <= _max_sd; client_sd++) {
		closeConn(client_sd);
	}
	std::cout << YELLOW << "Shutdown server good bye...\n" << std::endl;
}

void	ServerHandler::clientTimeout(){
	//  for (std::map<int, Client>::iterator cit = _clients_map.begin(); cit != _clients_map.end(); cit++)
	//  {
	// 	  if (time(NULL) - cit->second.getTime() > CONNECTION_TIMEOUT)
	// 	  {
	// 			std::cout << YELLOW << "Client " << cit->first << " Timeout, closing connection" << RESET << std::endl;
	// 			closeConnection(cit->first);
	// 			return ;
	// 	  }   
	//  }
	return ;
}
