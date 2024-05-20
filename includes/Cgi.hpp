#ifndef CGI_HPP
# define CGI_HPP

#include "HttpStatus.hpp"
#include <iostream>
#include <unistd.h>

#include "Request.hpp"
#include "Response.hpp"

class Request;
class Response;
class ServerBlock;

class Cgi {

    public:
        Cgi();
        Cgi(Cgi const &inst);
        Cgi &operator=(Cgi const &inst);
        ~Cgi();

        void                            setErrnum(int val);
        void                            setSocket(int val);
        void    initCgi(int errnum, int socket, std::vector<ServerBlock>* server_blocks, Request &reqinst);

        HttpStage                       apiRouter();
        void                            serveFile();
        std::vector<ServerBlock>*       server_blocks;

    private:
        Request                         _req;
        Response                        _resp;
        int                             _errnum;
	int				_socket;


};

#endif