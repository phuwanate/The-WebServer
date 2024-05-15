#include "Config.hpp"
#include "LocationBlock.hpp"
#include "ServerBlock.hpp"

#include <iostream>
#include <cstdlib>


int main (int ac, char **av) {

    std::string pathToConfigFile;

    if (ac > 2) {
        std::cout << "webserv should have only 1 argument." << std::endl;
        return EXIT_FAILURE;
    }
    ac == 2 ? pathToConfigFile = av[1] : pathToConfigFile = "./config_files/default.conf";

    try{
        Config config(pathToConfigFile);
        std::vector<ServerBlock> s_vec = config.getServerBlocks();
        ServerHandler Handler(s_vec);
        Handler.initHandler();
        Handler.startServerHandler();
        // config.DebugEverythinginConfig(serv);
        // start_server(s_vec);

    //  server part;
    } catch (std::string errorMassage) {
        std::cout << RED << errorMassage << DEFAULT << std::endl;
    } catch (std::out_of_range &e) {
        std::cout << RED << "Error: out of bound." << DEFAULT << std::endl;
    } catch (std::exception &e) {
        std::cout << RED << "Error: caught an exception." << DEFAULT << std::endl;
    }
    return EXIT_SUCCESS;
}


