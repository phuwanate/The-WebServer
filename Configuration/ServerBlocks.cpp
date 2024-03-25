#include "ServerBlocks.hpp"

ServerBlocks::ServerBlocks() {

    return;
}

ServerBlocks::ServerBlocks(std::string const &serverBlock) {

    //get every directives to all variable in serverBlocks class
    (void)serverBlock;
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