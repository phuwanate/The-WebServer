#include "ServerBlocks.hpp"

ServerBlocks::ServerBlocks() {

    return;
}

ServerBlocks::ServerBlocks(std::string const &serverBlock) {

    __initServer(serverBlock);
    return;
}

ServerBlocks::ServerBlocks(ServerBlocks const &serverBlockInstance) {

    *this = serverBlockInstance;
}

ServerBlocks &ServerBlocks::operator= (ServerBlocks const &serverBlockInstance) {

    if (this != &serverBlockInstance) {

        this->_root = serverBlockInstance._root;
        this->_index = serverBlockInstance._index;
        this->_hostIP = serverBlockInstance._hostIP;
        this->_portNumb = serverBlockInstance._portNumb;
        this->_errorPage = serverBlockInstance._errorPage;
        this->_autoIndex = serverBlockInstance._autoIndex;
        this->_serverNames = serverBlockInstance._serverNames;
        this->_locationBlocks = serverBlockInstance._locationBlocks;
        this->_clientMaxBodySize = serverBlockInstance._clientMaxBodySize;
    }
    return *this;
}

ServerBlocks::~ServerBlocks() {

    return;
}

int ServerBlocks::getPortNumb() {

    return this->_portNumb;
}

unsigned long ServerBlocks::getHostIP() {

    return this->_hostIP;
}

std::string ServerBlocks::getServerName() {

    return this->_serverNames;
}

std::string ServerBlocks::getRoot() {

    return this->_root;
}

size_t  ServerBlocks::getClientMaxBodySize() {

    return this->_clientMaxBodySize;
}

std::vector<std::string> ServerBlocks::getIndex() {

    return this->_index;
}

bool    ServerBlocks::getAutoindex() {

    return this->_autoIndex;
}

std::map<int, std::string>  ServerBlocks::getErrorPage() {

    return this->_errorPage;
}

std::vector<LocationBlocks> ServerBlocks::getLocationBlocks() {

    return this->_locationBlocks;
}

void    ServerBlocks::setPortNumb(int val) {
    
    this->_portNumb = val;
}

void    ServerBlocks::setHostIP(unsigned long val) {

    this->_hostIP = val;
}

void    ServerBlocks::setServerName(std::string val) {

    this->_serverNames = val;
}

void    ServerBlocks::setRoot(std::string val) {

    this->_root = val;
}

void    ServerBlocks::setClientMaxBodySize(size_t val) {

    this->_clientMaxBodySize = val;
}

void    ServerBlocks::setIndex(std::vector<std::string> val) {

    this->_index = val;
}

void    ServerBlocks::setAutoindex(bool val) {

    this->_autoIndex = val;
}

void    ServerBlocks::setErrorPage(int key, std::string val) {

    this->_errorPage[key] = val;
}


void ServerBlocks::__initServer(std::string const &serverBlock) {

    std::string                 currentDirective = "";
    std::string                 content, locationBlock, target;
    std::vector<std::string>    values;

    for (size_t index = 0; index < serverBlock.length();) {
        
        for (; isWhiteSpace(serverBlock[index]) == true; index++) { }

        content = serverBlock.substr(index, serverBlock.length());
        target = searchTarget(content);
        if (target == "location") {
            locationBlock = getBlock(content, "location", false);
            if (locationBlock.length() == 0)
                throw std::string("Error: invalid location block.");

            LocationBlocks locationBlockInstance(locationBlock, *this);
            if (this->_locationBlocks.size() > 1) {
                for (size_t index = 0; index < this->_locationBlocks.size(); index++) {
                    isLocationDuplicate(locationBlockInstance, this->_locationBlocks[index]);
                }
            }
            this->_locationBlocks.push_back(locationBlockInstance);
            index += locationBlock.length();
        }
        else if (target == "{") {
            if (currentDirective != "")
                throw std::string("Error: unexpected \"{\" in configuration file.");
            index += target.length();
        }
        else if (target == ";") {
            if (currentDirective == "") {
                throw std::string ("Error: unexpected \";\" in configuration file.");          
            }
            __initServerParameters(currentDirective, values);
            currentDirective = "";
            values.clear();
            index += target.length();
        }
        else {
            if (currentDirective == "")
                currentDirective = target;
            else
                values.push_back(target);
            index += target.length();
        }
    }
    if (this->_locationBlocks.size() == 0)
        throw std::string ("Error: location blocks does not exists in configuration file.");
}

