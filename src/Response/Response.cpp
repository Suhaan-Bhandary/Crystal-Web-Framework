#include "./Response.h"

#include <unistd.h>

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