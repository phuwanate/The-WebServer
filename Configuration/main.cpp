#include "Config.hpp"
#include "LocationBlocks.hpp"
#include "ServerBlocks.hpp"

#include <iostream>


int main (int ac, char **av) {

    std::string pathToConfigFile;

    if (ac > 2) {
        std::cout << "webserv should have only 1 argument." << std::endl;
        return EXIT_FAILURE;
    }
    ac == 2 ? pathToConfigFile = av[1] : pathToConfigFile = "./config_files/default.conf";

    try{
     Config config(pathToConfigFile);
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


