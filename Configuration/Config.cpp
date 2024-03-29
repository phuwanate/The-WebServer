#include "Config.hpp"

Config::Config() { 

    return;
}

Config::Config(std::string const &pathToConfigFile) { 
    
    //get every directives to all variable in Config class
    setPathToConfigFile(pathToConfigFile);
    if (__cleanContent() == false) {

        throw std::invalid_argument("Error: Can not open config file.");
    }
    __generateConfig();

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

//getter setters
void        Config::setPathToConfigFile(std::string const &pathToConfigFile) {

    this->_pathToCofigFile = pathToConfigFile;
}

std::string Config::getPathToConfigFile() {

    return this->_pathToCofigFile;
}

bool    Config::__cleanContent() {

    std::string     content;
    bool            isComment = false;
    std::ifstream   readFile(this->_pathToCofigFile.c_str());
    
    if (!readFile.is_open()) {

        return false;
    }

    std::stringstream outputToStream;
    outputToStream << readFile.rdbuf();
    content = outputToStream.str();
    readFile.close();

    for (size_t index = 0; index < content.length(); index++) {

        if (isComment == false) {
            if (content[index] == '#') {
                isComment = true;
            } else {
                this->_fileContents += content[index];
            }
        } else if (content[index] == '\n') {
            isComment = false;
        }
    }
    // std::cout << this->_fileContents << std::endl;
    return true;
}
bool    Config::__generateConfig() {

    std::string content = this->_fileContents;
    std::string serverBlock;

    for (size_t index = 0; index < content.length(); index += serverBlock.length()) {

        content = this->_fileContents.substr(index, this->_fileContents.length());
        serverBlock = getBlock(content, "server", true);
        if (serverBlock.length() == 0)
            return false;
        index += findFirstBrace(content, "server");

        ServerBlocks serverBlockInstance(serverBlock);   
    }
    return true;
}
