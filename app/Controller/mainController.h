#ifndef INCLUDED_MAIN_CONTROLLER
#define INCLUDED_MAIN_CONTROLLER

#include <iostream>

#include "../../src/Logger/Logger.h"
#include "../../src/Request/Request.h"
#include "../../src/Response/Response.h"

namespace Controller {
void getHome(http::Request &request, http::Response &response);
void getUser(http::Request &request, http::Response &response);
void getUserChat(http::Request &request, http::Response &response);
void getNotFound(http::Request &request, http::Response &response);
void saveUserData(http::Request &request, http::Response &response);
void redirectUserToGoogle(http::Request &request, http::Response &response);
void getXOGame(http::Request &request, http::Response &response);
void getHealth(http::Request &request, http::Response &response);
};  // namespace Controller

#endif