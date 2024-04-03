#include "utils.hpp"


std::string getBlock(std::string const &content, std::string const &needle, bool skipKeyword) {

    int         bracesCount = -1;
    size_t      index, start;
    std::string targetBlock;

    index = content.find(needle);
    if (index == std::string::npos)
        return targetBlock;
    
    index += needle.length();
    for (; index < content.length(); index++) {
        if (content[index] == '{') {
            skipKeyword == true ? start = index : start = 0;
            bracesCount = 1;
            index++;
            break;
        }
    }

    for (; index < content.length() && bracesCount > 0; index++) {
        if (content[index] == '{')
            bracesCount++;
        else if (content[index] == '}')
            bracesCount--;
    }

    if (bracesCount == 0)
        targetBlock = content.substr(start, index - start);
    return targetBlock;
}

size_t      findFirstBrace(std::string const &content, std::string const &needle) {
    
    // size_t index = content.find(needle);
    if (content.find(needle) == std::string::npos)
        return 0;
    // index += needle.length();
    // for (; index < content.length() && content[index] != '{'; index++) { }
    
    return content.find("{");
}

void        isServerConflict(ServerBlocks newInstance, ServerBlocks oldInstance) {

    if ( (newInstance.getHostIP() == oldInstance.getHostIP()) && \
    (newInstance.getPortNumb() == oldInstance.getPortNumb()) && \
    (newInstance.getServerName() == oldInstance.getServerName()))
        throw std::string("Error: found duplicating servers block.");
}

void          isLocationDuplicate(LocationBlocks newInstance, LocationBlocks oldInstance) {

    if (newInstance.getDirectoryPath() == oldInstance.getDirectoryPath())
        throw std::string ("Error: duplicate location \"" + newInstance.getDirectoryPath() + "\" in location block.");
}

std::string searchTarget(std::string const &content) {
    size_t      index = 0;
    std::string target;

    if (content[0] == '{' || content[0] == '}' || content[0] == ';')
        return content.substr(0, 1);
    for (; index < content.length(); index++) {

        if (isWhiteSpace(content[index]) == true)
            return content.substr(0, index); //for retrive directive keywords (listen, index, etc.)
        switch (content[index]){

            case '{':
                return content.substr(0, index);
            case '}':
                return content.substr(0, index);
            case ';':
                return content.substr(0, index);
        }
    }
    return content.substr(0, index);
}

bool isWhiteSpace(char target) {

    if (target == ' ' || target == '\t' || target == '\n' || target == '\r' || target == '\v')
        return true;
    return false;
}

bool  isDigit(std::string values) {

    for (std::string::iterator it = values.begin(); it != values.end(); it++) {
        if (std::isdigit(*it) == false)
            return false;
    }
    return true;
}

void    validateHostIP(std::string value) {
    int validateRange;
    while (true) {
        // std::cout << "here" << std::endl;
        size_t pos = value.find(".");
        if (pos == std::string::npos){
            if (value == "" || isDigit(value) == false)
                throw std::string ("Error: invalid host number \"" + value + "\" in host directive.");
            validateRange = convertString<int>(value);
            if (validateRange < 0 || validateRange > 255)
                throw std::string ("Error: host number \"" + value + "\" is out of bound.");
            break;
        }
        std::string target = value.substr(0, pos);
        if (target == "" || isDigit(target) == false)
            throw std::string ("Error: invalid host number \"" + value + "\" in host directive.");
        value = value.substr(pos + 1, (value.length()) - (pos + 1));
        validateRange = convertString<int>(target);
        if (validateRange < 0 || validateRange > 255)
            throw std::string ("Error: host number \"" + value + "\" is out of bound.");
    }
}

std::string   splitString(std::string &needToSplit, std::string const &delimeter) {

    std::size_t pos = needToSplit.find(delimeter);
    std::string result;

    if (pos == std::string::npos)
        return "";
    result = needToSplit.substr(0, pos);
    needToSplit = needToSplit.substr(pos + delimeter.length(), needToSplit.length());
    return result;
}

unsigned long hostIPToNetworkByteOrder(std::string const &hostIP) {

    std::string     needToConvert = hostIP;
    int             ipStorage[3];//IPV4
    unsigned long   result;

    for (int index = 0; index < 3; index++)
        ipStorage[index] = convertString<unsigned long>(splitString(needToConvert, "."));
    
    result = convertString<unsigned long>(needToConvert); //for the last bit in IP address.
    for (int index = 2; index >= 0; index --)
        result = (result * 256) + ipStorage[index];
    return result;
}

bool checkFileExists(std::string File) {

    std::ifstream inputStream(File.c_str());

    return  inputStream.good();
}

std::string   errNumberOfParameters(std::string const &directive, std::string const &block) {

    return "Error: [" + block + "] invalid numbers of parameters \"" + directive + "\" directive";
}

std::string   ftToupper(std::string src) {
    
    std::string result;
    
    for (size_t index = 0; index < src.length(); index++) {
        result += toupper(src[index]);
    }
    return result;
}

template <typename T>
T   convertString(std::string const &needToConvert) {

    std::stringstream   stream(needToConvert);
    T                   result;

    if (isDigit(needToConvert) == false)
        return false;

    stream >> result;
    return (result);
}

// template int ftConvert<int>(std::string const &needToConvert);
// template unsigned long ftConvert<unsigned long>(std::string const &needToConvert);