#ifndef CONF_HPP
# define CONF_HPP

#include <fstream>
#include <vector>
#include "ServerBlocks.hpp"

class ServerBlocks;

class Config {

    public:
            Config();
            Config(std::string const pathToConfigFile);
            Config(Config const &ConfigInstance);
            Config  &operator=(Config const &ConfigInstance);
            ~Config();

    private:
            std::string                 _pathToCofigFile;
            std::string                 _fileContents;
            std::vector<ServerBlocks>   _serverBlocks;

            void    __readFile();
            bool    __generateConfig();
};

#endif