void ServerBlocks::__initServerParameters(std::string const &directive, std::vector<std::string> values) {

    if (directive == "listen") {

        if (isDigit(values[0]) == false)
            throw std::string("Error: invalid parameter \"" + values[0] + "\" in listen directive.");    
        setPortNumb(convertString<int>(values[0]));
        //Debug
        std::cout << "Port: " << getPortNumb() << std::endl;
    }
    else if (directive == "host") {

        if (values.size() != 1)
            throw std::string("Error: invalid number of parameters in host directive.");
        
        if (values[0] == "localhost")
            values[0] = "127.0.0.1";
        validateHostIP(values[0]);
        setHostIP(hostIPToNetworkByteOrder(values[0]));
        //Debug
        std::cout << "Host: " << getHostIP() << std::endl;
    }
    else if (directive == "server_name") {
        setServerName(values[0]);
        //Debug
        std::cout << "Server Name: " << getServerName() << std::endl;
    }
    else if (directive == "root") {
        if (values.size() != 1)
            throw std::string ("Error: invalid number of parameters in root directive.");
        setRoot(values[0]);
        //Debug
        std::cout << "Root: " << getRoot() << std::endl;
    }
    else if (directive == "client_max_body_size") {
        if (values.size() != 1 || isDigit(values[0]) == false)
            throw std::string ("Error: invalid parameters in client_max_body_size directive.");
        setClientMaxBodySize(convertString<size_t>(values[0]));
        //Debug
        std::cout << "client_max_body_size: " << getClientMaxBodySize() << std::endl;
    }
    else if (directive == "index") {
        if (values.size() < 1)
            throw std::string ("Error: invalid number of parameters in index directive.");
        setIndex(values);
        //Debug
        std::cout << "index: ";
        std::vector<std::string> tmp = getIndex();
        std::vector<std::string>::iterator it = tmp.begin();
        for (; it != tmp.end(); it++)
            std::cout << *it << " ";
        std::cout << std::endl;
    }
    else if (directive == "autoindex") {

        if (values.size() != 1)
            throw std::string("Error: invalid number of parameters in autoindex directive.");
        if (values[0] == "on")
            setAutoindex(true);
        else if (values[0] == "off")
            setAutoindex(false);
        else
            throw std::string("Error: invalid  parameter" + values[0] + "in autoindex directive.");
        //Debug
        std::cout << std::boolalpha <<"Autoindex: " << getAutoindex() << std::endl;
    }
    else if (directive == "error_page") {
        if (values.size() != 2 || isDigit(values[0]) == false)
            throw std::string("Error: invalid  parameters in error_page directive.");
        
        std::string errorPage = pathToErrorPage(values[1]);
        if (checkFileExists(errorPage) == true) {
            setErrorPage(convertString<int>(values[0]), values[1]);
        }else{
            throw std::string("Error: Error page file does not exists: " + errorPage);
        }
        std::map<int, std::string> errpage =  getErrorPage();
        std::cout << errpage[convertString<int>(values[0])] << std::endl; 
    }
}

std::string ServerBlocks::pathToErrorPage(std::string errorFilePath) {

    std::string rootPath = getRoot();

    if (rootPath[rootPath.length() - 1] == '/' && errorFilePath[0] == '/')
        return (rootPath.substr(0, rootPath.length() - 1) + errorFilePath);
    if (rootPath[rootPath.length() - 1] != '/' && errorFilePath[0] != '/')
        return (rootPath + "/" + errorFilePath);
    return (rootPath + errorFilePath);
}
