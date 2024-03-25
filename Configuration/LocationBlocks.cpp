#include "LocationBlocks.hpp"

LocationBlocks::LocationBlocks() {

	return;
}

LocationBlocks::LocationBlocks(std::string const &locationBlock) {

    //get every directives to all variable in LocationBlocks class
	(void)locationBlock;
	return;
}

LocationBlocks::LocationBlocks(LocationBlocks const &locationBlockInstance) {

	*this = locationBlockInstance;
}

LocationBlocks &LocationBlocks::operator= (LocationBlocks const &locationBlockInstance) {

	if (this != &locationBlockInstance) {

		this->_path = locationBlockInstance._path;
		this->_root = locationBlockInstance._root;
		this->_index = locationBlockInstance._index;
		this->_alias = locationBlockInstance._alias;
		this->_return = locationBlockInstance._return;
		this->_autoIndex = locationBlockInstance._autoIndex;
		this->_allowMethods = locationBlockInstance._allowMethods;
		this->_cgiCompilerPath = locationBlockInstance._cgiCompilerPath;
		this->_cgiFileExtention = locationBlockInstance._cgiFileExtention;
		this->_clientMaxBodySize = locationBlockInstance._clientMaxBodySize;
	}
	return *this;
}

LocationBlocks::~LocationBlocks() {

	return;
}
