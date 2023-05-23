#include "./Request.h"

#include "../Json/Json.h"
#include "../Logger/Logger.h"
#include "../Utils/Utils.h"

http::Request::Request(char requestBuffer[]) {
    readRequest(requestBuffer);
    parseDataFromHeader();
    parseDataFromBody();
}

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

void http::Request::parseDataFromHeader() {
    if (header.size() == 0) return;

    // Extract lines
    std::vector<std::string> lines = Utils::split(header, "\r\n");

    // Read the first line
    std::vector<std::string> tokens = Utils::split(lines[0], " ");

    // Handle the query if exits
    std::vector<std::string> pathAndSearchQuery = Utils::split(tokens[1], "?");

    if (pathAndSearchQuery.size() == 2) {
        // Handle the search queries
        processSearchQuery(pathAndSearchQuery[1]);
    }

    valueMap["method"] = tokens[0];
    valueMap["path"] = pathAndSearchQuery[0];
    valueMap["http-version"] = tokens[2];

    for (int i = 1; i < lines.size(); i++) {
        std::vector<std::string> tokens = Utils::split(lines[i], ": ");
        std::string key = tokens[0], value = tokens[1];
        valueMap[key] = value;
    }
}

void http::Request::parseDataFromBody() {
    std::string contentType = getValue("Content-Type");

    // Check if contentType is valid
    if (contentType.size() == 0) return;

    // Check the type of content
    if (contentType == "application/json") {
        try {
            jsonData = new Json::Json(body);
        } catch (const std::exception &e) {
            jsonData = new Json::Json("");
        }
    }
}

std::string http::Request::getValue(std::string key) {
    if (valueMap.count(key) == 0) return "";
    return valueMap[key];
}

void http::Request::processSearchQuery(const std::string &queryString) {
    std::vector<std::string> tokens = Utils::split(queryString, "&");

    for (int i = 0; i < tokens.size(); i++) {
        std::vector<std::string> keyAndValue = Utils::split(tokens[i], "=");
        if (keyAndValue.size() < 2) continue;

        searchQueries.insert({keyAndValue[0], keyAndValue[1]});
    }
}
