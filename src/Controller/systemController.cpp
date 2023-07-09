#include "./systemController.h"

void Controller::defaultNotFound(http::Request &request,
                                 http::Response &response) {
    response.send404();
}

void Controller::getPublicFile(http::Request &request,
                               http::Response &response) {
    std::string path = request.getValue("path");
    response.sendPublicFile(path);
}
