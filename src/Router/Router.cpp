#include "./Router.h"

#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

#include "../Controller/Controller.h"
#include "../Logger/Logger.h"
#include "../Request/Request.h"
#include "../Response/Response.h"

// Define a size for the request buffer
#define REQUEST_Buffer_SIZE 1024

http::Router::Router() {}

// The function will contain the routing logic for the application
void http::Router::route(http::Request &request, http::Response &response) {
    Logger::log("Method: " + request.getValue("method"));
    Logger::log("Path: " + request.getValue("path"));
    Logger::log("Host: " + request.getValue("Host"));
    Logger::log("");

    if (request.getValue("path") == "/") {
        Controller::getHome(request, response);
    } else {
        Controller::getNotFound(request, response);
    }
}