#include "Cgi.hpp"
#include "utils.hpp"
// #include "ServerBlock.hpp"

Cgi::Cgi():_errnum(0) {

}

Cgi::Cgi(Cgi const &inst) {

    *this = inst;
}

Cgi &Cgi::operator=(Cgi const &inst) {

    if (this != &inst) {
        this->_errnum = inst._errnum;
        this->_req = inst._req;
        this->_resp = inst._resp;
    }
    return *this;
}

Cgi::~Cgi() {

}

void     Cgi::setErrnum(int val) {

    this->_errnum = val;
}

void     Cgi::setSocket(int val) {

    this->_socket = val;
}

void    Cgi::initCgi(int errnum, int socket, std::vector<ServerBlock>* instptr, Request &reqinst) {
 
    this->_errnum = errnum;
    this->_socket = socket;
    this->server_blocks = instptr;
    this->_req = reqinst;
}

HttpStage Cgi::apiRouter() {
    LocationBlock location = searchLocation(_req.header["Host"], _req.path, *server_blocks);
    ServerBlock server = searchServer(_req.header["Host"], *server_blocks);
    
    if (server.getServerName().length() == 0) {
        _resp.byStatus(_socket, 400);
        return RESPONSED;
    }
    if (_errnum != 0 && _errnum != 404) {
        _resp.byStatus(_socket, _errnum);
        return RESPONSED;
    }
    if (location.getDirectoryPath().length() != 0) {
        bool method_allow = location.getAllowMethods()[_req.method];
        if (method_allow == false) {
            _resp.byStatus(_socket, 405);
        }
        else if (location.getDirectoryPath() == "/cgi") {
            //Generate page with cgi
        }
        else if (location.getDirectoryPath() == "/upload") {
            //Upload file
        }
        else if (location.getDirectoryPath() == "/delete") {
            //Delete file
        }
        else if (location.getAlias().length() != 0) {
            //Redirect
        }
        else
            serveFile();
        return RESPONSED;
    }
    serveFile();
    return RESPONSED;
    // serveFile();
    // _resp.byFile(_socket, 200, "./page-copy.html", "text/html; charset=UTF-8"); // test response
}

void    Cgi::serveFile(){
    _resp.byFile(_socket, 200, "./page-copy.html", "text/html; charset=UTF-8"); // test response
    return ;
}