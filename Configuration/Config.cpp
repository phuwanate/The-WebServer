#include "Config.hpp"

Config::Config() { 

    return;
}

Config::Config(std::string const pathToConfigFile) { 
    
    //get every directives to all variable in Config class
    (void)pathToConfigFile;
    return;
}

Config::Config(Config const &ConfigInstance) { 
    
    *this = ConfigInstance;
}

Config  &Config::operator=(Config const &ConfigInstance) { 
    
    if (this != &ConfigInstance) {
        this->_fileContents = ConfigInstance._fileContents;
        this->_pathToCofigFile = ConfigInstance._pathToCofigFile;
        this->_serverBlocks = ConfigInstance._serverBlocks;
    }
    return *this;
}

Config::~Config(){
    
    return;
}

void    Config::__readFile() {

    return;
}
bool    Config::__generateConfig() {

    return true;
}