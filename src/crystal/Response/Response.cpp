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
#include "../Config/Config.h"

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
                                  Json::Json& data) {
    // Read the file
    std::string rootDir = Utils::getCurrentDirectory();
    std::string templateURL = rootDir + http::Config::TEMPLATE_DIR_PATH + templateName;

    // Create template object
    http::HTMLTemplate html_template(templateURL, data);

    std::string HTMLContent = html_template.getHtml();

    // Send Response
    sendResponse(HTMLContent, "text/html");
}

void http::Response::sendPublicFile(const std::string& relativePathToPublic) {
    std::string absoluteFilePath =
        Utils::getCurrentDirectory() + http::Config::PUBLIC_DIR_PATH + relativePathToPublic;

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
    std::ifstream file(absoluteFilePath, std::ios::binary);

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
    response_header += getCookiesHeader();
    response_header += getETagHeader();
    response_header += "\r\n";

    // LOGGER_WARNING(response_header);

    // Create response
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

// Cookie function
void http::Response::setCookie(const std::string& name,
                               const std::string& value,
                               const std::string& expires,
                               const std::string& domain, bool isHttpOnly,
                               bool isSecure, bool isPartitioned,
                               const std::string& path,
                               const std::string& sameSite) {
    // Creating a cookie
    Cookie cookie(name, value, expires, 0, domain, isHttpOnly, isSecure,
                  isPartitioned, path, sameSite);

    // Setting the value of the cookie
    if (cookieData.count(name) != 0) {
        cookieData.erase(name);
    }

    cookieData.insert({name, cookie});
}

void http::Response::setCookie(const std::string& name,
                               const std::string& value, int maxAgeInSeconds,
                               const std::string& domain, bool isHttpOnly,
                               bool isSecure, bool isPartitioned,
                               const std::string& path,
                               const std::string& sameSite) {
    Cookie cookie(name, value, "", maxAgeInSeconds, domain, isHttpOnly,
                  isSecure, isPartitioned, path, sameSite);

    // Setting the value of the cookie
    if (cookieData.count(name) != 0) {
        cookieData.erase(name);
    }

    cookieData.insert({name, cookie});
}

void http::Response::clearCookie(const std::string& name) {
    setCookie(name, "", 0);
}

std::string http::Response::getCookiesHeader() {
    std::string cookieHeader = "";

    for (auto p : cookieData) {
        cookieHeader += p.second.getHeader();
    }

    return cookieHeader;
}

// Cookie
http::Cookie::Cookie(const std::string& name, const std::string& value,
                     const std::string& expires, int maxAge,
                     const std::string& domain, bool isHttpOnly, bool isSecure,
                     bool isPartitioned, const std::string& path,
                     const std::string& sameSite)
    : name(name),
      value(value),
      expires(expires),
      maxAge(maxAge),
      domain(domain),
      isHttpOnly(isHttpOnly),
      isSecure(isSecure),
      isPartitioned(isPartitioned),
      path(path),
      sameSite(sameSite) {}

std::string http::Cookie::getHeader() {
    std::string header = "Set-Cookie: ";

    std::string separator = "; ";

    // Name and value
    header += (name + "=" + value + separator);

    // Expires
    if (expires.size() != 0) {
        header += ("Expires=" + expires + separator);
    }

    // maxAge
    if (maxAge != 0 || expires.size() == 0) {
        header += ("Max-Age=" + std::to_string(maxAge) + separator);
    }

    // domain
    if (domain.size() != 0) {
        header += ("Domain=" + domain + separator);
    }

    // http only
    if (isHttpOnly) {
        header += ("HttpOnly" + separator);
    }

    // secure
    if (isSecure) {
        header += ("Secure" + separator);
    }

    // partitioned
    if (isPartitioned) {
        header += ("Partitioned" + separator);
    }

    if (path.size() != 0) {
        header += ("Path=" + path + separator);
    }

    if (sameSite.size() != 0) {
        header += ("SameSite=" + sameSite + separator);
    }

    // Check if "; " is present at last
    if (header.back() == ' ') {
        header.pop_back();
        if (header.back() == ';') header.pop_back();
    }

    // Add the new line to the header
    header += "\r\n";

    return header;
}

// Etag
std::string http::Response::getETagHeader() {
    if (eTag == "") return "";
    return "ETag: " + eTag + "\r\n";
}

void http::Response::setETag(std::string eTagValue) { eTag = eTagValue; }