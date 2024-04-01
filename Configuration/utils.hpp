#ifndef UTILS_HPP
# define UTILS_HPP

#include <iostream>
#include <vector>
#include <cctype>
#include <sstream>
#include "ServerBlocks.hpp"

class ServerBlocks;

std::string getBlock(std::string const &content, std::string const &needle, bool skipKeyword);
size_t      findFirstBrace(std::string const &content, std::string const &needle);
void        isConflict(ServerBlocks newInstance,ServerBlocks oldInstance);
std::string searchTarget(std::string const &content);
bool        isWhiteSpace(char target);
bool        is_digit(std::string values);

template <typename T>
T           ft_convert(std::string const &needToConvert);

#endif