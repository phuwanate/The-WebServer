#ifndef CGI_HPP
# define CGI_HPP

#include "HttpStatus.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

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

        int                             getErrnum();
        void                            setErrnum(int val);
        void                            setSocket(int val);
        void    setContentTypes();
        void    initCgi(int errnum, int socket, std::vector<ServerBlock>* server_blocks, Request &reqinst);

        std::string                     getType(std::string key);
        
        HttpStage                       apiRouter();
        time_t                          startTime();
        pid_t                           waitcoc(pid_t child, int *status);
        void                            execute(std::string &file, std::string &compiler, std::string &root);
        void                            cgiResponse(pid_t exitpid, int status, std::string cgi_route, std::stringstream &stream);
        bool                            generatePage(LocationBlock &location, ServerBlock &server);
        bool                            upload(LocationBlock &location, ServerBlock &server);
        bool                            Delete(LocationBlock &location, ServerBlock &server);
        long                            creatFileStream();
        bool                            serveFile(ServerBlock &server, LocationBlock &location);
        bool                            isFileExists(const std::string& file);
        bool                            isDir(const std::string& filepath);
        bool                            hasPermission(const std::string& filepath, int mode);
        bool                            isIndexExists(std::string &filepath, std::vector<std::string> index, LocationBlock &location);
        std::string                     checkContentType(std::string file);
        bool                            useServerparameter(std::string &filepath, ServerBlock &server, LocationBlock &location);
        bool    prepareFilePath(ServerBlock &server, LocationBlock &location, std::string &root, std::string &endpoint, std::string &filepath);
        
        std::vector<ServerBlock>*           server_blocks;
        std::string                         currpath;

    private:
        Request                             _req;
        Response                            _resp;
        int                                 _errnum;
        std::map<std::string, std::string>  _contentTypes;
	    int				                    _socket;
        std::string                         _truePath;

};

#endif