#ifndef SERVERBLOCKS_HPP
# define SERVERBLOCKS_HPP

#include <map>
#include <vector>
#include <iostream>
#include "LocationBlocks.hpp"

class LocationBlocks;

class ServerBlocks {
    
    public:
        ServerBlocks();
        ServerBlocks(std::string const &serverBlock);
        ServerBlocks(ServerBlocks const &serverBlockInstance);
        ServerBlocks &operator= (ServerBlocks const &serverBlockInstance);
        ~ServerBlocks();
    
    private:
        int                         _portNumb; //setup port
        unsigned long               _hostIP; //setup host IP address
        std::string                 _serverName;
        std::map<int, std::string>  _errorPage;
        std::vector<std::string>    _index;
        bool                        _autoIndex;
        size_t                      _clientMaxBodySize;
        std::vector<LocationBlocks> _locationBlocks;
        std::string                 _root;

};

#endif