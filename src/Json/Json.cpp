#include "Json.h"

#include <iostream>
#include <stack>
#include <stdexcept>

#include "../Logger/Logger.h"
#include "../Utils/Utils.h"

// Json
Json::Json::Json() { data = new JsonNode(); }

Json::Json::Json(const std::string &body) {
    data = nullptr;
    parseJsonFromString(body);
}

Json::Json::Json(Json &object) {
    data = nullptr;
    parseJsonFromString(object.getJsonString());
}

Json::Json::~Json() {
    if (data != nullptr) {
        delete data;
    }
}

Json::JsonType Json::Json::getJsonTokenType(const std::string &jsonToken) {
    // Note:
    // long long / double: Starts with number
    // Array: Starts with [
    // Bool: string is true or false
    // String: Starts with "
    // Object: Starts with {
    // Else Invalid

    int n = jsonToken.size();
    char firstChar = jsonToken[0];
    char lastChar = jsonToken[n - 1];

    bool isFirstCharDigit = firstChar >= '0' && firstChar <= '9';
    bool isInteger =
        isFirstCharDigit ? jsonToken.find('.') == std::string::npos : false;

    if (jsonToken == "null") {
        return JsonType::NULL_TOKEN;
    }

    if (isFirstCharDigit && isInteger) {
        return JsonType::INT;
    } else if (isFirstCharDigit && !isInteger) {
        return JsonType::DOUBLE;
    } else if (firstChar == '[' && lastChar == ']') {
        return JsonType::ARRAY;
    } else if (jsonToken == "true" || jsonToken == "false") {
        return JsonType::BOOL;
    } else if (firstChar == '"' && lastChar == '"') {
        return JsonType::STRING;
    } else if (firstChar == '{' && lastChar == '}') {
        return JsonType::OBJECT;
    } else {
        LOGGER_ERROR("Invalid Token: " + jsonToken);
        throw std::invalid_argument("Invalid Json");
    }
}

// Function to get elements from an array
std::vector<std::string> Json::Json::getElementsOfJsonArrayString(
    const std::string &jsonString) {
    int n = jsonString.size();

    std::string token = "";
    std::vector<std::string> tokens;

    std::stack<char> stk;
    for (int i = 1; i <= n - 2; i++) {
        char ch = jsonString[i];
        char preCh = jsonString[i - 1];

        switch (ch) {
            case '[':
                if (!stk.empty() && stk.top() == '"') {
                    token.push_back(ch);
                } else {
                    stk.push(ch);
                    token.push_back(ch);
                }
                break;

            case ']':
                if (stk.empty()) {
                    throw std::invalid_argument(
                        "Invalid Json Array: While parsing");
                }

                if (stk.top() == '[') {
                    stk.pop();
                    token.push_back(ch);
                } else if (stk.top() == '"') {
                    token.push_back(ch);
                } else {
                    throw std::invalid_argument(
                        "Invalid Json Array: While parsing");
                }
                break;

            case '{':
                if (!stk.empty() && stk.top() == '"') {
                    token.push_back(ch);
                } else {
                    stk.push(ch);
                    token.push_back(ch);
                }

                break;

            case '}':
                if (stk.empty()) {
                    throw std::invalid_argument(
                        "Invalid Json Array: While parsing");
                }

                if (stk.top() == '{') {
                    stk.pop();
                    token.push_back(ch);
                } else if (stk.top() == '"') {
                    token.push_back(ch);
                } else {
                    throw std::invalid_argument(
                        "Invalid Json Array: While parsing");
                }
                break;

            case '"':
                if (preCh == '\\') {
                    // escape condition
                    token.push_back(ch);
                } else if (!stk.empty() && stk.top() == '"') {
                    stk.pop();
                    token.push_back(ch);
                } else {
                    stk.push(ch);
                    token.push_back(ch);
                }
                break;

            case ',':
                if (stk.empty()) {
                    tokens.push_back(token);
                    token.clear();
                } else {
                    token.push_back(ch);
                }
                break;

            default:
                token.push_back(ch);
                break;
        }
    }

    if (!stk.empty()) {
        throw std::invalid_argument("Invalid Json Array: While parsing");
    }

    // Insert the last token
    if (token.size() > 0) tokens.push_back(token);

    return tokens;
}

