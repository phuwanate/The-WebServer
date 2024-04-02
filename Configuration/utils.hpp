#ifndef UTILS_HPP
# define UTILS_HPP

#include <iostream>
#include <vector>
#include <cctype>
#include <sstream>
#include <fstream>
#include "ServerBlocks.hpp"

class ServerBlocks;

std::string   getBlock(std::string const &content, std::string const &needle, bool skipKeyword);
size_t        findFirstBrace(std::string const &content, std::string const &needle);
void          isConflict(ServerBlocks newInstance,ServerBlocks oldInstance);
std::string   searchTarget(std::string const &content);
bool          isWhiteSpace(char target);
bool          isDigit(std::string values);
void          validateHostIP(std::string value);
unsigned long hostIPToNetworkByteOrder(std::string const &hostIP);
std::string   splitString(std::string &needToSplit, std::string const &delimeter);
bool          checkFileExists(std::string File);

template <typename T>
T             convertString(std::string const &needToConvert);

#endif