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
void http::Router::route(int &client_socket) {
    // Reading the request
    // Read the request from the client
    char requestBuffer[REQUEST_Buffer_SIZE] = {0};
    if (read(client_socket, requestBuffer, REQUEST_Buffer_SIZE) == -1) {
        Logger::log("Request Read Failed!!");
        close(client_socket);
        return;
    }

    // Reading the request
    http::Request request = http::Request(requestBuffer);

    // Returning the response
    http::Response response = http::Response(client_socket);
    std::string htmlContent =
        "<html><head><title>Hello World</title></head><body><h1>Hello "
        "World!</h1></body></html>\r\n";
    response.sendHTML(htmlContent);
}