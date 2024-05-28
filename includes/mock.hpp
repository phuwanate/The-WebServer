#ifndef MOCK_HPP
# define MOCK_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <string.h>
#include <sys/socket.h>

#define DEFAULT "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"

typedef struct s_res {
	std::string htmlContent;
	const char* httpResponse;
} t_res;

std::stringstream	createMockData();
void				createMockResponse(int write_sd);

#endif