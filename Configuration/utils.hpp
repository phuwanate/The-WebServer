#ifndef UTILS_HPP
# define UTILS_HPP

#include <iostream>

std::string getBlock(std::string const &content, std::string const &needle, bool skipKeyword);
size_t      findFirstBrace(std::string const &content, std::string const &needle);

#endif