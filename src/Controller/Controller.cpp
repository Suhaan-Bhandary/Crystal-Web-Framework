#include "Controller.h"

#include <iostream>

void Controller::getHome(http::Request &request, http::Response &response) {
    // Send html response to the client
    std::string htmlContent =
        "<html>"
        "<head><title>Home Page</title></head>"
        "<body>"
        "<h1>Home Page!</h1>"
        "<p>HTTP Server made with C++</p>"
        "</body>"
        "</html>\r\n";

    response.sendHTML(htmlContent);
}

void Controller::getUser(http::Request &request, http::Response &response) {
    std::string userId = request.pathParams[":id"];

    // Send html response to the client
    std::string htmlContent =
        "<html>"
        "<head><title>User</title></head>"
        "<body>"
        "<h1>User Profile Page</h1>"
        "<h2>Welcome, " +
        userId +
        " !!</h2>"
        "<p>Made with C++</p>"
        "</body>"
        "</html>\r\n";

    response.sendHTML(htmlContent);
}

void Controller::getUserChat(http::Request &request, http::Response &response) {
    // Send html response to the client
    std::string htmlContent =
        "<html>"
        "<head><title>User Chat</title></head>"
        "<body>"
        "<h1>User Chat!</h1>"
        "<p>Welcome to user chat C++</p>"
        "</body>"
        "</html>\r\n";

    response.sendHTML(htmlContent);
}

void Controller::getNotFound(http::Request &request, http::Response &response) {
    std::string homePageLink = "/";

    // Send html response to the client
    std::string htmlContent =
        "<html>"
        "<head><title>Page Not Found</title></head>"
        "<body>"
        "<h1>404 Error: Page not Found!!</h1>"
        "<a href=\"" +
        homePageLink +
        "\">Home Page</a>"
        "</body>"
        "</html>\r\n";

    response.sendHTML(htmlContent);
}

void Controller::defaultNotFound(http::Request &request,
                                 http::Response &response) {
    response.send404();
}
