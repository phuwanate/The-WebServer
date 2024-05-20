#ifndef CGI_HPP
# define CGI_HPP

#include "HttpStatus.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "Request.hpp"
#include "Response.hpp"

class Request;
class Response;
class ServerBlock;
class LocationBlock;

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
        bool                            serveFile(ServerBlock &server, LocationBlock &location);
        bool                            isFileExists(const std::string& file);
        bool                            isDir(const std::string& filepath);
        std::vector<ServerBlock>*       server_blocks;

    private:
        Request                         _req;
        Response                        _resp;
        int                             _errnum;
	int				_socket;


};

#endif