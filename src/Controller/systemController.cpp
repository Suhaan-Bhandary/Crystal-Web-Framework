#include "./systemController.h"

#include "../Logger/Logger.h"
#include "../Utils/Utils.h"

void Controller::defaultNotFound(http::Request &request,
                                 http::Response &response) {
    response.send404();
}

void Controller::getPublicFile(http::Request &request,
                               http::Response &response) {
    // Check If-None-Match from request
    std::string ifNoneMatch = request.getValue("If-None-Match");

    std::string path = request.getValue("path");
    std::string absoluteFilePath =
        Utils::getCurrentDirectory() + "/app/public" + path;
    std::string fileETag = Utils::getFileETag(absoluteFilePath);

    if (ifNoneMatch == fileETag) {
        response.setStatusCode(304);
        return response.sendJson("");
    }

    // Set the file ETag
    response.setETag(fileETag);
    response.sendPublicFile(path);
}
