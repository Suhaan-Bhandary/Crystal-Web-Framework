#include "./Response.h"

#include <sys/socket.h>
#include <unistd.h>

#include <fstream>
#include <sstream>
#include <stdexcept>

#include "../HTMLTemplate/HTMLTemplate.h"
#include "../Json/Json.h"
#include "../Logger/Logger.h"
#include "../Utils/Utils.h"

http::Response::Response(int client_socket) {
    this->client_socket = client_socket;
}

void http::Response::sendHTML(const std::string& html) {
    // Send Response
    sendResponse(html, "text/html");
}

void http::Response::send404() {
    std::string response_body = "Page Not Found!!";

    // Send Response
    setStatusCode(404);
    sendResponse(response_body, "text/plain");
}

void http::Response::sendTemplate(const std::string& templateName,
                                  const Json::Json& data) {
    // Read the file
    std::string rootDir = Utils::getCurrentDirectory();
    std::string templateURL = rootDir + "/src/templates/" + templateName;

    // Create template object
    http::HTMLTemplate html_template(templateURL, data);

    std::string HTMLContent = html_template.getHtml();

    // Send Response
    sendResponse(HTMLContent, "text/html");
}

void http::Response::sendPublicFile(const std::string& relativePathToPublic) {
    Logger::log(relativePathToPublic);

    // Find the type of the file
    std::string extension = Utils::split(relativePathToPublic, ".")[1];

    std::string response_type = "application/octet-stream";

    // TODO: Only this 3 file types are supported for now
    if (extension == "html") {
        response_type = "text/html";
    } else if (extension == "js") {
        response_type = "text/javascript";
    } else if (extension == "css") {
        response_type = "text/css";
    } else if (extension == "jpg") {
        response_type = "image/jpeg";
    }

    // * File is opened in binary format as images will deform in text format
    std::ifstream file(
        Utils::getCurrentDirectory() + "/src/public" + relativePathToPublic,
        std::ios::binary);

    // Read the file contents
    std::ostringstream file_stream;
    file_stream << file.rdbuf();
    std::string response_body = file_stream.str();
    file.close();

    // Send Response
    setStatusCode(200);
    sendResponse(response_body, response_type);
}

void http::Response::sendJson(const std::string& jsonString) {
    std::string response_type = "application/json";
    sendResponse(jsonString, response_type);
}

void http::Response::sendResponse(const std::string& response_body,
                                  const std::string& response_type) {
    // Get the status code, if not found out of range exception
    std::string response_status = statusCodes.at(statusCode);

    // Send HTTP response_header
    std::string response_header;
    response_header += "HTTP/1.1 " + response_status + "\r\n";
    response_header += "Content-Type: " + response_type + "\r\n";
    response_header += "\r\n";

    // Create reponse
    std::string httpResponse = response_header + response_body;

    // Send file contents
    send(client_socket, httpResponse.c_str(), httpResponse.length(), 0);
}

void http::Response::setStatusCode(int status) { statusCode = status; }

void http::Response::redirect(const std::string& redirect_url) {
    std::string httpResponse = "";

    // Add header to define redirect
    httpResponse += "HTTP/1.1 " + statusCodes.at(301) + "\r\n";
    httpResponse += "Location: " + redirect_url + "\r\n";
    httpResponse += "Content-Type: text/html\r\n";
    httpResponse += "Content-Length: 0\r\n";
    httpResponse += "Connection: close\r\n";
    httpResponse += "\r\n";

    // send to client
    send(client_socket, httpResponse.c_str(), httpResponse.length(), 0);
}