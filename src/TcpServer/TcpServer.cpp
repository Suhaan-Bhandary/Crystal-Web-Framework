#include "TcpServer.h"

#include <sys/socket.h>
#include <unistd.h>

#include "../Logger/Logger.h"
#include "../Request/Request.h"
#include "../Response/Response.h"
#include "../Router/Router.h"

// Define a size for the request buffer
#define REQUEST_Buffer_SIZE 1024

namespace http {
TcpServer::TcpServer(const std::string &ip_address, int port)
    : server_ip_address(ip_address), server_port(port), server_socketAddress() {
    // Creating  a socket address to bind with socket
    server_socketAddress.sin_family = AF_INET;
    server_socketAddress.sin_port = htons(server_port);
    server_socketAddress.sin_addr.s_addr = inet_addr(server_ip_address.c_str());

    startServer();
}

TcpServer::~TcpServer() { closeServer(); }

int TcpServer::startServer() {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1) {
        Logger::log("Cannot create Socket");
        exit(1);
    }

#ifdef DEVELOPMENT_ENVIRONMENT
    Logger::log(
        "Development Environment: SO_REUSEADDR | SO_REUSEPORT in "
        "TcpServer::startServer");

    // Forcefully attaching socket to the port 8080
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
#endif

    if (bind(server_socket, (sockaddr *)&server_socketAddress,
             sizeof(server_socketAddress))) {
        Logger::log("Cannot connect socket to address");
        exit(1);
    }

    return 0;
}

void TcpServer::closeServer() {
    close(server_socket);
    exit(0);
}

void TcpServer::startListen() {
    if (listen(server_socket, 30) == -1) {
        Logger::log("Socket Listening Failed");
        exit(1);
    }

    // Listening Started
    Logger::log("Listening on Address: " +
                (std::string)inet_ntoa(server_socketAddress.sin_addr));
    Logger::log("Port: " + std::to_string(server_port));

    // Router to handle request
    http::Router router = http::Router();
    router.displayAllRoutes();

    // Infinite loop to keep on listening
    while (true) {
        // Waiting for a connection
        // and then creating a new socket for the request
        sockaddr_in client_address;
        socklen_t size = sizeof(client_address);
        int client_socket =
            accept(server_socket, (sockaddr *)&client_address, &size);

        if (client_socket == -1) {
            Logger::log(
                "Server Failed to accept Incoming Connection "
                "Address: " +
                (std::string)inet_ntoa(client_address.sin_addr) +
                " Port: " + std::to_string(client_address.sin_port));
            continue;
        }

        // Reading the new request socket
        Logger::log("Received Request from client from Address: " +
                    (std::string)inet_ntoa(client_address.sin_addr) +
                    " Port: " + std::to_string(client_address.sin_port));

        // Reading the request
        // Read the request from the client
        char requestBuffer[REQUEST_Buffer_SIZE] = {0};
        if (read(client_socket, requestBuffer, REQUEST_Buffer_SIZE) == -1) {
            Logger::log("Request Read Failed!!");
            close(client_socket);
            return;
        }

        // Reading the request, and creating response
        http::Request request = http::Request(requestBuffer);
        http::Response response = http::Response(client_socket);

        router.route(request, response);

        // Clearing the new request socket
        close(client_socket);
    }
}
}  // namespace http
