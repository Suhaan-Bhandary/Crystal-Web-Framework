#include "./Router.h"

#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

#include "../Logger/Logger.h"
#include "../Request/Request.h"

// Define a size for the request buffer
#define REQUEST_Buffer_SIZE 1024

void http::router(int &client_socket) {
    // Reading the request
    // Read the request from the client
    char requestBuffer[REQUEST_Buffer_SIZE] = {0};
    if (read(client_socket, requestBuffer, REQUEST_Buffer_SIZE) == -1) {
        std::cerr << "Request Read Failed!!" << std::endl;
        close(client_socket);
        return;
    }

    // Reading the request
    http::Request request = http::Request(requestBuffer);

    // Returning the response
    std::string htmlResponse =
        "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    htmlResponse +=
        "<html><head><title>Hello World</title></head><body><h1>Hello "
        "World!</h1></body></html>\r\n";

    // Sending response
    long bytesSent =
        write(client_socket, htmlResponse.c_str(), htmlResponse.size());
}