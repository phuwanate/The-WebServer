#ifndef CONF_HPP
# define CONF_HPP

#include <sstream>
#include <fstream>
#include <vector>
#include "ServerBlocks.hpp"
#include "utils.hpp"

#define DEFAULT "\033[0m"
#define RED     "\033[31m"

class ServerBlocks;

class Config {

    public:
            Config();
            Config(std::string const &pathToConfigFile);
            Config(Config const &ConfigInstance);
            Config  &operator=(Config const &ConfigInstance);
            ~Config();

            void        setPathToConfigFile(std::string const &pathToConfigFile);
            
            std::string                 getPathToConfigFile();
            std::vector<ServerBlocks>   getServerBlocks();
            void                        DebugEverythinginConfig(std::vector<ServerBlocks> serverBlocks);

    private:
            std::string                 _pathToCofigFile;
            std::string                 _fileContents;
            std::vector<ServerBlocks>   _serverBlocks;

            bool    __cleanContent();
            bool    __generateConfig();
};

#endif