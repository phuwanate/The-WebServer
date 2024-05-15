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

ServerHandler& ServerHandler::operator=(ServerHandler const &inst) {

   if (this != &inst) {

      this->_serverBlocks = inst._serverBlocks;
      this->_listen_set = inst._listen_set;
      this->_max_sd = inst._max_sd;
      this->_read_set = inst._read_set;
      this->_write_set = inst._write_set;
      this->_servers_map = inst._servers_map;
   }
   return *this;
}

ServerHandler::~ServerHandler() {

   return ;
}

void    ServerHandler::initHandler() {

   std::cout << "initiate servers" << std::endl;
   std::vector<ServerBlock>::iterator s_it = this->_serverBlocks.begin();
   for (; s_it != this->_serverBlocks.end(); s_it++) {
      if (s_it->manageSocket() == false) {
         clearSocket();
      }
   }
}

void    ServerHandler::clearSocket() {

   std::vector<ServerBlock>::iterator s_it = this->_serverBlocks.begin();
   for (; s_it != this->_serverBlocks.end(); s_it++) {
      close (s_it->getSocket());
   }
   std::cout << "Shut down...\n" << std::endl;
   exit(-1);
}

void    ServerHandler::startServerHandler() {

      fd_set   working_read, working_write;
      struct timeval timeout;
      int      s_ready;
      
      timeout.tv_sec = 60;
      timeout.tv_usec = 0;

      listenNewConnection();

      do {
         ft_memcpy(&working_read, &_read_set, sizeof(_read_set));
         ft_memcpy(&working_write, &_write_set, sizeof(_write_set));

         std::cout << RED << "Waiting on select..." << DEFAULT << std::endl;
         s_ready = select(_max_sd + 1, &working_read, &working_write, NULL, &timeout);
         if (s_ready < 0)
         {
            std::cerr << RED << "Error: select failed..." << DEFAULT << std::endl;
            break;
         }
         if (s_ready == 0)
         {
            std::cerr << GREEN << "select timeout: server is shuting down..." << DEFAULT << std::endl;
            break;
         }

         checkStates(&s_ready, working_read, working_write);

      }while (true);
}

void    ServerHandler::checkStates(int *s_ready, fd_set &working_read, fd_set &working_write) {

   for (int i = 0; (i <= _max_sd); ++i)
   {            
      if (FD_ISSET(i, &_listen_set))
      {
          readytoAccept(i);
      } else if (FD_ISSET(i, &working_read)) {
          readRequest(i);
            s_ready--;
      } else if (FD_ISSET(i, &working_write)) {
          writeResponse(i);
            s_ready--;
      }
   }
   (void)s_ready;
      //   checkTimeout();
}

void ServerHandler::readytoAccept(int listen_sd) {
   
   int                  new_sd;
   struct sockaddr_in   c_addr;
   socklen_t            c_addr_len = sizeof(c_addr);

   new_sd = accept(listen_sd, (struct sockaddr *)&c_addr, &c_addr_len);
   if (new_sd < 0)
        return ;
      
   if (fcntl(new_sd ,F_SETFL, O_NONBLOCK) < 0) {

		std::cerr << RED << "Error: cannot set socket [" << new_sd << "]" << "to be non-blocking..." << DEFAULT << std::endl;
      clearMasterSet(new_sd, &_read_set);
      close(new_sd);
		return ;
	}

   addMasterSet(new_sd, &_read_set);
   std::cout << GREEN << "Accept new connection on socket: [" << new_sd << "]" << std::endl;
   return ;
}

void ServerHandler::readRequest(int read_sd) {

   char buffer[READ_BUFF];

   int rc = recv(read_sd, buffer, sizeof(buffer), 0);
   if (rc > 0) {
      std::cout << YELLOW << "Request: " << buffer << DEFAULT << std::endl;
      clearMasterSet(read_sd, &_read_set);
      addMasterSet(read_sd, &_write_set);
   }

   if (rc == 0) {
        std::cout << GREEN << "Socket : [" << read_sd << "] Closed connection!" << DEFAULT << std::endl;
        ft_memset(&buffer, 0, sizeof(buffer));
        closeConn(read_sd);
    }
   return ;
}

void ServerHandler::writeResponse(int write_sd) {

   t_res resp;

   build_response(resp);

   send(write_sd, resp.httpResponse, strlen(resp.httpResponse), 0);
   send(write_sd, resp.htmlContent.c_str(), resp.htmlContent.length(), 0);

   closeConn(write_sd);
   return ;
}

void    ServerHandler::listenNewConnection() {

   FD_ZERO(&_listen_set);
   FD_ZERO(&_read_set);
   FD_ZERO(&_write_set);

   std::vector<ServerBlock>::iterator s_it = this->_serverBlocks.begin();
   for (; s_it != this->_serverBlocks.end(); s_it++) {
        if (listen(s_it->getSocket(), MAX_CON) < 0) {
		      std::cerr << RED << "Error: listening on socket [" << s_it->getSocket() << "]" << DEFAULT << std::endl;
            clearSocket();
        }

        if (fcntl(s_it->getSocket(), F_SETFL, O_NONBLOCK) < 0) {
            std::cerr << RED << "Error: listening on socket [" << s_it->getSocket() << "]" << DEFAULT << std::endl;
            clearSocket();
        }

        if (this->_servers_map.count(s_it->getSocket()) == 0) {
            this->_servers_map.insert(std::pair<int, ServerBlock>(s_it->getSocket(), *s_it));
        }
        addMasterSet(s_it->getSocket(), &_listen_set);
        addMasterSet(s_it->getSocket(), &_read_set);
        std::cout << GREEN << "Listening on socket: [" << s_it->getSocket() << "]" << DEFAULT << std::endl;
   }
   _max_sd = _serverBlocks.back().getSocket();
   std::cout << "max_sd: " << _max_sd << std::endl;
}

void ServerHandler::addMasterSet(int new_sock, fd_set *masterset) {

   FD_SET(new_sock, masterset);
   if (new_sock > _max_sd)
      _max_sd = new_sock;
}

void  ServerHandler::clearMasterSet(int socket, fd_set *master_set) {

   FD_CLR(socket, master_set);
   int new_max;

    if (socket == _max_sd)
    {
        for (int i = 0; i < _max_sd; i++)
        {
            if (FD_ISSET(i, &_write_set))
                new_max = i;
            if (FD_ISSET(i, &_read_set))
                new_max = i;
        }
        _max_sd = new_max;
    }
}

void    ServerHandler::closeConn(int socket)
{
    if (FD_ISSET(socket, &_listen_set))
        clearMasterSet(socket, &_listen_set);

    if (FD_ISSET(socket, &_read_set))
        clearMasterSet(socket, &_read_set);

    if (FD_ISSET(socket, &_write_set))
        clearMasterSet(socket, &_write_set);

    close(socket);
   //  _clients_map.erase(fd);
}
