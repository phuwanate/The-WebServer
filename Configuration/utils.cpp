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

    for (; index < content.length(); index++) {
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
