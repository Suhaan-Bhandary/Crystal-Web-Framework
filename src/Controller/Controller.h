// The ifndef is used so that the same File is not loaded again
#ifndef INCLUDED_CONTROLLER
#define INCLUDED_CONTROLLER

#include "../Request/Request.h"
#include "../Response/Response.h"

namespace Controller {
void getHome(http::Request &request, http::Response &response);
void getUser(http::Request &request, http::Response &response);
void getUserChat(http::Request &request, http::Response &response);
void getNotFound(http::Request &request, http::Response &response);
void defaultNotFound(http::Request &request, http::Response &response);
void getPublicFile(http::Request &request, http::Response &response);
}  // namespace Controller

#endif