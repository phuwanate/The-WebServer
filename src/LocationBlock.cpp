#include "LocationBlock.hpp"

LocationBlock::LocationBlock():_autoIndex(0) {
	return;
}

LocationBlock::LocationBlock(std::string const &locationBlock, ServerBlock &serverBlock):_autoIndex(0) {
	__copyAttributes(serverBlock);
	__initLocation(locationBlock);
	return;
}

LocationBlock::LocationBlock(LocationBlock const &locationBlockInstance) {

	*this = locationBlockInstance;
}

LocationBlock	&LocationBlock::operator= (LocationBlock const &locationBlockInstance) {
	if (this != &locationBlockInstance) {

		this->_cgiMap = locationBlockInstance._cgiMap;
		this->_root = locationBlockInstance._root;
		this->_index = locationBlockInstance._index;
		this->_alias = locationBlockInstance._alias;
		this->_return = locationBlockInstance._return;
		this->_autoIndex = locationBlockInstance._autoIndex;
		this->_allowMethods = locationBlockInstance._allowMethods;
		this->_directoryPath = locationBlockInstance._directoryPath;
		this->_cgiCompilerPath = locationBlockInstance._cgiCompilerPath;
		this->_cgiFileExtention = locationBlockInstance._cgiFileExtention;
		this->_clientMaxBodySize = locationBlockInstance._clientMaxBodySize;
	}
	return *this;
}

LocationBlock::~LocationBlock() {
	return;
}

std::string	LocationBlock::getDirectoryPath() {
	return this->_directoryPath;
}

std::string	LocationBlock::getAlias() {
	return this->_alias;
}

std::string	LocationBlock::getRoot() {
	return this->_root;
}

std::string	LocationBlock::getReturn() {
	return this->_return;
}

std::vector<std::string>	LocationBlock::getIndex() {
	return this->_index;
}

std::vector<std::string>	LocationBlock::getCgiFileExtention() {
	return this->_cgiFileExtention;
}

std::vector<std::string>	LocationBlock::getCgiCompilerPath() {
	return this->_cgiCompilerPath;
}

std::map<std::string, bool>	LocationBlock::getAllowMethods() {
	return this->_allowMethods;
}

std::map<std::string, std::string> LocationBlock::getCgiMap() {
	return this->_cgiMap;
}

bool	LocationBlock::getAutoIndex() {
	return this->_autoIndex;
}

size_t	LocationBlock::getClientMaxBodySize() {
	return this->_clientMaxBodySize;
}

void	LocationBlock::setDirectoryPath(std::string const &val) {
	this->_directoryPath = val;
}

void	LocationBlock::setAlias(std::string const &val) {
	this->_alias = val;
}

void	LocationBlock::setRoot(std::string const &val) {
	this->_root = val;
}

void	LocationBlock::setReturn(std::string const &val) {
	this->_return = val;
}

void	LocationBlock::setIndex(std::vector<std::string> const &val) {
	this->_index = val;
}

void	LocationBlock::setCgiFileExtention(std::vector<std::string>  const &val) {
	this->_cgiFileExtention = val;
}

void	LocationBlock::setCgiCompilerPath(std::vector<std::string>  const &val) {
	this->_cgiCompilerPath = val;
}

void	LocationBlock::setAllowMethod(std::string const &key, bool val) {
	this->_allowMethods[key] = val;
}

void	LocationBlock::setAutoindex(bool val) {
	this->_autoIndex = val;
}

void	LocationBlock::setClientMaxBodySize(size_t val) {
	this->_clientMaxBodySize = val;
}

void	LocationBlock::__copyAttributes(ServerBlock &serverBlock) {
	// setRoot(serverBlock.getRoot());
	// setIndex(serverBlock.getIndex());
	setAutoindex(serverBlock.getAutoindex());
	setClientMaxBodySize(serverBlock.getClientMaxBodySize());
}

void	LocationBlock::__setAllMethods(bool val) {
	setAllowMethod("GET", val);
	setAllowMethod("POST", val);
	setAllowMethod("PUT", val);
	setAllowMethod("DELETE", val);

}

void	LocationBlock::__setCgiMap() {
	std::vector<std::string>::iterator p_it = _cgiCompilerPath.begin();
	std::vector<std::string>::iterator x_it = _cgiFileExtention.begin();

	if (_cgiCompilerPath.size() == _cgiFileExtention.size())
		for (;p_it != _cgiCompilerPath.end(); p_it++) {
			_cgiMap[*x_it] = *p_it;
			x_it++;
		}
	else
		throw std::string ("Numbers of cgi compiler path and file extention doesn't match.");
}

