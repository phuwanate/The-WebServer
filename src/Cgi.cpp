#include "Cgi.hpp"
#include "utils.hpp"

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
            if (serveFile(server, location) == false)
                _resp.error404(_socket, _req.setDefaultErrorPage());
        return RESPONSED;
    }
    //location wasn't found
    if (serveFile(server, location) == false)
        _resp.error404(_socket, _req.setDefaultErrorPage());
    return RESPONSED;
    // serveFile();
    // _resp.byFile(_socket, 200, "./page-copy.html", "text/html; charset=UTF-8"); // test response
}

bool    Cgi::serveFile(ServerBlock &server, LocationBlock &location){

    //need root path + dirpath
    std::string root;
    std::string endpoint;
    std::vector<std::string> index;

    if (location.getRoot().length() != 0) {
        root = location.getRoot();
    } else {
        root = server.getRoot();
    }
    if (location.getDirectoryPath().length() != 0)
        endpoint = location.getDirectoryPath();
    
    std::string filepath = "./" + root + endpoint;

    if (isFileExists(filepath) != false) {
        std::cout << "This filepath:" << filepath << "is exists." << std::endl;
        if (isDir(filepath)) {
            std::cout << "This filepath:" << filepath << "is directory." << std::endl;
        //if dirpath == directory
        // get index to concat with path
            if (location.getIndex().size() != 0)
                index = location.getIndex();
            else {
                //Check autoindex in location block before use server's index
                if (location.getAutoIndex() == true) {
                    //resp.byAutoindex();
                    return true;
                }
                index = server.getIndex();
                //No need to fill index line in config file.
                if (index.size() == 0) {
                    return false;
                }

            }
            std::vector<std::string>::iterator it = index.begin();
            for (; it != index.end(); it++){
                // std::cout << "file test:" << filepath + *it << std::endl;
                if (isFileExists(filepath + *it))
                {
                    filepath += *it;
                    break;
                }
            }//end of for loop
            if (it == index.end())
            {
                // std::cout << "index file doesn't exists." << std::endl;
                return false;
            }
        }//end of if (isFileExists(filepath))
        if (isFileExists(filepath) == false) {
			if (server.getAutoindex() == true) {
                //resp.byAutoindex();
            }
            else {
                return false; //autoindex is off reponse 404 default page.
            }
		}
        // std::cout << "This filepath:" << filepath << " is a file." << std::endl;
    }
    //if dirpath == file
    // get content-type from file
    // response by file
    _resp.byFile(_socket, 200, "./page-copy.html", "text/html; charset=UTF-8"); // test response
    return true;
}

bool Cgi::isFileExists(const std::string& filepath) {
    struct stat buffer;
    return (stat(filepath.c_str(), &buffer) == 0);
}

bool Cgi::isDir(const std::string& filepath) {
    struct stat buffer;
    if (stat(filepath.c_str(), &buffer) == 0) {
        return S_ISDIR(buffer.st_mode);
    }
    return false;
}