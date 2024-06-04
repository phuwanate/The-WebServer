#include "ServerBlock.hpp"

ServerBlock::ServerBlock():_portNumb(8080), _hostIP(0), _clientMaxBodySize(3000000), _autoIndex(false) {
	return;
}

ServerBlock::ServerBlock(std::string const &serverBlock):_portNumb(8080), _hostIP(0), _clientMaxBodySize(3000000), _autoIndex(false) {
	__initServer(serverBlock);
	return;
}

ServerBlock::ServerBlock(ServerBlock const &serverBlockInstance) {
	*this = serverBlockInstance;
}

ServerBlock	&ServerBlock::operator= (ServerBlock const &serverBlockInstance) {
	if (this != &serverBlockInstance) {

		this->_socket_fd = serverBlockInstance._socket_fd;
		this->_rawPort   = serverBlockInstance._rawPort;
		this->_bindingPort = serverBlockInstance._bindingPort;
		this->_root = serverBlockInstance._root;
		this->_index = serverBlockInstance._index;
		this->_hostIP = serverBlockInstance._hostIP;
		this->_portNumb = serverBlockInstance._portNumb;
		this->_errorPage = serverBlockInstance._errorPage;
		this->_autoIndex = serverBlockInstance._autoIndex;
		this->_serverNames = serverBlockInstance._serverNames;
		this->_locationBlocks = serverBlockInstance._locationBlocks;
		this->_clientMaxBodySize = serverBlockInstance._clientMaxBodySize;
	}
	return *this;
}

ServerBlock::~ServerBlock() {
	return;
}

size_t	ServerBlock::getPortNumb() {
	return this->_portNumb;
}

unsigned long	ServerBlock::getHostIP() {
	return this->_hostIP;
}

std::string	ServerBlock::getServerName() {
	return this->_serverNames;
}

std::string	ServerBlock::getFullName() {
	return this->_bindingPort;
}

std::string	ServerBlock::getRoot() {
	return this->_root;
}

size_t	ServerBlock::getClientMaxBodySize() {
	return this->_clientMaxBodySize;
}

std::vector<std::string>	ServerBlock::getIndex() {
	return this->_index;
}

bool	ServerBlock::getAutoindex() {
	return this->_autoIndex;
}

std::map<int, std::string>	ServerBlock::getErrorPage() {
	return this->_errorPage;
}

std::vector<LocationBlock>&	ServerBlock::getLocationBlocks() {
	return this->_locationBlocks;
}

int	ServerBlock::getSocket() {
	return this->_socket_fd;
}

std::string	ServerBlock::getRawPort() {
	return this->_rawPort;
}

void	ServerBlock::setPortNumb(int val) {
	this->_portNumb = val;
}

void	ServerBlock::setRawPort(std::string val) {
	this->_rawPort = val;
}

void	ServerBlock::setHostIP(unsigned long val) {
	this->_hostIP = val;
}

void	ServerBlock::setServerName(std::string val) {
	this->_serverNames = val;
}

void	ServerBlock::setBindingPort(std::string val1, std::string val2){
	std::string colon = ":";
	this->_bindingPort = val1 + colon + val2;
}

void	ServerBlock::setRoot(std::string val) {
	this->_root = val;
}

void	ServerBlock::setClientMaxBodySize(size_t val) {
	this->_clientMaxBodySize = val;
}

void	ServerBlock::setIndex(std::vector<std::string> val) {
	this->_index = val;
}

void	ServerBlock::setAutoindex(bool val) {
	this->_autoIndex = val;
}

void	ServerBlock::setErrorPage(int key, std::string val) {
	this->_errorPage[key] = val;
}

void	ServerBlock::setLocationMap(std::string directoryPath, LocationBlock const &locationBlock) {
	this->_locationMap[directoryPath] = locationBlock;
}

void	ServerBlock::__initServer(std::string const &serverBlock) {
	std::string                 currentDirective = "";
	std::string                 content, locationBlock, target;
	std::vector<std::string>    values;

	for (size_t index = 0; index < serverBlock.length();) {
		
		for (; isWhiteSpace(serverBlock[index]) == true; index++) { }

		content = serverBlock.substr(index, serverBlock.length());
		target = searchTarget(content);
		if (target == "location") {
			locationBlock = getBlock(content, "location ", false);
			if (locationBlock.length() == 0)
				throw std::string("Error: invalid location block.");
			LocationBlock locationBlockInstance(locationBlock, *this);
			if (this->_locationBlocks.size() != 0) {
				for (size_t index = 0; index < this->_locationBlocks.size(); index++) {
					isLocationDuplicate(locationBlockInstance, this->_locationBlocks[index]);
				}
			}
			//Add
			setLocationMap(locationBlockInstance.getDirectoryPath(), locationBlockInstance);
			this->_locationBlocks.push_back(locationBlockInstance);
			index += locationBlock.length();
		}
		else if (target == "{") {
			if (currentDirective != "")
				throw std::string("Error: unexpected \"{\" in configuration file.");
			index += target.length();
		}
		else if (target == ";") {
			if (currentDirective == "") {
				throw std::string ("Error: unexpected \";\" in configuration file.");          
			}
			__initServerParameters(currentDirective, values);
			currentDirective = "";
			values.clear();
			index += target.length();
		}
		else {
			if (currentDirective == "")
				currentDirective = target;
			else
				values.push_back(target);
			index += target.length();
		}
	}
	checkAllparametersAfterParsing();
}

