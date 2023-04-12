// The ifndef is used so that the same File is not loaded again
#ifndef INCLUDED_ROUTER
#define INCLUDED_ROUTER

#include <sys/socket.h>
#include <unistd.h>

#include "../Request/Request.h"
#include "../Response/Response.h"

namespace http {
class Router {
   private:
   public:
    Router();
    void route(http::Request &request, http::Response &response);
};
}  // namespace http

#endif