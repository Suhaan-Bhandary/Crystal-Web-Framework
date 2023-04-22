#include "./Response.h"

#include <sys/socket.h>
#include <unistd.h>

#include <fstream>
#include <sstream>

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
    std::string rootDir = Utils::getCurrentDirectory();
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

void http::Response::sendPublicFile(const std::string& relativePathToPublic) {
    Logger::log(relativePathToPublic);

    // Find the type of the file
    std::string extension = Utils::split(relativePathToPublic, ".")[1];

    std::string type = "application/octet-stream";

    // TODO: Only this 3 file types are supported for now
    if (extension == "html") {
        type = "text/html";
    } else if (extension == "js") {
        type = "text/javascript";
    } else if (extension == "css") {
        type = "text/css";
    } else if (extension == "jpg") {
        type = "image/jpeg";
    }

    // Send HTTP header
    std::string header;
    header += "HTTP/1.1 200 OK\r\n";
    header += "Content-Type: " + type + "\r\n";
    header += "\r\n";

    send(client_socket, header.c_str(), header.size(), 0);

    // * File is opened in binary format as images will deform in text format
    std::ifstream file(
        Utils::getCurrentDirectory() + "/src/public" + relativePathToPublic,
        std::ios::binary);

    // Read the file contents
    std::ostringstream file_stream;
    file_stream << file.rdbuf();
    std::string response_body = file_stream.str();

    // Send file contents
    send(client_socket, response_body.c_str(), response_body.length(), 0);

    file.close();
}
