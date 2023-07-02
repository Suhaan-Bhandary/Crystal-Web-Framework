// The ifndef is used so that the same File is not loaded again
#ifndef INCLUDED_JSON
#define INCLUDED_JSON

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "./JsonNode.h"

namespace Json {
class Json {
   public:
    JsonNode *data;
    Json();
    Json(const std::string &body);
    Json(Json &object);
    ~Json();

    // Function to parse the string and convert to json object
    JsonType getJsonTokenType(const std::string &jsonToken);
    void parseJsonFromString(const std::string &jsonString);
    JsonNode *parseJsonFromStringCallback(const std::string &jsonString);

    // Function to get Json String
    std::string getJsonString();

    // Json utility functions
    std::vector<std::string> getElementsOfJsonArrayString(
        const std::string &jsonString);

    std::vector<std::pair<std::string, std::string>>
    getKeyAndValuePairsOfJsonArrayObject(const std::string &jsonString);
};
}  // namespace Json

#endif