// Function to get key value pairs of a json string object
// the key will be returned as a string json, eg: "hi" and not hi
std::vector<std::pair<std::string, std::string>>
Json::Json::getKeyAndValuePairsOfJsonArrayObject(
    const std::string &jsonString) {
    int n = jsonString.size();
    std::vector<std::pair<std::string, std::string>> keyAndValuePairs;

    // first get the key till :
    // then get the value till ,
    int i = 1;
    while (i <= n - 2) {
        std::stack<char> stk;
        std::string key = "";

        // go till "
        while (i <= n - 2 && jsonString[i] != '"') i++;

        // Find the key till we get : and stack is empty
        bool keyStringCompleted = false;
        while (i <= n - 2 && !keyStringCompleted) {
            char ch = jsonString[i];
            char preCh = jsonString[i - 1];

            switch (ch) {
                case '[':
                    if (!stk.empty() && stk.top() == '"') {
                        key.push_back(ch);
                    } else {
                        stk.push(ch);
                        key.push_back(ch);
                    }
                    break;

                case ']':
                    if (stk.empty()) {
                        throw std::invalid_argument(
                            "Invalid Json Object: While parsing");
                    }

                    if (stk.top() == '[') {
                        stk.pop();
                        key.push_back(ch);
                    } else if (stk.top() == '"') {
                        key.push_back(ch);
                    } else {
                        throw std::invalid_argument(
                            "Invalid Json Object: While parsing");
                    }
                    break;

                case '{':
                    if (!stk.empty() && stk.top() == '"') {
                        key.push_back(ch);
                    } else {
                        stk.push(ch);
                        key.push_back(ch);
                    }

                    break;

                case '}':
                    if (stk.empty()) {
                        throw std::invalid_argument(
                            "Invalid Json Object: While parsing");
                    }

                    if (stk.top() == '{') {
                        stk.pop();
                        key.push_back(ch);
                    } else if (stk.top() == '"') {
                        key.push_back(ch);
                    } else {
                        throw std::invalid_argument(
                            "Invalid Json Object: While parsing");
                    }
                    break;

                case '"':
                    if (preCh == '\\') {
                        // escape condition
                        key.push_back(ch);
                    } else if (!stk.empty() && stk.top() == '"') {
                        stk.pop();
                        key.push_back(ch);
                    } else {
                        stk.push(ch);
                        key.push_back(ch);
                    }
                    break;

                case ',':
                    if (stk.empty() || stk.top() != '"') {
                        throw std::invalid_argument(
                            "Invalid Json Object: While parsing");
                    }
                    key.push_back(ch);
                    break;

                case ':':
                    if (!stk.empty()) {
                        if (stk.top() == '"') {
                            key.push_back(ch);
                        } else {
                            throw std::invalid_argument(
                                "Invalid Json Object: While parsing");
                        }
                    } else {
                        keyStringCompleted = true;
                    }

                    break;

                default:
                    key.push_back(ch);
                    break;
            }
            i++;
        }

        // Check if key only contains space and \n
        bool onlyContainsSpaceAndNewLine = true;
        for (auto ch : key) {
            if (ch != ' ' && ch != '\n') {
                onlyContainsSpaceAndNewLine = false;
                break;
            }
        }

        // This means that the object has ended
        if (onlyContainsSpaceAndNewLine) return keyAndValuePairs;

        if (!stk.empty() || !keyStringCompleted) {
            throw std::invalid_argument("Invalid Json Object: While parsing");
        }

        // extract the value
        std::string value = "";

        while (i <= n - 2) {
            char ch = jsonString[i];
            char preCh = jsonString[i - 1];

            bool shouldBreakLoop = false;
            switch (ch) {
                case '[':
                    if (!stk.empty() && stk.top() == '"') {
                        value.push_back(ch);
                    } else {
                        stk.push(ch);
                        value.push_back(ch);
                    }
                    break;

                case ']':
                    if (stk.empty()) {
                        throw std::invalid_argument(
                            "Invalid Json Object: While parsing");
                    }

                    if (stk.top() == '[') {
                        stk.pop();
                        value.push_back(ch);
                    } else if (stk.top() == '"') {
                        value.push_back(ch);
                    } else {
                        throw std::invalid_argument(
                            "Invalid Json Object: While parsing");
                    }
                    break;

                case '{':
                    if (!stk.empty() && stk.top() == '"') {
                        value.push_back(ch);
                    } else {
                        stk.push(ch);
                        value.push_back(ch);
                    }

                    break;

                case '}':
                    if (stk.empty()) {
                        throw std::invalid_argument(
                            "Invalid Json Object: While parsing");
                    }

                    if (stk.top() == '{') {
                        stk.pop();
                        value.push_back(ch);
                    } else if (stk.top() == '"') {
                        value.push_back(ch);
                    } else {
                        throw std::invalid_argument(
                            "Invalid Json Object: While parsing");
                    }
                    break;

                case '"':
                    if (preCh == '\\') {
                        // escape condition
                        value.push_back(ch);
                    } else if (!stk.empty() && stk.top() == '"') {
                        stk.pop();
                        value.push_back(ch);
                    } else {
                        stk.push(ch);
                        value.push_back(ch);
                    }
                    break;

                case ',':
                    if (stk.empty()) {
                        shouldBreakLoop = true;
                    } else {
                        value.push_back(ch);
                    }
                    break;

                default:
                    value.push_back(ch);
                    break;
            }
            i++;

            if (shouldBreakLoop) break;
        }

        if (!stk.empty()) {
            throw std::invalid_argument("Invalid Json Object: While parsing");
        }

        keyAndValuePairs.push_back({key, value});
    }

    return keyAndValuePairs;
}

