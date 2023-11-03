#include "mainController.h"

#include "../db/services/visitors.h"

void Controller::getHome(http::Request &request, http::Response &response) {
    // Add visitor time in database
    database::services::addVisitor();

    // Get the visitor count
    int visitorCount = database::services::getVisitorsCount();

    response.setStatusCode(200);

    // Set the cookies
    if (request.cookies.count("count") != 0) {
        int count = stoi(request.cookies["count"]);

        // Reset cookie if count is >= 10
        if (count >= 10) {
            response.clearCookie("count");
        } else {
            response.setCookie("count", std::to_string(count + 1), 10 * 60);
        }
    } else {
        response.setCookie("count", "1", 10 * 60);
    }

    Json::Json jsonObject("{}");
    jsonObject.data->insertInObject("visitorCount",
                                    std::to_string(visitorCount));
    response.sendTemplate("home.html", jsonObject);
}

void Controller::getUser(http::Request &request, http::Response &response) {
    std::string userId = request.pathParams[":id"];
    std::string search = request.searchQueries["search"];

    Json::Json jsonObject("{}");
    jsonObject.data->insertInObject("id", userId);
    jsonObject.data->insertInObject("search", search);

    response.setStatusCode(200);
    response.sendTemplate("getUser.html", jsonObject);
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

void Controller::getHealth(http::Request &request, http::Response &response) {
    // Send html response to the client
    std::string htmlContent =
        "<html>"
        "<head><title>Server Health</title></head>"
        "<body>"
        "<h1>Server is Healthy</h1>"
        "</body>"
        "</html>\r\n";

    response.setStatusCode(200);
    response.sendHTML(htmlContent);
}
