#include "Client.hpp"
#include "utils.hpp"

Client::Client(){}

Client::Client(int new_sd, int listen_sd, std::vector<ServerBlock>  serverBlocks){
	this->_socket = new_sd;
	this->sever_socket = listen_sd;
	this->server_blocks = serverBlocks;
	this->request.sever_socket = listen_sd;
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

	switch (_stage) {
		case FIRST_LINE: {
			// std::cout << RED << "Request path firstline: " << DEFAULT << std::endl;
			_stage = request.parseFirstLine(_stage);
		}
		case HEADER: {
			// std::cout << RED << "Request path header: " << request.path << DEFAULT << std::endl;
			_stage = request.parseHeader(_stage); 
		}
		case BODY: {
			// std::cout << RED << "Request path body: " << request.path << DEFAULT << std::endl;
			_stage = request.parseBody(_stage);
		}
		case ROUTER: {
			// std::cout << RED << "Request path before: " << request.path << DEFAULT << std::endl;
			_cgi.initCgi(request.errNum, _socket, request.server_blocks, request);
			_stage = _cgi.apiRouter();
		}
		case RESPONSED: {
			request.clear();
			_stage = FIRST_LINE;
			return (true); 
		}
		default:
			std::cout << "Stage: " << _stage << std::endl;
			break;
	}
	return (false); 
}