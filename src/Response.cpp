#include "Response.hpp"
#include <cstring>
Response::Response(){
	_status = initStatusrMsg();
}

Response::Response(Response const &other) {
	this->_status = other._status;
}

Response& Response::operator=(Response const &other) {
	if (this != &other) {
		this->_status = other._status;
	}
	return *this;
}

Response::~Response() {
}

void Response::errorDefault(int socket, std::string defLoc, int status) {
	if (defLoc.empty()){
		byStatus(socket, status);
		return ;
	}
	byFile(socket, 404, defLoc, "text/html; charset=UTF-8");
}

void Response::byStatus(int socket, int status){
	std::string firstLine = createFirstLine(status);
	std::string body = createBodyByStatus(status);
	std::string response = createResponse(firstLine, body, "text/html");

	if (send(socket, response.c_str(), response.size(), 0) < 0)
		std::cout << "Sending Error" << std::endl;

}

void Response::byStringstream(int socket, int status, std::stringstream &file, const std::string &content_type) {
	std::string firstLine = createFirstLine(status);
	std::string response = createResponse(firstLine, file.str(), content_type);

	send(socket, response.c_str(), response.size(), 0);
}

void Response::byFile(int socket, int status, std::string const &location,  std::string const &content_type){
	std::string firstLine = createFirstLine(status);
	Response res;

	std::string body;
	if (!readFile(location, body)) {
		res.byStatus(socket, 404);
	}
	std::string response = createResponse(firstLine, body, content_type);
	send(socket, response.c_str(), response.size(), 0);
}

void Response::byRedirect(int socket, int status, std::string const &location) {
	std::stringstream ss;

	ss << createFirstLine(status) << "\r\n"; 
	ss << "Location: " << location << "\r\n";
	ss << "Content-Length: 0\r\n";
    ss << "Connection: close\r\n";
    ss << "\r\n";

	send(socket, ss.str().c_str(), ss.str().size(), 0);
}

void Response::byAutoIndex(int socket, int status, const std::string& directory_path) {
	std::string firstLine = createFirstLine(status);
	std::string body = buildIndex(directory_path);
	std::string response = createResponse(firstLine, body, "text/html");

	send(socket, response.c_str(), response.size(), 0);
}

std::string Response::buildIndex(const std::string& directory_path)
{
    std::string body;
    DIR *directory;
    struct dirent *dir_entry;

    if ((directory = opendir(directory_path.c_str())) != NULL)
    {
        body.append("<!DOCTYPE html>\n<html>\n\t<head>\n");
        body.append("\t\t<title> index of " + directory_path + "</title>\n");
        body.append("\t\t<style>\n\t\t\ttable, th, td{\n\t\t\t\tborder-collapse: collapse;\n\t\t\t}\n");
        body.append("\t\t\tth, td{\n\t\t\t\tpadding: 5px;\n\t\t\t}\n");
        body.append("\t\t\tth {\n\t\t\t\ttext-align: left;\n\t\t}\n");
        body.append("\t\t</style>\n\t</head>\n\n");

        body.append("\t<body>\n\t<h1> Index of " + directory_path + "</h1>\n");
        body.append("\t\t<table style=\"width:100%; font-size: 15px\">\n");
        body.append("\t\t\t<tr>\n\t\t\t\t<th style=\"width:60%\">File Name</th>\n");
        body.append("\t\t\t\t<th style=\"width:300\">Last Modification</th>\n");
        body.append("\t\t\t\t<th style=\"width:100\">File Size</th>\n");
        body.append("\t\t\t</tr>\n");

        while ((dir_entry = readdir(directory)) != NULL)
        {
            struct stat f_stat;
            std::string f_name = dir_entry->d_name;
            stat(f_name.c_str(), &f_stat);

            //file name
            // body.append("\t\t\t<tr>\n\t\t\t\t<td>" + f_name);
            // if (S_ISDIR(f_stat.st_mode))
            //     body.append("/");
            // body.append("</td>\n");
			// File name with link
			body.append("\t\t\t<tr>\n\t\t\t\t<td><a href=\"" + f_name + "\">" + f_name);
			if (S_ISDIR(f_stat.st_mode))
				body.append("/");
			body.append("</a></td>\n");


            //file last modified time
            body.append("\t\t\t\t<td>");
            body.append(ctime(&f_stat.st_mtime));
            body.append("</td>\n");

            //file size
            body.append("\t\t\t\t<td>");
            if (S_ISDIR(f_stat.st_mode))
                body.append("-");
            else{
                std::ostringstream stream;
				stream << f_stat.st_size;
				body.append(stream.str());
			}
			body.append("</td>\n\t\t\t</tr>");
        }
        closedir(directory);
        body.append("\t\t</table>\n\t</body>\n</html>\n");
        return body;
    }
    else
        return "";
}

std::string Response::createFirstLine(int status) {
	// std::string firstLine = "HTTP/1.1 " + std::to_string(status) + " " + _status[status];
	std::string firstLine = "HTTP/1.1 " + ft_to_string(status) + " " + _status[status];
	return (firstLine);
}

std::string Response::ft_to_string(int val) {

	std::ostringstream conv;

	conv << val;

	return (conv.str());
}

std::string Response::createBodyByStatus(int status) {
	std::stringstream body;

	body << "<!DOCTYPE html>" << std::endl;
	body << "<html>" << std::endl;
	body << "<head>" << std::endl;
	body << "<title>" << status << " " << _status[status] << "</title>" << std::endl;
	body << "</head>" << std::endl;
	body << "<body>" << std::endl;
	body << "<h1>" << status << " " << _status[status] << "</h1>" << std::endl;
	body << "</body>" << std::endl;
	body << "</html>" << std::endl;
	return(body.str());
}

std::string Response::createResponse(std::string firstLine, std::string body, std::string contentType) {
	std::stringstream ss;
	
	ss << firstLine << "\r\n";
	ss << "Connection: Keep-Alive\r\n";
	ss << "Keep-Alive: timeout=5, max=20\r\n";
	ss << "Content-Type: " << contentType << "\r\n";
	ss << "Content-Length: " << body.size() << "\r\n";
	ss << "\r\n";  // end of headers
	ss << body;
	return(ss.str());
}

bool	Response::readFile(std::string file_location, std::string& buffer){
	std::ifstream file(file_location.c_str());
	std::string    temp_string;

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