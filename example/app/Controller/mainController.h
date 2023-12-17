#ifndef INCLUDED_MAIN_CONTROLLER
#define INCLUDED_MAIN_CONTROLLER

#include <iostream>

#include "crystal.h"

namespace Controller {
void getHome(Crystal::Request &request, Crystal::Response &response);
void getUser(Crystal::Request &request, Crystal::Response &response);
void getUserChat(Crystal::Request &request, Crystal::Response &response);
void getNotFound(Crystal::Request &request, Crystal::Response &response);
void saveUserData(Crystal::Request &request, Crystal::Response &response);
void redirectUserToGoogle(Crystal::Request &request,
                          Crystal::Response &response);
void getXOGame(Crystal::Request &request, Crystal::Response &response);
void getHealth(Crystal::Request &request, Crystal::Response &response);
};  // namespace Controller

#endif