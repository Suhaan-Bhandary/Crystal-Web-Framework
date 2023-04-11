#include <iostream>

namespace http {
class Request {
   public:
    std::string header, body;

    Request(char requestBuffer[]);
    void readRequest(char requestBuffer[]);
};
}  // namespace http