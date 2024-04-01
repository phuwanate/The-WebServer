#ifndef SERVERBLOCKS_HPP
# define SERVERBLOCKS_HPP

#include <map>
#include <vector>
#include <iostream>
#include "LocationBlocks.hpp"
#include "utils.hpp"

class LocationBlocks;

class ServerBlocks {
    
    public:
        ServerBlocks();
        ServerBlocks(std::string const &serverBlock);
        ServerBlocks(ServerBlocks const &serverBlockInstance);
        ServerBlocks &operator= (ServerBlocks const &serverBlockInstance);
        ~ServerBlocks();

        int                         getPortNumb();
        unsigned long               getHostIP();
        std::string                 getserverNames();

        void                        setPortNumb(int val);
    
    private:
        void                        __initAllAttributes(std::string const &serverBlock);
        void                        __initServerAttribute(std::string const &directive, std::vector<std::string> values);
        

        size_t                      _portNumb; //setup port
        unsigned long               _hostIP; //setup host IP address
        size_t                      _clientMaxBodySize;
        std::string                 _serverNames;
        std::string                 _root;
        std::vector<std::string>    _index;
        std::vector<LocationBlocks> _locationBlocks;
        std::map<int, std::string>  _errorPage;
        bool                        _autoIndex;

};

#endif