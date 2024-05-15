#ifndef LOCATIONBLOCK_HPP
# define LOCATIONBLOCK_HPP

#include <map>
#include <vector>
#include <cctype>
#include "ServerBlock.hpp"

class ServerBlock;

class LocationBlock {

    public:
        LocationBlock();
        LocationBlock(std::string const &locationBlock, ServerBlock &serverBlock);
        LocationBlock(LocationBlock const &locationBlockInstance);
        LocationBlock &operator= (LocationBlock const &locationBlockInstance);
        ~LocationBlock();

        std::string                 getDirectoryPath();
        std::string                 getAlias();
        std::string                 getRoot();
        std::string                 getReturn();
        std::vector<std::string>    getIndex();
        std::vector<std::string>    getCgiFileExtention();
        std::vector<std::string>    getCgiCompilerPath();
        std::map<std::string, bool> getAllowMethods();
        bool                        getAutoIndex();
        size_t                      getClientMaxBodySize();

        void        setDirectoryPath(std::string const &val); 
        void        setRoot(std::string const &val);
        void        setAlias(std::string const &val);
        void        setReturn(std::string const &val);
        void        setIndex(std::vector<std::string> const &val);
        void        setCgiFileExtention(std::vector<std::string>  const &val);
        void        setCgiCompilerPath(std::vector<std::string>  const &val);
        void        setAllowMethod(std::string const &key, bool val);
        void        setAutoindex(bool val);
        void        setClientMaxBodySize(size_t val);
        
        void        DebugLocationBlock();

    private:

        void                            __initLocation(std::string const &locationBlock);
        void                            __initLocationParameters(std::string const &directive, std::vector<std::string> values);
        void                            __copyAttributes(ServerBlock &serverBlock);
        void                            __setAllMethods(bool val);

        std::string                     _directoryPath;
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