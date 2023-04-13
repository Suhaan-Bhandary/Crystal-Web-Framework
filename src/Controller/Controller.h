#include "../Request/Request.h"
#include "../Response/Response.h"

namespace Controller {
void getHome(http::Request &request, http::Response &response);
void getNotFound(http::Request &request, http::Response &response);
}  // namespace Controller
