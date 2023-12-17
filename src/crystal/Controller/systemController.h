// The ifndef is used so that the same File is not loaded again
#ifndef INCLUDED_SYSTEM_CONTROLLER
#define INCLUDED_SYSTEM_CONTROLLER

#include "../Request/Request.h"
#include "../Response/Response.h"

namespace Crystal::Controller {
void defaultNotFound(Crystal::Request &request, Crystal::Response &response);
void getPublicFile(Crystal::Request &request, Crystal::Response &response);
}  // namespace Crystal::Controller

#endif