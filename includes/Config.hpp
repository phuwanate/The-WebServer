#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include "ServerBlock.hpp"
#include "utils.hpp"
#include "ServerHandler.hpp"

#define DEFAULT "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"

#define MAX_CON 1000
#define READ_BUFF 100000

class ServerBlock;

class Config {

	public:
			Config();
			Config(std::string const &pathToConfigFile);
			Config(Config const &ConfigInstance);
			Config  &operator=(Config const &ConfigInstance);
			~Config();

			void						setPathToConfigFile(std::string const &pathToConfigFile);
			
			std::string					getPathToConfigFile();
			std::vector<ServerBlock>	getServerBlocks();

			void						DebugEverythinginConfig(std::vector<ServerBlock> serverBlocks);   

	private:
			std::string					_pathToCofigFile;
			std::string					_fileContents;
			std::vector<ServerBlock>	_serverBlocks;
		// std::map<std::string, ServerBlock>_serverMap;

			bool						__cleanContent();
			bool						__generateConfig();
};

#endif