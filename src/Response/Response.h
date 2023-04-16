// The ifndef is used so that the same File is not loaded again
#ifndef INCLUDED_Response
#define INCLUDED_Response

#include <string>

namespace http {
class Response {
    int client_socket;

   public:
    Response(int client_socket);
    void sendHTML(const std::string& html);
    void send404();
    void sendTemplate(const std::string& templateName);
};
}  // namespace http

#endif