// The ifndef is used so that the same File is not loaded again
#ifndef INCLUDED_Response
#define INCLUDED_Response

#include <string>
#include <unordered_map>

namespace http {
class Response {
    int client_socket;
    int statusCode = 200;  // Default status code is 200
    const static std::unordered_map<int, std::string> statusCodes;

   public:
    Response(int client_socket);
    void sendHTML(const std::string &html);
    void send404();
    void sendTemplate(const std::string &templateName);
    void sendPublicFile(const std::string &relativePathToPublic);
    void sendJson(const std::string &jsonString);
    void sendResponse(const std::string &response_body,
                      const std::string &response_type);

    void setStatusCode(int status);
};
}  // namespace http

#endif