// The ifndef is used so that the same File is not loaded again
#ifndef INCLUDED_REQUEST
#define INCLUDED_REQUEST

#include <string>
#include <unordered_map>

#include "../Json/Json.h"

namespace http {
class Request {
    std::unordered_map<std::string, std::string> valueMap;

    // This stores the json data if we have json in body
    Json::Json *jsonData = nullptr;

   public:
    std::string header, body;

    std::unordered_map<std::string, std::string> pathParams;
    std::unordered_map<std::string, std::string> searchQueries;

    Request(char requestBuffer[]);
    ~Request();

    void readRequest(char requestBuffer[]);
    void parseDataFromHeader();
    void parseDataFromBody();
    std::string getValue(std::string key);
    void processSearchQuery(const std::string &queryString);

    Json::JsonNode *getJsonData();
};
}  // namespace http

#endif