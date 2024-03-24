#ifndef LOCATIONBLOCKS_HPP
# define LOCATIONBLOCKS_HPP

#include <map>
#include <vector>
#include "ServerBlocks.hpp"

class ServerBlocks;

class LocationBlocks {

    public:
        LocationBlocks();
        LocationBlocks(std::string const &locationBlock);
        LocationBlocks(LocationBlocks const &locationBlockInstance);
        LocationBlocks &operator= (LocationBlocks const &locationBlockInstance);
        ~LocationBlocks();
    
    private:
        std::string                     _path;
        std::string                     _root;
        bool                            _autoIndex;
        std::vector<std::string>        _index;
        std::string                     _alias;
        std::map<std::string, bool>     _allowMethods;
        std::string                     _return;
        std::vector<std::string>        _cgiExt; //for file extension
        std::vector<std::string>        _cgiPath; //for cgi compiler path
        size_t                          _clientMaxBodySize;
};

#endif