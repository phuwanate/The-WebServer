#ifndef SERVERBLOCK_HPP
# define SERVERBLOCK_HPP

#include <map>
#include <vector>
#include <iostream>
#include "LocationBlock.hpp"
#include "utils.hpp"
#include <arpa/inet.h>

class LocationBlock;

class ServerBlock {
	
	public:
		ServerBlock();
		ServerBlock(std::string const &serverBlock);
		ServerBlock(ServerBlock const &serverBlockInstance);
		ServerBlock &operator= (ServerBlock const &serverBlockInstance);
		~ServerBlock();

		size_t						getPortNumb();
		unsigned long				getHostIP();
		std::string					getServerName();
		std::string					getRoot();
		size_t						getClientMaxBodySize(); //P'karn
		std::vector<std::string>	getIndex();
		bool						getAutoindex();
		std::map<int, std::string>	getErrorPage();
		std::vector<LocationBlock>&	getLocationBlocks();
		int							getSocket();
		std::string					getFullName();
		std::string					getRawPort();
		// std::map<std::string , LocationBlocks> getLocationMap();
		

		void						setPortNumb(int val);
		void						setHostIP(unsigned long val);
		void						setBindingPort(std::string val1, std::string val2);
		void						setServerName(std::string val);
		void						setRawPort(std::string val);
		void						setRoot(std::string val);
		void						setClientMaxBodySize(size_t val);
		void						setIndex(std::vector<std::string> val);
		void						setAutoindex(bool val);
		void						setErrorPage(int key, std::string val);
		void						setLocationMap(std::string directoryPath, LocationBlock const &locationBlock);

		bool						manageSocket();
		std::string					pathToErrorPage(std::string values);
		void						DebugServerBlock();        


	private:
		void						__initServer(std::string const &serverBlock);
		void						__initServerParameters(std::string const &directive, std::vector<std::string> values);

		int							_socket_fd;
		std::string					_rawPort;
		size_t						_portNumb; //setup port
		unsigned long				_hostIP; //setup host IP address
		size_t						_clientMaxBodySize;
		std::string					_serverNames;
		std::string					_bindingPort;
		std::string					_root;
		std::vector<std::string>	_index;
		std::vector<LocationBlock>	_locationBlocks;
		std::map<int, std::string>	_errorPage;
		bool						_autoIndex;

		std::map<std::string, LocationBlock>	_locationMap;
};

#endif