void	LocationBlock::__initLocation(std::string const &locationBlock){
	std::string currentDirective = "";
    std::string content, target;
    std::vector<std::string> values;

	for (size_t index = 0; index < locationBlock.length();) {

		for (; isWhiteSpace(locationBlock[index]) == true; index++) { }

		content = locationBlock.substr(index, locationBlock.length());
		target = searchTarget(content);
		if (currentDirective == "location") {
			if (target.find("/") != std::string::npos) {
				setDirectoryPath(target);
			}
			else if (target == "{" || target == "}")
				currentDirective = "";
			if (getDirectoryPath() == "")
				throw std::string ("Error: invalid path in location directive.");
			index += target.length();
		}
		else if (target == "{" || target == "}") {

			if (currentDirective != "")
				throw std::string ("Error: unexpected \"" + target + "\" in configuration file.");
			index += target.length();
		}
		else if (target == ";") {

			if (currentDirective == "")
				throw std::string ("Error: unexpected \"" + target + "\" in configuration file.");
			__initLocationParameters(currentDirective, values);
			values.clear();
			currentDirective = "";
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
	__setCgiMap();
}

void	LocationBlock::__initLocationParameters(std::string const &directive, std::vector<std::string> values) {
	if (directive == "alias") {
		if (values.size() != 1)
			throw errNumberOfParameters(directive, "location");
		setAlias(values[0]);
	}
	else if (directive == "allow_http_methods") {
		if (values.size() < 1)
			throw errNumberOfParameters(directive, "location");
		__setAllMethods(false);

		std::vector<std::string>::iterator it;
		for (it = values.begin(); it != values.end(); it++) {

			std::string method = ftToupper(*it);
			std::map<std::string, bool> allowMethods = getAllowMethods();
			if (allowMethods.find(method) == allowMethods.end())
				throw std::string ("Error: [location] invalid parameter \"" + *it + "\" in allow_http_methods directive.");
			setAllowMethod(method, true);
		}
	}
	else if (directive == "index") {
		if (values.size() < 1)
			throw errNumberOfParameters(directive, "location");
		validateIndex(values);
		setIndex(values);
	}
	else if (directive == "autoindex") {
		if (values.size() != 1)
			throw errNumberOfParameters(directive, "location");
		if (values[0] == "on")
        	setAutoindex(true);
        else if (values[0] == "off")
            setAutoindex(false);
        else
            throw std::string("Error: [location] invalid  parameter " + values[0] + " in autoindex directive.");
	}
	else if (directive == "return") {
		if (values.size() != 1)
			throw errNumberOfParameters(directive, "location");
		setReturn(values[0]);
	}
	else if (directive == "cgi_extensions") {
		if (values.size() < 1)
			throw errNumberOfParameters(directive, "location");
		validateCgiExt(values);
		setCgiFileExtention(values);
	}
	else if (directive == "cgi_compiler_path") {
		if (values.size() < 1)
			throw errNumberOfParameters(directive, "location");
		setCgiCompilerPath(values);
	}
	else if (directive == "client_max_body_size") {
		if (values.size() != 1 || isDigit(values[0]) == false)
			throw errNumberOfParameters(directive, "location");
		setClientMaxBodySize(convertString<size_t>(values[0]));
	}
	else if (directive == "root") {
		if (values.size() != 1)
			throw errNumberOfParameters(directive, "location");
		setRoot(values[0]);
	}
	return;
}

void LocationBlock::validateCgiExt(std::vector<std::string> extension) {

	std::vector<std::string>::iterator it = extension.begin();
	for (; it != extension.end(); it++) {
		if ((*it)[0] != '.')
			throw std::string ("Error: Invalid extension [" + *it + "] at \"cgi_extensions\" parameters.");
	}
}

void LocationBlock::validateIndex(std::vector<std::string> index) {

	std::vector<std::string>::iterator it = index.begin();

	for (; it != index.end(); it++) {
		if ((*it).find(".") == std::string::npos)
			throw std::string ("Error: invalid index parameters " + *it);
	}
}

void LocationBlock::DebugLocationBlock(){
	std::cout << "	location { " << std::endl;
	std::cout << "		path: " << getDirectoryPath() << std::endl;
	std::cout << "		alias: " << getAlias() << std::endl;
	std::cout << "		root: " << getRoot() << std::endl;
	if (getIndex().size() != 0){
		std::vector<std::string> index = getIndex();
		for (std::vector<std::string>::iterator it = index.begin(); it != index.end(); it++)
			std::cout << "		index: " << *it << std::endl;
	}
	std::cout << "		autoindex: " << getAutoIndex() << std::endl;

	std::cout << "		cgi_compiler_path: ";
	std::vector<std::string> cgiPath = getCgiCompilerPath();
	std::vector<std::string>::iterator it_p = cgiPath.begin();
	for (; it_p != cgiPath.end(); it_p++)
		std::cout <<  *it_p << " ";
	std::cout << std::endl;

	std::cout << "		cgi_file_extension: ";
	std::vector<std::string> cgiExt = getCgiFileExtention();
	std::vector<std::string>::iterator it_x = cgiExt.begin();
	for (; it_x!= cgiExt.end(); it_x++)
		std::cout <<  *it_x << " ";
	std::cout << std::endl;
	
	std::cout << "		allow_http_methods [GET]: " << std::boolalpha << getAllowMethods()["GET"] << std::endl;
	std::cout << "		allow_http_methods [POST]: " << std::boolalpha << getAllowMethods()["POST"] << std::endl;
	std::cout << "		allow_http_methods [PUT]: " << std::boolalpha << getAllowMethods()["PUT"] << std::endl;
	std::cout << "		allow_http_methods [DELETE]: " << std::boolalpha << getAllowMethods()["DELETE"] << std::endl;
	std::cout << "	} " << std::endl;
}