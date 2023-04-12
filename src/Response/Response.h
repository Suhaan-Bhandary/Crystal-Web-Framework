#include <iostream>

namespace http {
class Response {
    int client_socket;

   public:
    Response(int client_socket);
    void sendHTML(const std::string &html);
};
}  // namespace http