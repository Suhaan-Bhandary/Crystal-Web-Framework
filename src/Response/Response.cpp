#include "./Response.h"

#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <vector>

#include "../Logger/Logger.h"
#include "../Utils/Utils.h"

http::Response::Response(int client_socket) {
    this->client_socket = client_socket;
}

void http::Response::sendHTML(const std::string &html) {
    std::string httpResponse = "";
    httpResponse += "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    httpResponse += html;

    // Sending response
    long bytesSent =
        write(client_socket, httpResponse.c_str(), httpResponse.size());
}