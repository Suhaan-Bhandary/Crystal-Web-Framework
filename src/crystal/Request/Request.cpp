#include "./Request.h"

#include <stdexcept>

#include "../Json/Json.h"
#include "../Logger/Logger.h"
#include "../Utils/Utils.h"

Crystal::Request::Request(char requestBuffer[]) {
    readRequest(requestBuffer);
    parseDataFromHeader();
    parseDataFromBody();

    // Function to parse cookie
    parseCookiesData();
}

Crystal::Request::~Request() {
    if (jsonData != nullptr) {
        delete jsonData;
    }
}

void Crystal::Request::readRequest(char requestBuffer[]) {
    std::vector<std::string> blocks =
        Crystal::Utils::split(std::string(requestBuffer), "\r\n\r\n");

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
            LOGGER_ERROR("Invalid\\Unknown Request from client");
            break;
    }
}

void Crystal::Request::parseDataFromHeader() {
    if (header.size() == 0) return;

    // Extract lines
    std::vector<std::string> lines = Crystal::Utils::split(header, "\r\n");

    // Read the first line
    std::vector<std::string> tokens = Crystal::Utils::split(lines[0], " ");

    // Handle the query if exits
    std::vector<std::string> pathAndSearchQuery =
        Crystal::Utils::split(tokens[1], "?");

    if (pathAndSearchQuery.size() == 2) {
        // Handle the search queries
        processSearchQuery(pathAndSearchQuery[1]);
    }

    valueMap["method"] = tokens[0];
    valueMap["path"] = pathAndSearchQuery[0];
    valueMap["http-version"] = tokens[2];

    for (int i = 1; i < lines.size(); i++) {
        std::vector<std::string> tokens = Crystal::Utils::split(lines[i], ": ");

        // Handling the tokens if overflow occurred
        if (tokens.size() != 2) {
            continue;
        }

        std::string key = tokens[0], value = tokens[1];
        valueMap[key] = value;
    }
}

void Crystal::Request::parseDataFromBody() {
    std::string contentType = getValue("Content-Type");

    // Check if contentType is valid
    if (contentType.size() == 0) return;

    // Check the type of content
    if (contentType == "application/json") {
        try {
            jsonData = new Json::Json(body.c_str());
        } catch (const std::exception &e) {
            jsonData = new Json::Json("");
        }
    }
}

void Crystal::Request::parseCookiesData() {
    std::string cookiesStream = getValue("Cookie");

    if (cookiesStream.size() == 0) return;

    // Parse the data and store it in cookies
    std::vector<std::string> tokens = Crystal::Utils::split(cookiesStream, ";");
    for (auto token : tokens) {
        std::vector<std::string> keyAndValue =
            Crystal::Utils::split(token, "=");
        if (keyAndValue.size() < 2) continue;

        // Inserting the cookies
        cookies.insert({keyAndValue[0], keyAndValue[1]});
    }
}

std::string Crystal::Request::getValue(std::string key) {
    if (valueMap.count(key) == 0) return "";
    return valueMap[key];
}

void Crystal::Request::processSearchQuery(const std::string &queryString) {
    std::vector<std::string> tokens = Crystal::Utils::split(queryString, "&");

    for (int i = 0; i < tokens.size(); i++) {
        std::vector<std::string> keyAndValue =
            Crystal::Utils::split(tokens[i], "=");
        if (keyAndValue.size() < 2) continue;

        searchQueries.insert(
            {Utils::cleanEncodedString(keyAndValue[0]),
             Crystal::Utils::cleanEncodedString(keyAndValue[1])});
    }
}

// Getter for getting json data from body
Crystal::Json::Json &Crystal::Request::getJsonData() {
    if (jsonData == nullptr) {
        throw std::invalid_argument("Body Json Data not available");
    }
    return *jsonData;
}
