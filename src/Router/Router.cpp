#include "./Router.h"

#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

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

    // Send html response to the client
    std::string htmlContent =
        "<html>"
        "<head><title>Hello World</title></head>"
        "<body><h1>Hello World!</h1></body>"
        "</html>\r\n";

    response.sendHTML(htmlContent);
}