#include "./Response.h"

#include <unistd.h>

#include <fstream>

#include "../Logger/Logger.h"
#include "../Utils/Utils.h"

http::Response::Response(int client_socket) {
    this->client_socket = client_socket;
}

void http::Response::sendHTML(const std::string& html) {
    std::string httpResponse = "";
    httpResponse += "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    httpResponse += html;

    // Sending response
    long bytesSent =
        write(client_socket, httpResponse.c_str(), httpResponse.size());
}

void http::Response::send404() {
    std::string httpResponse = "";
    httpResponse +=
        "HTTP/1.1 404 NOT FOUND\r\nContent-Type: text/plain\r\n\r\n";
    httpResponse += "Page Not Found!!";

    // Sending response
    long bytesSent =
        write(client_socket, httpResponse.c_str(), httpResponse.size());
}

void http::Response::sendTemplate(const std::string& templateName) {
    // Read the file
    std::string rootDir = Utils::get_current_dir();
    std::string templateURL = rootDir + "/src/templates/" + templateName;
    std::ifstream HTMLFile(templateURL);

    if (!HTMLFile.is_open()) {
        Logger::log("Error in opening file!!");
        return;
    }

    std::string HTMLContent((std::istreambuf_iterator<char>(HTMLFile)),
                            (std::istreambuf_iterator<char>()));

    // close the file
    HTMLFile.close();

    std::string httpResponse = "";
    httpResponse += "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    httpResponse += HTMLContent;

    // Sending response
    long bytesSent =
        write(client_socket, httpResponse.c_str(), httpResponse.size());
}
