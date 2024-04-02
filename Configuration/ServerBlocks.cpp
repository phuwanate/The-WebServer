#include "ServerBlocks.hpp"

ServerBlocks::ServerBlocks() {

    return;
}

ServerBlocks::ServerBlocks(std::string const &serverBlock) {

    __initAllAttributes(serverBlock);
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

std::string ServerBlocks::getserverNames() {

    return this->_serverNames;
}

void    ServerBlocks::setPortNumb(int val) {
    
    this->_portNumb = val;
}

void    ServerBlocks::setHostIP(unsigned long val) {

    this->_hostIP = val;
}

void ServerBlocks::__initAllAttributes(std::string const &serverBlock) {

    std::string                 currentDirective = "";
    std::string                 content;
    std::string                 locationBlock;
    std::string                 target;
    std::vector<std::string>    values;

    for(size_t index = 0; index < serverBlock.length();) {
        
        for (; isWhiteSpace(serverBlock[index]) == true; index++) { }

        content = serverBlock.substr(index, serverBlock.length());
        target = searchTarget(content);
        if (target == "location") {
            //create location instance
            locationBlock = getBlock(content, "location", false);
            if (locationBlock.length() == 0)
                throw std::string("Error: invalid location block.");
        }
        else if (target == "{") {
            if (currentDirective != "")
                throw std::string("Error: unexpected \"{\" in configuration file.");
            index += target.length();//index++
        }
        else if (target == ";") {
            if (currentDirective == "") {
                throw std::string ("Error: unexpected \";\" in configuration file.");          
            }
            //get directive's value to server instance;
            __initServerParameters(currentDirective, values);
            currentDirective = "";
            values.clear();
            index += target.length();//index++
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

        if (is_digit(values[0]) == false)
            throw std::string("Error: invalid parameter \"" + values[0] + "\" in listen directive.");    
        setPortNumb(ft_convert<int>(values[0]));
        std::cout << "Port: " << getPortNumb() << std::endl;
    }
    else if (directive == "host") {

        if (values.size() != 1)
            throw std::string("Error: invalid numbers of parameter in host directive.");
        
        if (values[0] == "localhost")
            values[0] = "127.0.0.1";
        validateHostIP(values[0]);
        setHostIP(hostIPToNetworkByteOrder(values[0]));
        std::cout << "Host: " << getHostIP() << std::endl;
    }
}