void Json::Json::parseJsonFromString(const std::string &jsonString) {
    if (data != nullptr) delete data;
    data = parseJsonFromStringCallback(jsonString);
}

Json::JsonNode *Json::Json::parseJsonFromStringCallback(
    const std::string &jsonString) {
    JsonNode *node = new JsonNode();

    // Handling the null type
    if (jsonString.size() == 0) {
        node->type = JsonType::NULL_VALUE;
        return node;
    }

    // Clean the jsonString: Removing the extra spaces if any around it
    std::string cleanedJsonString = Utils::trim(jsonString);

    // Get the type of token
    JsonType tokenType = getJsonTokenType(cleanedJsonString);

    // Temp variables used in switch
    std::vector<std::string> tokens;
    std::string stringWithFirstAndLastCharTrimmed =
        cleanedJsonString.substr(1, cleanedJsonString.size() - 2);

    // Set the node type
    node->type = tokenType;

    switch (tokenType) {
        case JsonType::INT:
            node->intValue = std::stoll(cleanedJsonString);
            break;

        case JsonType::DOUBLE:
            node->doubleValue = std::stod(cleanedJsonString);
            break;

        case JsonType::BOOL:
            node->boolValue = (cleanedJsonString == "true" ? true : false);
            break;

        case JsonType::STRING:
            node->stringValue = stringWithFirstAndLastCharTrimmed;
            break;

        case JsonType::NULL_TOKEN:
            break;

        case JsonType::ARRAY:
            tokens = getElementsOfJsonArrayString(cleanedJsonString);
            for (std::string token : tokens) {
                node->arrayValue.push_back(parseJsonFromStringCallback(token));
            }
            break;

        case JsonType::OBJECT:
            std::vector<std::pair<std::string, std::string>> keyValuePairs =
                getKeyAndValuePairsOfJsonArrayObject(cleanedJsonString);

            for (std::pair<std::string, std::string> pair : keyValuePairs) {
                std::string key = pair.first;
                std::string value = pair.second;

                if (getJsonTokenType(key) != JsonType::STRING) {
                    throw std::invalid_argument("Invalid Json String");
                }

                // remove the " from the string
                key = key.substr(1, key.size() - 2);

                // Recursively call value to parse it
                node->objectValue[key] = parseJsonFromStringCallback(value);
            }
            break;
    }

    return node;
}

// Function to get string representation
std::string Json::Json::getJsonString() const {
    return data->getJsonString(0, 4);
}

namespace Json {
std::ostream &operator<<(std::ostream &os, const Json &data) {
    os << data.getJsonString();
    return os;
}
}  // namespace Json
