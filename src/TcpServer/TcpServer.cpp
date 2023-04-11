#include "TcpServer.h"

#include <sys/socket.h>
#include <unistd.h>

#include <vector>

#include "../Logger/Logger.h"
#include "../Utils/Utils.h"

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

    // Infinite loop to keep on listening
    while (true) {
        // Waiting for a connection
        // and then creating a new socket for the request
        struct sockaddr_in client_address;
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
            std::cerr << "Request Read Failed!!" << std::endl;
            close(client_socket);
            continue;
        }

        // ! Checking if split works or not
        std::vector<std::string> res =
            Utils::split(std::string(requestBuffer), "\r\n\r\n");

        for (auto block : res) {
            std::cout << "-----------------------------" << std::endl;
            std::cout << block << std::endl;
        }

        // Returning the response
        std::string htmlResponse =
            "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        htmlResponse +=
            "<html><head><title>Hello World</title></head><body><h1>Hello "
            "World!</h1></body></html>\r\n";

        // Sending response
        long bytesSent =
            write(client_socket, htmlResponse.c_str(), htmlResponse.size());

        // Clearing the new request socket
        close(client_socket);
    }
}
}  // namespace http
