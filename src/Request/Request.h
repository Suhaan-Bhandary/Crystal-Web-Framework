// The ifndef is used so that the same File is not loaded again
#ifndef INCLUDED_REQUEST
#define INCLUDED_REQUEST

#include <iostream>

namespace http {
class Request {
   public:
    std::string header, body;

    Request(char requestBuffer[]);
    void readRequest(char requestBuffer[]);
};
}  // namespace http

#endif