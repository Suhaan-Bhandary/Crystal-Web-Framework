#include "Controller.h"

#include <iostream>

void Controller::getHome(http::Request &request, http::Response &response) {
    response.setStatusCode(200);

    Json::Json jsonObject("{}");
    response.sendTemplate("home.html", jsonObject);
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
    Json::Json jsonObject("{}");

    response.setStatusCode(404);
    response.sendTemplate("404NotFound.html", jsonObject);
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

void Controller::saveUserData(http::Request &request,
                              http::Response &response) {
    try {
        // Load data from json
        std::string name =
            request.getJsonData()->getObjectValue("name")->getStringValue();

        std::string email =
            request.getJsonData()->getObjectValue("email")->getStringValue();

        Json::Json responseData;
        responseData.data->setAsObject();
        responseData.data->insertInObject("message", "Successful");
        responseData.data->insertInObject("name", name);
        responseData.data->insertInObject("email", email);
        responseData.data->insertEmptyObject("colors");
        responseData.data->getObjectValue("colors")->insertInObject("red",
                                                                    "redo");

        response.setStatusCode(200);
        response.sendJson(responseData.getJsonString());
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        response.setStatusCode(500);
        response.sendHTML("<h1>Error</h1>");
    }
}

void Controller::redirectUserToGoogle(http::Request &request,
                                      http::Response &response) {
    std::string url = request.searchQueries["url"];
    if (url.size() == 0) url = "/";

    response.redirect(url);
}

void Controller::getXOGame(http::Request &request, http::Response &response) {
    Json::Json jsonObject("{}");

    response.setStatusCode(200);
    response.sendTemplate("xo-game.html", jsonObject);
}
