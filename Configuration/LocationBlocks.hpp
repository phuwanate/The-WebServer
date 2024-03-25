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
        std::string                     _alias;
        std::string                     _return;
        std::vector<std::string>        _index;
        std::vector<std::string>        _cgiFileExtention; //for file extension
        std::vector<std::string>        _cgiCompilerPath; //for cgi compiler path
        std::map<std::string, bool>     _allowMethods;
        bool                            _autoIndex;
        size_t                          _clientMaxBodySize;
};

#endif