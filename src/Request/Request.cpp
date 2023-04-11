#include "./Request.h"

#include <iostream>
#include <vector>

#include "../Logger/Logger.h"
#include "../Utils/Utils.h"

http::Request::Request(char requestBuffer[]) { readRequest(requestBuffer); }

void http::Request::readRequest(char requestBuffer[]) {
    std::vector<std::string> blocks =
        Utils::split(std::string(requestBuffer), "\r\n\r\n");

    int blockCount = blocks.size();

    switch (blockCount) {
        case 1:
            header = blocks[0];
            body = "";
            break;
        case 2:
            header = blocks[0];
            body = blocks[1];
            break;
        default:
            Logger::log("Invalid\\Unknown Request from client");
            break;
    }
}