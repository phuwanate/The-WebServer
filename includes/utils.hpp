#ifndef UTILS_HPP
# define UTILS_HPP

#include <iostream>
#include <vector>
#include <cctype>
#include <sstream>
#include <fstream>
#include "Config.hpp"

class LocationBlock;
class ServerBlock;
class Config;

std::string		getBlock(std::string const &content, std::string const &needle, bool skipKeyword);
size_t			findFirstBrace(std::string const &content, std::string const &needle);
void			isServerConflict(ServerBlock newInstance,ServerBlock oldInstance);
void			isLocationDuplicate(LocationBlock newInstance, LocationBlock oldInstance);
std::string		searchTarget(std::string const &content);
bool			isWhiteSpace(char target);
bool			isDigit(std::string values);
void			validateHostIP(std::string value);
unsigned long	hostIPToNetworkByteOrder(std::string const &hostIP);
std::string		splitString(std::string &needToSplit, std::string const &delimeter);
bool			checkFileExists(std::string File);
std::string		errNumberOfParameters(std::string const &directive, std::string const &block);
std::string		ftToupper(std::string src);
LocationBlock	searchLocation(std::string serverName, std::string directoryPath, std::vector<ServerBlock> s_vec);
ServerBlock	    searchServer(std::string serverName, std::vector<ServerBlock> s_vec);
ServerBlock		searchServerByPort(size_t serverPort, std::vector<ServerBlock> s_vec);
void			*ft_memset(void *s, int src, size_t n);
void			*ft_memcpy(void *dst, const void *src, size_t n);
bool            isDirective(const std::string &key);

template <typename T>
T				convertString(std::string const &needToConvert);

#endif