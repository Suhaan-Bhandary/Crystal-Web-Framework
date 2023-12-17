#include "./systemController.h"

#include "../Config/Config.h"
#include "../Logger/Logger.h"
#include "../Utils/Utils.h"

void Crystal::Controller::defaultNotFound(Crystal::Request &request,
                                          Crystal::Response &response) {
    response.send404();
}

void Crystal::Controller::getPublicFile(Crystal::Request &request,
                                        Crystal::Response &response) {
    // Check If-None-Match from request
    std::string ifNoneMatch = request.getValue("If-None-Match");

    std::string path = request.getValue("path");
    std::string absoluteFilePath =
        Crystal::Utils::getCurrentDirectory() + Crystal::Config::PUBLIC_DIR_PATH + path;
    std::string fileETag = "\"" + Crystal::Utils::getFileETag(absoluteFilePath) + "\"";

    if (ifNoneMatch == fileETag) {
        response.setStatusCode(304);
        return response.sendJson("");
    }

    // Set the file ETag
    response.setETag(fileETag);
    response.sendPublicFile(path);
}
