// The ifndef is used so that the same File is not loaded again
#ifndef INCLUDED_REQUEST
#define INCLUDED_REQUEST

#include <string>

#include "../Json/Json.h"

namespace Crystal {
class Request {
    std::unordered_map<std::string, std::string> valueMap;

    // This stores the json data if we have json in body
    Json::Json *jsonData = nullptr;

   public:
    std::string header, body;

    std::unordered_map<std::string, std::string> pathParams;
    std::unordered_map<std::string, std::string> searchQueries;

    // Cookies are key, value pair with both as string,
    // note cookies can be publicly accessed
    std::unordered_map<std::string, std::string> cookies;

    Request(char requestBuffer[]);
    ~Request();

    void readRequest(char requestBuffer[]);
    void parseDataFromHeader();
    void parseDataFromBody();

    // Function to parse data from valueMap to cookies
    void parseCookiesData();

    std::string getValue(std::string key);
    void processSearchQuery(const std::string &queryString);

    Json::Json &getJsonData();
};
}  // namespace Crystal

#endif