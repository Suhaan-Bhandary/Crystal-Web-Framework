#include "mainController.h"

#include "../db/services/visitors.h"
#include "crystal.h"

void Controller::getHome(Crystal::Request &request,
                         Crystal::Response &response) {
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

    Crystal::Json::Json jsonObject("{}");
    jsonObject.getData().set("visitorCount", std::to_string(visitorCount));
    response.sendTemplate("home.html", jsonObject);
}

void Controller::getUser(Crystal::Request &request,
                         Crystal::Response &response) {
    std::string userId = request.pathParams[":id"];
    std::string search = request.searchQueries["search"];

    Crystal::Json::Json jsonObject("{}");
    jsonObject.getData().set("id", userId);
    jsonObject.getData().set("search", search);

    response.setStatusCode(200);
    response.sendTemplate("getUser.html", jsonObject);
}

void Controller::getUserChat(Crystal::Request &request,
                             Crystal::Response &response) {
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

void Controller::getNotFound(Crystal::Request &request,
                             Crystal::Response &response) {
    Crystal::Json::Json jsonObject("{}");

    response.setStatusCode(404);
    response.sendTemplate("404NotFound.html", jsonObject);
}

void Controller::saveUserData(Crystal::Request &request,
                              Crystal::Response &response) {
    try {
        auto &bodyNode = request.getJsonData().getData();

        Crystal::Json::Json responseData("{}");
        auto &node = responseData.getData();

        if (!bodyNode.contains("name") || !bodyNode.contains("email")) {
            bodyNode.set("message", "Please provide name and email");
            return response.sendJson(bodyNode.toString());
        }

        // Load data from json
        std::string name = bodyNode.get("name").get<Crystal::Json::String>();
        std::string email = bodyNode.get("email").get<Crystal::Json::String>();

        node.set("message", "Successful");
        node.set("name", name);
        node.set("email", email);

        // Creating a object of colors pairs
        node.set("colors", Crystal::Json::Object());
        node.get("colors").set("red", "blue");

        // Numbers
        node.set("languages", Crystal::Json::Array());
        node.get("languages").push("C++");
        node.get("languages").push("Java");
        node.get("languages").push("Python");

        response.setStatusCode(200);
        response.sendJson(node.toString());
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        response.setStatusCode(500);
        response.sendHTML("<h1>Error</h1>");
    }
}

void Controller::redirectUserToGoogle(Crystal::Request &request,
                                      Crystal::Response &response) {
    std::string url = request.searchQueries["url"];
    if (url.size() == 0) url = "/";

    response.redirect(url);
}

void Controller::getXOGame(Crystal::Request &request,
                           Crystal::Response &response) {
    Crystal::Json::Json jsonObject("{}");

    response.setStatusCode(200);
    response.sendTemplate("xo-game.html", jsonObject);
}

void Controller::getHealth(Crystal::Request &request,
                           Crystal::Response &response) {
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
