#include "Controller.h"

#include <iostream>

void Controller::getHome(http::Request &request, http::Response &response) {
    response.setStatusCode(200);
    response.sendTemplate("home.html");
}

void Controller::getUser(http::Request &request, http::Response &response) {
    std::string userId = request.pathParams[":id"];
    std::string rollno = request.searchQueries["rollno"];

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
        "Your Rollno is: " +
        rollno +
        "</body>"
        "</html>\r\n";

    response.setStatusCode(200);
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

    response.setStatusCode(200);
    response.sendHTML(htmlContent);
}

void Controller::getNotFound(http::Request &request, http::Response &response) {
    response.setStatusCode(404);
    response.sendTemplate("404NotFound.html");
}

void Controller::defaultNotFound(http::Request &request,
                                 http::Response &response) {
    response.send404();
}

void Controller::getPublicFile(http::Request &request,
                               http::Response &response) {
    std::string path = request.getValue("path");
    response.sendPublicFile(path);
}
