#include "Config.hpp"

Config::Config() { 

    return;
}

Config::Config(std::string const &pathToConfigFile) { 
    
    //get every directives to all variable in Config class
    setPathToConfigFile(pathToConfigFile);
    if (__cleanContent() == false)
        throw std::string("Error: Can not open config file.");
    __generateConfig();
    if (this->_serverBlocks.size() == 0)
        throw(std::string("can't build server"));

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

std::vector<ServerBlocks>   Config::getServerBlocks() {

    return this->_serverBlocks;
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
        } else if (content[index] == '\n')
            isComment = false;
    }
    return true;
}
bool    Config::__generateConfig() {

    std::string content = this->_fileContents;
    std::string serverBlock;
    bool        serverFound;

    for (size_t index = 0; index < content.length(); index += serverBlock.length()) {
        content = this->_fileContents.substr(index, this->_fileContents.length());
        serverBlock = getBlock(content, "server ", true);
        if (serverBlock.length() == 0 && serverFound != true)
            serverFound = false;
        if (serverBlock.length() == 0 && serverFound == false)
            throw std::string ("Error: invalid server block.");
        else if (serverBlock.length() == 0 && serverFound == true)
            break;//handle case: newline at the last line.
        serverFound = true;
        index += findFirstBrace(content, "server");
        ServerBlocks serverBlockInstance(serverBlock);
        if (this->_serverBlocks.size() != 0) {
            for (size_t index = 0; index < this->_serverBlocks.size(); index++){    
                isServerConflict(serverBlockInstance, this->_serverBlocks[index]);
            }
        }
        this->_serverBlocks.push_back(serverBlockInstance);
    }
    return true;
}

void  Config::DebugEverythinginConfig(std::vector<ServerBlocks> serverBlocks) {

    std::vector<ServerBlocks>::iterator it_s = serverBlocks.begin();
    for (; it_s != serverBlocks.end(); it_s++) {
        ServerBlocks curr_s = *it_s;
        std::cout << "Server {" << std::endl;
        curr_s.DebugServerBlock();
        std::vector<LocationBlocks> vec_l = curr_s.getLocationBlocks();
        std::vector<LocationBlocks>::iterator it_l = vec_l.begin();
        for (; it_l != vec_l.end(); it_l++) {
            LocationBlocks curr_l = *it_l;
            curr_l.DebugLocationBlock();
        }
        std::cout << "}" << std::endl;
    }
}
