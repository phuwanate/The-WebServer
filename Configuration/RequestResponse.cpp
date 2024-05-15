#include "RequestResponse.hpp"

void build_request(t_req &req) {

   req.method = "GET";
   req.serverName = "plertsir.42.th";
   req.path = "/";
}

void build_response(t_res &res) {

   res.htmlContent = "<!DOCTYPE html>\n"
                      "<html>\n"
                      "<head>\n"
                      "    <title>Simple Page</title>\n"
                      "</head>\n"
                      "<body>\n"
                      "    <h1>👋 Welcome to My Simple Page</h1>\n"
                      "    <p>This is a simple HTML page served by a C++ server.</p>\n"
                      "</body>\n"
                      "</html>";
   
   res.httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n";
}
