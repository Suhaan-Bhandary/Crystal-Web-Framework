// The ifndef is used so that the same File is not loaded again
#ifndef INCLUDED_SYSTEM_CONTROLLER
#define INCLUDED_SYSTEM_CONTROLLER

#include "../Request/Request.h"
#include "../Response/Response.h"

namespace Controller {
void defaultNotFound(http::Request &request, http::Response &response);
void getPublicFile(http::Request &request, http::Response &response);
}  // namespace Controller

#endif