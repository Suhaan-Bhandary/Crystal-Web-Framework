// The ifndef is used so that the same File is not loaded again
#ifndef INCLUDED_JSON
#define INCLUDED_JSON

#include <string>
#include <unordered_map>
#include <vector>

namespace Json {

enum JsonType {
    NULL_VALUE = 0,
    INT = 1,
    DOUBLE = 2,
    BOOL = 3,
    STRING = 4,
    ARRAY = 5,
    OBJECT = 6
};

struct JsonNode {
    /*
        Type:
        0: Null value
        1: intValue
        2: doubleValue
        3: boolValue
        4: stringValue
        5: arrayValue
        6: objectValue
    */

    JsonType type;

    long long intValue;
    double doubleValue;
    bool boolValue;
    std::string stringValue;

    std::vector<JsonNode *> arrayValue;
    std::unordered_map<std::string, JsonNode *> objectValue;

    JsonNode();
};

class Json {
   public:
    JsonNode *data;
    Json(const std::string &body);

    // Function to parse the string and convert to json object
    JsonType getJsonTokenType(const std::string &jsonToken);
    void parseJsonFromString(const std::string &jsonString);
    JsonNode *parseJsonFromStringCallback(const std::string &jsonString);

    // Json utility functions
    std::vector<std::string> getElementsOfJsonArrayString(
        const std::string &jsonString);

    std::vector<std::pair<std::string, std::string>>
    getKeyAndValuePairsOfJsonArrayObject(const std::string &jsonString);

    // TODO: Create a Destructure
};
}  // namespace Json

#endif