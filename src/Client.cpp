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
}

Client& Client::operator=(Client const &cli) {
	if (this != &cli) {
		this->_socket = cli._socket;
	}
	return *this;
}

Client::~Client(){}

bool Client::httpStage() {
	// Cgi cgi(request.method, request.path, request.header, request.body); 

	switch (_stage) {
		case FIRST_LINE: {
			_stage = request.parseFirstLine(_stage);
		}
		case HEADER: {
			_stage = request.parseHeader(_stage); 
		}
		case BODY: {
			_stage = request.parseBody(_stage);
		}
		case ROUTER: {
			// _stage = cgi.router(_stage);
			// _response.error404(_socket, errLocation);
			// _response.byFile(_socket, 404, errLocation, "text/html; charset=UTF-8"); // test response
			// _response.byFile(_socket, 200, "./page-copy.html", "text/html; charset=UTF-8"); // test response
			// _response.byStatus(_socket, 307); // test response
			// std::stringstream mockStringstream = createMockData();// test response
			// _response.byStringstream(_socket, 200, mockStringstream,"text/html");// test response
			_cgi.initCgi(request.errNum, _socket, request.server_blocks, request);
			_stage = _cgi.apiRouter();
		}
		case RESPONSED: {
			request.clear();
			_stage = FIRST_LINE;
			return (true); 
		}
		default:
			break;
	}
	return (false); 
}