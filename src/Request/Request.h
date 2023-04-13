// The ifndef is used so that the same File is not loaded again
#ifndef INCLUDED_REQUEST
#define INCLUDED_REQUEST

#include <string>
#include <unordered_map>

namespace http {
class Request {
    std::unordered_map<std::string, std::string> valueMap;

   public:
    std::string header, body;

    Request(char requestBuffer[]);
    void readRequest(char requestBuffer[]);
    void parseDataFromHeader();
    std::string getValue(std::string key);
};
}  // namespace http

#endif