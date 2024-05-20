#include "mock.hpp"

// std::stringstream createMockData() {

// 	std::stringstream mock_data;

// 	mock_data << "POST / HTTP/1.1\r\n"
// 			  << "user-agent: got (https://github.com/sindresorhus/got)\r\n"
// 			  << "accept-encoding: gzip, deflate, br\r\n"
// 			  << "Host: localhost:9000\r\n"
// 			  << "Connection: close\r\n"
// 			  << "Content-Length: 0\r\n"
// 			  << "\r\n"; 
// }

std::stringstream createMockData() {

	std::stringstream mock_data;

	// Append the request line
	mock_data << "POST / HTTP/1.1\r\n";
	
	// Append headers
	mock_data << "content-type: multipart/form-data; boundary=--------------------------149922245000676558181503\r\n";
	mock_data << "content-length: 1616\r\n";
	mock_data << "accept-encoding: gzip, deflate, br\r\n";
	mock_data << "Accept: */*\r\n";
	mock_data << "User-Agent: Thunder Client (https://www.thunderclient.com)\r\n";
	mock_data << "Host: localhost:9000\r\n";
	mock_data << "Connection: close\r\n";
	mock_data << "\r\n";

	// Append the body
	mock_data << "----------------------------149922245000676558181503\r\n";
	mock_data << "Content-Disposition: form-data; name=\"conf\"; filename=\"default.conf\"\r\n";
	mock_data << "Content-Type: text/plain\r\n";
	mock_data << "\r\n";
	mock_data << "server {\r\n"
			  << "    listen 9000;\r\n"
			  << "\r\n"
			  << "    server_name plertsir.42.th;\r\n"
			  << "    host 127.0.0.1;\r\n"
			  << "    root docs/curl;\r\n"
			  << "    client_max_body_size 3000000;\r\n"
			  << "    index index1.html index2.html;\r\n"
			  << "    error_page 404 default-error/404.html;\r\n"
			  << "    autoindex on;\r\n"
			  << "\r\n"
			  << "    location /tours {\r\n"
			  << "        alias docs/curl/test;\r\n"
			  << "        allow_http_methods GET POST PUT DELETE;\r\n"
			  << "        autoindex off;\r\n"
			  << "    }\r\n"
			  << "\r\n"
			  << "    location / {\r\n"
			  << "        alias docs/curl/test;\r\n"
			  << "        allow_http_methods GET POST PUT DELETE;\r\n"
			  << "        autoindex off;\r\n"
			  << "    }\r\n"
			  << "\r\n"
			  << "    location /cgi-bin {\r\n"
			  << "        root ./;\r\n"
			  << "        allow_http_methods GET POST DELETE;\r\n"
			  << "        index hi.py;\r\n"
			  << "        cgi_compiler_path /usr/bin/python3 /bin/bash /usr/bin/perl;\r\n"
			  << "        cgi_extensions .py .sh .pl;\r\n"
			  << "    }\r\n"
			  << "}\r\n"
			  << "\r\n"
			  << "server {\r\n"
			  << "    listen 9001;\r\n"
			  << "\r\n"
			  << "    server_name plertsir.42.th;\r\n"
			  << "    host 127.0.0.1;\r\n"
			  << "    root docs/curl;\r\n"
			  << "    client_max_body_size 3000000;\r\n"
			  << "    index index1.html index2.html;\r\n"
			  << "    error_page 404 default-error/404.html;\r\n"
			  << "    autoindex on;\r\n"
			  << "\r\n"
			  << "    location /tours {\r\n"
			  << "        alias docs/curl/test;\r\n"
			  << "        allow_http_methods GET POST PUT DELETE;\r\n"
			  << "        autoindex off;\r\n"
			  << "    }\r\n"
			  << "\r\n"
			  << "    location / {\r\n"
			  << "        alias docs/curl/test;\r\n"
			  << "        allow_http_methods GET POST PUT DELETE;\r\n"
			  << "        autoindex off;\r\n"
			  << "    }\r\n"
			  << "\r\n"
			  << "    location /cgi-bin {\r\n"
			  << "        root ./;\r\n"
			  << "        allow_http_methods GET POST DELETE;\r\n"
			  << "        index hi.py;\r\n"
			  << "        cgi_compiler_path /usr/bin/python3 /bin/bash /usr/bin/perl;\r\n"
			  << "        cgi_extensions .py .sh .pl;\r\n"
			  << "    }\r\n"
			  << "}\r\n";
	mock_data << "----------------------------149922245000676558181503--\r\n";

	return (mock_data);
}

bool	readFile(std::string file_location, std::string& buffer)
{
	std::ifstream file(file_location.c_str());
	std::string temp_string;

	buffer = "";

	if (!file.is_open())
		return(false);
	while (file)
	{
		std::getline(file, temp_string);
		buffer.append(temp_string);
		buffer.append("\n");
		temp_string = "";
	}
	file.close();
	return (true);
}

void	createMockResponse(int write_sd) {
	t_res res;

	std::cout << YELLOW << "Write Response on : socket [" << write_sd << "]" << DEFAULT << std::endl;
	readFile("./src/page.html", res.htmlContent);
					  
	res.httpResponse = "HTTP/1.1 200 OK\r\n"
					   "Content-Type: text/html; charset=UTF-8\r\n"
					   "Connection: close\r\n\r\n";
	
	send(write_sd, res.httpResponse, strlen(res.httpResponse), 0);
	send(write_sd, res.htmlContent.c_str(), res.htmlContent.length(), 0);


}
	// if (_connections == "keepalive")
	// clearMasterSet(write_sd, &_write_set);
	// addMasterSet(write_sd, &_read_set);
	// // else{
	// closeConn(write_sd);
	// }