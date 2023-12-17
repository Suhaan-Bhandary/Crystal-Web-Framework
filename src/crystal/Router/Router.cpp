#include "./Router.h"

#include "../Config/Config.h"
#include "../Logger/Logger.h"
#include "../Request/Request.h"
#include "../Response/Response.h"
#include "../Utils/Utils.h"

typedef void (*controller_type)(Crystal::Request &request,
                                Crystal::Response &response);

// Constructor
Crystal::Router::Router() {
    // Initialize the root
    root = new Crystal::PathTrie("ROOT", 0);

    // Register public files
    registerPublicPath();
}

// The function will contain the routing logic for the application
void Crystal::Router::route(Crystal::Request &request,
                            Crystal::Response &response) {
    // LOGGER("Request:", request.getValue("method"), request.getValue("path"));

    std::string method = request.getValue("method");
    std::string path = request.getValue("path");

    // Get the controller from the user
    std::unordered_map<std::string, std::string> pathParams;

    controller_type controller =
        getControllerFromPathTrie(method, path, pathParams);

    // Add path params to request
    request.pathParams = pathParams;

    // Running the controller
    if (controller) controller(request, response);
}

// Function to register a path with controller
void Crystal::Router::registerPath(const std::string &method,
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
    std::vector<std::string> tokens = Crystal::Utils::split(triePath, "/");

    // remove the last empty ones if any
    while (tokens.size() > 0 && tokens.back() == "") tokens.pop_back();

    Crystal::PathTrie *curr = root;
    int weight = 0;
    for (auto token : tokens) {
        if (!token.empty() && token[0] == ':') {
            if (curr->pathParamChildrens.count(token) == 0) {
                curr->pathParamChildrens[token] =
                    new Crystal::PathTrie(token, weight);
            }
            curr = curr->pathParamChildrens[token];
        } else {
            // weight only increases when normal node is present in path
            weight++;

            if (curr->normalChildrens.count(token) == 0) {
                curr->normalChildrens[token] =
                    new Crystal::PathTrie(token, weight);
            }
            curr = curr->normalChildrens[token];
        }
    }

    curr->isEnd = true;
    curr->controllerFunction = controllerFunction;
}

void Crystal::Router::get(const std::string &path,
                          controller_type controllerFunction) {
    registerPath("GET", path, controllerFunction);
}

void Crystal::Router::post(const std::string &path,
                           controller_type controllerFunction) {
    registerPath("POST", path, controllerFunction);
}

void Crystal::Router::put(const std::string &path,
                          controller_type controllerFunction) {
    registerPath("PUT", path, controllerFunction);
}

void Crystal::Router::patch(const std::string &path,
                            controller_type controllerFunction) {
    registerPath("PATCH", path, controllerFunction);
}

void Crystal::Router::delete_(const std::string &path,
                              controller_type controllerFunction) {
    registerPath("DELETE", path, controllerFunction);
}

void Crystal::Router::head(const std::string &path,
                           controller_type controllerFunction) {
    registerPath("HEAD", path, controllerFunction);
}

void Crystal::Router::options(const std::string &path,
                              controller_type controllerFunction) {
    registerPath("OPTIONS", path, controllerFunction);
}

void Crystal::Router::displayAllRoutes() {
    LOGGER_MINIMAL("\n----------------------- Paths -----------------------");
    displayAllRoutesCallback(root, "", true);
    LOGGER_MINIMAL("-----------------------------------------------------");
}

void Crystal::Router::displayAllRoutesCallback(PathTrie *node, std::string path,
                                               bool isStart = false) {
    if (node->isEnd) {
        LOGGER_MINIMAL(path + node->value + " " + std::to_string(node->weight));
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

controller_type Crystal::Router::getControllerFromPathTrie(
    const std::string &method, const std::string &path,
    std::unordered_map<std::string, std::string> &pathParams) {
    std::string triePath = method + path;

    // get tokens for the trie path
    std::vector<std::string> tokens = Crystal::Utils::split(triePath, "/");

    // Clean tokens
    for (int i = 0; i < tokens.size(); i++) {
        tokens[i] = Crystal::Utils::cleanEncodedString(tokens[i]);
    }

    // remove the last empty ones if any
    while (tokens.size() > 0 && tokens.back() == "") tokens.pop_back();

    // Using recursive function call the
    controller_type controllerFunction = nullptr;
    int minWeight = -1;
    getControllerFromPathTrieCallback(
        0, tokens, root, controllerFunction, minWeight, pathParams,
        std::unordered_map<std::string, std::string>());

    // For now we are returning hard coded controllers
    if (controllerFunction != nullptr) return controllerFunction;
    return getNotFoundRoute(method);
}

controller_type Crystal::Router::getNotFoundRoute(const std::string &method) {
    if (notFoundRoutes.count(method) == 0)
        return Crystal::Controller::defaultNotFound;
    return notFoundRoutes[method];
}

void Crystal::Router::getControllerFromPathTrieCallback(
    int idx, const std::vector<std::string> &tokens, PathTrie *node,
    controller_type &controllerFunction, int &minWeight,
    std::unordered_map<std::string, std::string> &pathParams,
    std::unordered_map<std::string, std::string> tempParams) {
    if (idx == tokens.size()) {
        if (controllerFunction == nullptr || node->weight > minWeight) {
            pathParams = tempParams;
            minWeight = node->weight;
            controllerFunction = node->controllerFunction;
        }
        return;
    }

    // Current token
    std::string currentToken = tokens[idx];

    // Go to the matching child node
    if (node->normalChildrens.count(currentToken)) {
        getControllerFromPathTrieCallback(
            idx + 1, tokens, node->normalChildrens[currentToken],
            controllerFunction, minWeight, pathParams, tempParams);
    }

    // Go to all available path params
    for (auto p : node->pathParamChildrens) {
        std::unordered_map<std::string, std::string> currParams = tempParams;
        currParams[p.first] = currentToken;

        getControllerFromPathTrieCallback(idx + 1, tokens, p.second,
                                          controllerFunction, minWeight,
                                          pathParams, currParams);
    }
}

void Crystal::Router::registerPublicPath() {
    LOGGER_MINIMAL("\nRegistering Public Paths");
    std::vector<std::string> publicFilesPath;
    std::string path =
        Crystal::Utils::getCurrentDirectory() + Crystal::Config::PUBLIC_DIR_PATH;
    Crystal::Utils::listFiles(path, publicFilesPath);

    // register each file
    for (auto filePath : publicFilesPath) {
        std::vector<std::string> tokens = Crystal::Utils::split(filePath, path);
        if (tokens.size() < 2) continue;

        std::string relativePath = "/" + tokens[1];
        LOGGER_MINIMAL(relativePath);

        registerPath("GET", relativePath, Crystal::Controller::getPublicFile);
    }
}

// Trie Definition
Crystal::PathTrie::PathTrie(const std::string &value, int weight) {
    this->value = value;
    this->isEnd = false;
    this->weight = weight;
    this->controllerFunction = nullptr;
}
