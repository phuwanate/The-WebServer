#ifndef REQUESTRESPONSE_HPP
# define REQUESTRESPONSE_HPP

#include <string>

typedef struct s_req {

   std::string serverName;
   std::string path;
   std::string method;

} t_req;

typedef struct s_res {

   std::string htmlContent;
   const char* httpResponse;
} t_res;

void build_request(t_req &req);
void build_response(t_res &res);

#endif