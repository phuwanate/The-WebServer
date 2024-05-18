#include "RequestResponse.hpp"
#include <iostream>
#include <fstream>


bool     readFile(std::string file_location, std::string& buffer)
{
    std::ifstream file(file_location.c_str());
    std::string    temp_string;

    buffer = "";

    if (file.is_open())
    {
        while (file)
        {
            std::getline(file, temp_string);
            buffer.append(temp_string);
            buffer.append("\n");
            temp_string = "";
        }
        file.close();
    } else {
        return (false);
    }

    return (true);
}

void build_request(t_req &req) {

   req.method = "GET";
   req.serverName = "plertsir.42.th";
   req.path = "/";
}

void build_response(t_res &res) {

   // res.htmlContent = "<!DOCTYPE html>\n"
   //                    "<html>\n"
   //                    "<head>\n"
   //                    "    <title>Simple Page</title>\n"
   //                    "</head>\n"
   //                    "<body>\n"
   //                    "    <h1>👋 Welcome to My Simple Page</h1>\n"
   //                    "    <p>This is a simple HTML page served by a C++ server.</p>\n"
   //                    "</body>\n"
   //                    "</html>\n";
   
   readFile("./src/page.html", res.htmlContent);
                      
   // res.httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n";
   res.httpResponse = "HTTP/1.1 200 OK\r\n"
                       "Content-Type: text/html; charset=UTF-8\r\n"
                       "Connection: close\r\n\r\n";
   
}
