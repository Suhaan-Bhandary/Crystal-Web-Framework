// The ifndef is used so that the same File is not loaded again
#ifndef INCLUDED_ROUTER
#define INCLUDED_ROUTER

#include <string>
#include <unordered_map>
#include <vector>

#include "../Controller/systemController.h"
#include "../Request/Request.h"
#include "../Response/Response.h"

typedef void (*controller_type)(Crystal::Request &request,
                                Crystal::Response &response);

namespace Crystal {
struct PathTrie {
   public:
    std::string value;
    bool isEnd;
    int weight;

    // controller function stores the controller for the specified path
    controller_type controllerFunction;

    // gives the path component and next
    std::unordered_map<std::string, PathTrie *> normalChildrens;

    // gives the param name and next
    std::unordered_map<std::string, PathTrie *> pathParamChildrens;

    PathTrie(const std::string &value, int weight);
};

class Router {
   private:
    PathTrie *root;
    std::unordered_map<std::string, controller_type> notFoundRoutes;

   public:
    Router();
    void route(Crystal::Request &request, Crystal::Response &response);

    void registerPath(const std::string &method, const std::string &path,
                      controller_type controllerFunction);
    void displayAllRoutes();
    void displayAllRoutesCallback(PathTrie *node, std::string path,
                                  bool isStart);

    controller_type getControllerFromPathTrie(
        const std::string &method, const std::string &path,
        std::unordered_map<std::string, std::string> &pathParams);

    void getControllerFromPathTrieCallback(
        int idx, const std::vector<std::string> &tokens, PathTrie *node,
        controller_type &controllerFunction, int &minWeight,
        std::unordered_map<std::string, std::string> &pathParams,
        std::unordered_map<std::string, std::string> tempParams);

    controller_type getNotFoundRoute(const std::string &method);

    void registerPublicPath();

    // Methods to register paths in RestApi
    void get(const std::string &path, controller_type controllerFunction);
    void post(const std::string &path, controller_type controllerFunction);
    void put(const std::string &path, controller_type controllerFunction);
    void patch(const std::string &path, controller_type controllerFunction);
    void delete_(const std::string &path, controller_type controllerFunction);
    void head(const std::string &path, controller_type controllerFunction);
    void options(const std::string &path, controller_type controllerFunction);
};
}  // namespace Crystal

#endif
