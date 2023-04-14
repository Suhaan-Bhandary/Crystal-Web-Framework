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
    controller_type controller = getControllerFromPathTrie(method, path);

    // Running the controller
    if (controller) controller(request, response);
}

// Function to register a path with controller
void http::Router::registerPath(const std::string &method,
                                const std::string &path,
                                controller_type controllerFunction) {
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
    int weight = 0;
    for (auto token : tokens) {
        if (!token.empty() && token[0] == ':') {
            if (curr->pathParamChildrens.count(token) == 0) {
                curr->pathParamChildrens[token] =
                    new http::PathTrie(token, weight);
            }
            curr = curr->pathParamChildrens[token];
        } else {
            // weight only increases when normal node is present in path
            weight++;

            if (curr->normalChildrens.count(token) == 0) {
                curr->normalChildrens[token] =
                    new http::PathTrie(token, weight);
            }
            curr = curr->normalChildrens[token];
        }
    }

    curr->isEnd = true;
    curr->controllerFunction = controllerFunction;
}

void http::Router::displayAllRoutes() {
    Logger::log("\n----------------------- Paths -----------------------");
    displayAllRoutesCallback(root, "", true);
    Logger::log("-----------------------------------------------------");
}

void http::Router::displayAllRoutesCallback(PathTrie *node, std::string path,
                                            bool isStart = false) {
    if (node->isEnd) {
        Logger::log(path + node->value, std::to_string(node->weight));
    }

    if (!isStart) path += (node->value + "/");

    // Call all the childrens
    for (auto p : node->normalChildrens) {
        displayAllRoutesCallback(p.second, path);
    }

    for (auto p : node->pathParamChildrens) {
        displayAllRoutesCallback(p.second, path);
    }
}

controller_type http::Router::getControllerFromPathTrie(
    const std::string &method, const std::string &path) {
    std::string triePath = method + path;
    Logger::log(triePath);

    // get tokens for the trie path
    std::vector<std::string> tokens = Utils::split(triePath, "/");

    // remove the last empty ones if any
    while (tokens.size() > 0 && tokens.back() == "") tokens.pop_back();

    // Using recursive function call the
    controller_type controllerFunction = nullptr;
    int minWeight = -1;
    getControllerFromPathTrieCallback(0, tokens, root, controllerFunction,
                                      minWeight);

    // For now we are returning hard coded controllers
    if (controllerFunction != nullptr) return controllerFunction;
    return getNotFoundRoute(method);
}

controller_type http::Router::getNotFoundRoute(const std::string &method) {
    if (notFoundRoutes.count(method) == 0) return Controller::defaultNotFound;
    return notFoundRoutes[method];
}

void http::Router::getControllerFromPathTrieCallback(
    int idx, const std::vector<std::string> &tokens, PathTrie *node,
    controller_type &controllerFunction, int &minWeight) {
    if (idx == tokens.size()) {
        Logger::log("-----------------");
        Logger::log(node->value);

        if (controllerFunction == nullptr || node->weight > minWeight) {
            minWeight = node->weight;
            controllerFunction = node->controllerFunction;
        }
        return;
    }

    // Current token
    std::string currentToken = tokens[idx];

    // Go to the matching child node
    if (node->normalChildrens.count(currentToken)) {
        getControllerFromPathTrieCallback(idx + 1, tokens,
                                          node->normalChildrens[currentToken],
                                          controllerFunction, minWeight);
    }

    // Go to all available path params
    for (auto p : node->pathParamChildrens) {
        getControllerFromPathTrieCallback(idx + 1, tokens, p.second,
                                          controllerFunction, minWeight);
    }
}

// Trie Definition
http::PathTrie::PathTrie(const std::string &value, int weight) {
    this->value = value;
    this->isEnd = false;
    this->weight = weight;
    this->controllerFunction = nullptr;
}
