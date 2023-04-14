#include "./Router.h"

#include "../Controller/Controller.h"
#include "../Logger/Logger.h"
#include "../Request/Request.h"
#include "../Response/Response.h"
#include "../Utils/Utils.h"

typedef void (*controller_type)(http::Request &request,
                                http::Response &response);

// The function will contain the routing logic for the application
void http::Router::route(http::Request &request, http::Response &response) {
    Logger::log("Method: " + request.getValue("method"));
    Logger::log("Path: " + request.getValue("path"));
    Logger::log("Host: " + request.getValue("Host"));
    Logger::log("");

    std::string method = request.getValue("method");
    std::string path = request.getValue("path");

    // Get the controller from the user
    void (*controller)(http::Request & request, http::Response & response) =
        getControllerFromPathTrie(method, path);

    // Running the controller
    if (controller) controller(request, response);
}

// Function to register a path with controller
void http::Router::registerPath(
    const std::string &method, const std::string &path,
    void (*controllerFunction)(http::Request &request,
                               http::Response &response)) {
    // Special path when no other path matches
    if (path == "*") {
        notFoundRoutes[method] = controllerFunction;
        return;
    }

    // Create a path starting with the method name
    std::string triePath = method + path;

    // Insert the path in the trie
    std::vector<std::string> tokens = Utils::split(triePath, "/");

    // remove the last empty ones if any
    while (tokens.size() > 0 && tokens.back() == "") tokens.pop_back();

    http::PathTrie *curr = root;
    for (auto token : tokens) {
        if (!token.empty() && token[0] == ':') {
            if (curr->pathParamChildrens.count(token) == 0) {
                curr->pathParamChildrens[token] = new http::PathTrie(token);
            }
            curr = curr->pathParamChildrens[token];
        } else {
            if (curr->normalChildrens.count(token) == 0) {
                curr->normalChildrens[token] = new http::PathTrie(token);
            }
            curr = curr->normalChildrens[token];
        }
    }

    curr->isEnd = true;
    curr->controllerFunction = controllerFunction;
}

void http::Router::displayAllRoutes() {
    Logger::log("\n----------------------- Paths -----------------------");
    displayAllRoutesCallback(root, "", 0, true);
    Logger::log("-----------------------------------------------------");
}

void http::Router::displayAllRoutesCallback(PathTrie *node, std::string path,
                                            int weight, bool isStart = false) {
    if (node->isEnd) {
        Logger::log(path + node->value, std::to_string(weight));
    }

    if (!isStart) path += (node->value + "/");

    // Call all the childrens
    for (auto p : node->normalChildrens) {
        displayAllRoutesCallback(p.second, path, weight + 1);
    }

    for (auto p : node->pathParamChildrens) {
        displayAllRoutesCallback(p.second, path, weight);
    }
}

controller_type http::Router::getControllerFromPathTrie(
    const std::string &method, const std::string &path) {
    std::string triePath = method + path;
    Logger::log(triePath);

    // TODO: Find the controller from the pathTrie, and return the controller
    // For now we are returning hard coded controllers
    if (method == "GET" && path == "/") {
        return Controller::getHome;
    } else {
        return Controller::getNotFound;
    }
}

// Trie Definition
http::PathTrie::PathTrie(const std::string &value) {
    this->value = value;
    this->isEnd = false;
    this->controllerFunction = nullptr;
}