void	ServerBlock::checkAllparametersAfterParsing() {

	if (this->_locationBlocks.size() == 0)
		throw std::string ("Error: location blocks does not exists in configuration file.");
	if (this->_serverNames.length() == 0)
		throw std::string ("Error: Server must have server name in config file.");
	if (this->_hostIP == 0)
		throw std::string ("Error: Server must have ip-adress at \"host\" directive.");

}

void	ServerBlock::__initServerParameters(std::string const &directive, std::vector<std::string> values) {
	if (directive == "listen") {
		if (isDigit(values[0]) == false)
			throw std::string("Error: invalid parameter \"" + values[0] + "\" in listen directive.");    
		setRawPort(values[0]);
		setPortNumb(convertString<int>(values[0]));
	}
	else if (directive == "host") {
		if (values.size() != 1)
			throw std::string("Error: invalid number of parameters in host directive.");
		
		if (values[0] == "localhost")
			values[0] = "127.0.0.1";
		validateHostIP(values[0]);
		setHostIP(hostIPToNetworkByteOrder(values[0]));
	}
	else if (directive == "server_name") {
		setServerName(values[0]);
		setBindingPort(_serverNames, _rawPort);
	}
	else if (directive == "root") {
		if (values.size() != 1)
			throw std::string ("Error: invalid number of parameters in root directive.");
		validateRoot(values[0]);
		setRoot(values[0]);
	}
	else if (directive == "client_max_body_size") {
		if (values.size() != 1 || isDigit(values[0]) == false)
			throw std::string ("Error: invalid parameters in client_max_body_size directive.");
		setClientMaxBodySize(convertString<size_t>(values[0]));
	}
	else if (directive == "index") {
		// std::cout << getFullName() << std::endl;
		if (values.size() < 1)
			throw std::string ("Error: invalid number of parameters in index directive.");
		setIndex(values);
	}
	else if (directive == "autoindex") {
		if (values.size() != 1)
			throw std::string("Error: invalid number of parameters in autoindex directive.");
		if (values[0] == "on")
			setAutoindex(true);
		else if (values[0] == "off")
			setAutoindex(false);
		else
			throw std::string("Error: invalid  parameter" + values[0] + "in autoindex directive.");
	}
	else if (directive == "error_page") {
		if (values.size() != 2 || isDigit(values[0]) == false)
			throw std::string("Error: invalid  parameters in error_page directive.");
		if (this->_root.length() == 0)
			throw std::string ("Error: Server must have \"root\" directive.");

		std::string errorPage = pathToErrorPage(values[1]);
		if (checkFileExists(errorPage) == true) {
			setErrorPage(convertString<int>(values[0]), values[1]);
		}else{
			throw std::string("Error: Error page file does not exists: " + errorPage);
		}
	}
}

void ServerBlock::validateRoot(std::string root) {

	if (!(std::isalpha(root[0])))
		throw std::string ("Error: root must starting with alphabet.");
}

std::string	ServerBlock::pathToErrorPage(std::string errorFilePath) {
	std::string rootPath = getRoot();

	if (rootPath[rootPath.length() - 1] == '/' && errorFilePath[0] == '/')
		return (rootPath.substr(0, rootPath.length() - 1) + errorFilePath);
	if (rootPath[rootPath.length() - 1] != '/' && errorFilePath[0] != '/')
		return (rootPath + "/" + errorFilePath);
	return (rootPath + errorFilePath);
}

void	ServerBlock::DebugServerBlock(void) {
		std::cout << "          Port:           " << getPortNumb() << std::endl;
		std::cout << "          Server Name:    " << getServerName() << std::endl;
		std::cout << "	        FullName:       " << getFullName() << std::endl;
		std::cout << "          Host:           " << getHostIP() << std::endl; 
		std::cout << "          Root:           " << getRoot() << std::endl;
		std::cout << "          client_max_body_size: " << getClientMaxBodySize() << std::endl;
		
		std::cout << "          index:	 ";
		std::vector<std::string> tmp = getIndex();
		std::vector<std::string>::iterator it = tmp.begin();
		for (; it != tmp.end(); it++)
			std::cout << " " << *it << " ";
		std::cout << std::endl;

		std::cout << "          error_page:	 ";
		std::map<int, std::string> errpage =  getErrorPage();
		std::cout << " " << errpage[convertString<int>("404")] << std::endl; 
		
		std::cout << std::boolalpha << "          Autoindex:	  " << getAutoindex() << std::endl;      
}

bool	ServerBlock::manageSocket() {
	struct sockaddr_in s_addr;
	int				   flag = 1;

	if ((_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		throw std::string("Error: cannot init socket...");
	}

	if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&flag, sizeof(flag)) < 0) {
		std::cerr << RED << "Error: cannot set socket [" << _socket_fd << "]" << "to be reuseable..." << DEFAULT << std::endl;
		return false;
	}

	if (fcntl(_socket_fd ,F_SETFL, O_NONBLOCK) < 0) {
		std::cerr << RED << "Error: cannot set socket [" << _socket_fd << "]" << "to be non-blocking..." << DEFAULT << std::endl;
		return false;
	}

	ft_memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_family = AF_INET; //IPV4
	s_addr.sin_addr.s_addr = getHostIP(); //IP address as Network Byte order. 127.0.0.1
	s_addr.sin_port = htons(getPortNumb());//8080

	if (bind(_socket_fd, (struct sockaddr*) &s_addr, sizeof(s_addr)) < 0) {
		std::cerr << RED << "Error: cannot bind socket [" << _socket_fd << "]" << DEFAULT << std::endl;
		return false;
	}
	return true;
}