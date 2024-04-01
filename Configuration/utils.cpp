#include "utils.hpp"


std::string getBlock(std::string const &content, std::string const &needle, bool skipKeyword) {

    int         bracesCount = 0;
    size_t      index, start;
    std::string targetBlock;

    index = content.find(needle);
    if (index == std::string::npos)
        return targetBlock;
    
    index += needle.length();
    for (; index < content.length(); index++) {
        if (content[index] == '{') {
            skipKeyword == true ? start = index : start = 0;
            bracesCount++;
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
    
    size_t index = content.find(needle);

    if (index == std::string::npos)
        return 0;
    index += needle.length();
    for (; index < content.length() && content[index] != '{'; index++) { }
    return index;
}

void        isConflict(ServerBlocks newInstance, ServerBlocks oldInstance) {

    if ( (newInstance.getHostIP() == oldInstance.getHostIP()) && \
    (newInstance.getPortNumb() == oldInstance.getPortNumb()) && \
    (newInstance.getserverNames() == oldInstance.getserverNames()))
        throw std::string("Error: found duplicating servers block.");
}

std::string searchTarget(std::string const &content) {
    size_t      index = 0;
    std::string target;

    if (content[0] == '{' || content[0] == '}' || content[0] == ';')
        return content.substr(0, 1);
    for (; index < content.length(); index++) {

        if (isWhiteSpace(content[index]) == true)
            return content.substr(0, index); //for collect directive keywords (listen, index, etc.)
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

bool  is_digit(std::string values) {

    for (std::string::iterator it = values.begin(); it != values.end(); it++) {
        if (std::isdigit(*it) == false)
            return false;
    }
    return true;
}

template <typename T>
T   ft_convert(std::string const &needToConvert) {

    std::stringstream   stream(needToConvert);
    T                   result;

    stream >> result;
    return (result);
}

template int ft_convert<int>(std::string const &needToConvert);
