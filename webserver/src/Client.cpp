#include "Client.hpp"
#include "utils.hpp"

Client::Client(){}

Client::Client(int new_sd, int listen_sd, std::vector<ServerBlock>  &serverBlocks){
	this->_socket = new_sd;
	this->sever_socket = listen_sd;
	this->server_blocks = serverBlocks;
	
	//First Edited
	this->request = new Request;
	request->sever_socket = listen_sd;
	this->_stage = request->_stage;
}

Client::Client(Client const &inst) {
	this->request = inst.request;
	this->sever_socket = inst.sever_socket;
	this->server_blocks = inst.server_blocks;
	this->_response = inst._response;
	this->_socket = inst._socket;
	this->_stage = inst._stage;
	this->_cgi = inst._cgi;
}

Client& Client::operator=(Client const &cli) {
	if (this != &cli) {
		this->_socket = cli._socket;
	}
	return *this;
}

Client::~Client(){}

bool Client::httpStage() {
	if (_stage == FIRST_LINE) {
		// std::cout << GREEN << "FIRSTLINE \n" << DEFAULT << std::endl;
		_stage = request->parseFirstLine(_stage, _socket);
	} 
	if (_stage == HEADER) {
		// std::cout << GREEN << "HEADER \n" << DEFAULT << std::endl;
		_stage = request->parseHeader(_stage);
	}
	if (_stage == BODY) {
		// std::cout << GREEN << "BODY \n" << DEFAULT << std::endl;
		_stage = request->parseBody (_stage);
	}
	if (_stage == ROUTER) {
		// std::cout << GREEN << "ROUTER \n" << DEFAULT << std::endl;
		_cgi.initCgi(request->errNum, _socket, request->server_blocks, *request);
		_stage = _cgi.apiRouter();
	}
	if (_stage == RESPONSED) {
		request->clear();
		_stage = FIRST_LINE;
	}
	return (false); 
}

time_t Client::getWorkingTime() {

    return this->_workingTime;
}
        
void Client::updateWorkingTime() {
    this->_workingTime = time(NULL);
}