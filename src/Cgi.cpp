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

void    Cgi::setContentTypes() {

    this->_contentTypes["html"] = "text/html; charset=UTF-8";
    this->_contentTypes["htm"] = "text/html; charset=UTF-8";
    this->_contentTypes["css"] = "text/css";
    this->_contentTypes["jpg"] = "image/jpeg";
    this->_contentTypes["jpeg"] = "image/jpeg";
    this->_contentTypes["webp"] = "image/webp";
    this->_contentTypes["js"] = "application/x-javascript";
}

std::string Cgi::getType(std::string key) {

    return this->_contentTypes[key];
}

void    Cgi::initCgi(int errnum, int socket, std::vector<ServerBlock>* instptr, Request &reqinst) {
 
    this->_errnum = errnum;
    this->_socket = socket;
    this->server_blocks = instptr;
    this->_req = reqinst;
    setContentTypes();
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
    std::string filepath;

    //need to know is it a full path with file_name or it just a directory.
    if (checkContentType(_req.path) == "unkown") {
        if (location.getRoot().length() != 0) {
            root = location.getRoot();
        } else {
            root = server.getRoot();
        }
        if (root[root.size() - 1] != '/') {
            root += "/";
        }
        if (location.getDirectoryPath().length() != 0) {
            endpoint = location.getDirectoryPath();
            if (endpoint[endpoint.size() - 1] != '/') {
                endpoint += "/";
            }
        }
        
        if (endpoint[0] == '/')
            filepath = "./" + root.substr(0, root.find_last_of("/")) + endpoint;
        else
            filepath = "./" + root + endpoint;
        // std::string filepath = "./" + root + endpoint;
    }
    else
        filepath = "." + _req.path;
    std::cout << filepath << std::endl;

    if (isFileExists(filepath) == true) {
        std::cout << "This filepath:" << filepath << " does exists." << std::endl;
        if (isDir(filepath)) {
            std::cout << "This filepath:" << filepath << "is directory." << std::endl;
        //if dirpath == directory
        // get index to concat with path
            if (location.getIndex().size() != 0) {
                std::cout << YELLOW << "Use location parameters." << DEFAULT << std::endl;
                index = location.getIndex();
            }
            if (isIndexExsists(filepath, index) == false) {
                if (location.getAutoIndex() == true) {
                    //resp.byAutoindex();
                    return true;
                }
                else {
                    if (useServerparameter(server) == true)
                        return true;
                    else
                        return false;
                }
            } else {
                //found index in location block and can be use;
                std::cout << YELLOW << "Healthy request by location block." << DEFAULT << std::endl; 
                std::string contentTypes = checkContentType(filepath);
                _resp.byFile(_socket, 200, filepath, contentTypes);
                return true;
            }
        }
        std::cout << "This filepath:" << filepath << " is a file." << std::endl;
        std::string contentTypes = checkContentType(filepath);
        _resp.byFile(_socket, 200, filepath, contentTypes);
        return true;
    }//end of if (isFileExists(filepath))
    return false;
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

bool Cgi::isIndexExsists(std::string &filepath, std::vector<std::string> index) {

    std::vector<std::string>::iterator it = index.begin();
    for (; it != index.end(); it++) {
        if (isFileExists(filepath + *it))
        {
            filepath += *it;
            return true;
        }
    }
    return false;
}

std::string Cgi::checkContentType(std::string file) {

    std::string type = "unkown";
    size_t      found = file.find_last_of('.');
    std::string file_ext;
    
    if (found == std::string::npos)
        return type;
    
    file_ext = file.substr(found + 1);
    type = getType(file_ext);
    return type;
}



bool Cgi::useServerparameter(ServerBlock &server){

    //in location path cannot found index, go back to use server_index.
    std::string serv_root = server.getRoot();
    std::vector<std::string> index;
    std::string filepath;
    
    std::cout << YELLOW << "Going back to outter block for server paramters." << DEFAULT << std::endl;
    if (serv_root[serv_root.size() - 1] != '/')
        serv_root += "/";
    index = server.getIndex();
    filepath = "./" + serv_root;
    std::cout << "filepath server : " << filepath << std::endl;
    if (isIndexExsists(filepath, index) == false) {
        //there is no index exists in server_root check if autoindex = on
        std::cout << "filepath server : " << filepath + index[0] << "does not exists." << std::endl;
    
        if (server.getAutoindex() == true) {
            //resp.byAutoindex();
            return true;
        }
        else {
            //autoindex = off go to response error default
            return false;
        }
    }
    std::string contentTypes = checkContentType(filepath);
    _resp.byFile(_socket, 200, filepath, contentTypes);
    return true;
}