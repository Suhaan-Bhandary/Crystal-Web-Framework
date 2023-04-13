// The ifndef is used so that the same File is not loaded again
#ifndef INCLUDED_ROUTER
#define INCLUDED_ROUTER

#include <string>
#include <unordered_map>

#include "../Request/Request.h"
#include "../Response/Response.h"

namespace http {
struct PathTrie {
   public:
    std::string value;
    bool isEnd;

    // controller function stores the controller for the specified path
    void (*controllerFunction)(http::Request &request,
                               http::Response &response);

    // gives the path component and next
    std::unordered_map<std::string, PathTrie *> normalChildrens;

    // gives the param name and next
    std::unordered_map<std::string, PathTrie *> pathParamChildrens;

    PathTrie(const std::string &value);
};

class Router {
   private:
    PathTrie *root;
    std::unordered_map<std::string, void (*)(http::Request &request,
                                             http::Response &response)>
        notFoundRoutes;

   public:
    Router();
    void route(http::Request &request, http::Response &response);

    void registerPath(const std::string &method, const std::string &path,
                      void (*controllerFunction)(http::Request &request,
                                                 http::Response &response));
    void displayAllRoutes();
    void displayAllRoutesCallback(PathTrie *node, std::string path, int weight,
                                  bool isStart);

    void matchPathAndRunController(const std::string &method,
                                   const std::string &path);
};
}  // namespace http

#endif