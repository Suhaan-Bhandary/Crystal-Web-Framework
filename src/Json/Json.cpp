#include "Json.h"

#include <iostream>
#include <stack>
#include <stdexcept>

#include "../Logger/Logger.h"
#include "../Utils/Utils.h"

// Json Node
// Setting all the values to nullptr
Json::JsonNode::JsonNode() { type = JsonType::NULL_VALUE; }

Json::JsonType Json::JsonNode::getType() { return type; }

long long Json::JsonNode::getIntValue() {
    if (type != JsonType::INT) {
        throw std::invalid_argument("Json Value not found");
    }
    return intValue;
}

double Json::JsonNode::getDoubleValue() {
    if (type != JsonType::DOUBLE) {
        throw std::invalid_argument("Json Value not found");
    }
    return doubleValue;
}

bool Json::JsonNode::getBoolValue() {
    if (type != JsonType::BOOL) {
        throw std::invalid_argument("Json Value not found");
    }
    return boolValue;
}

std::string Json::JsonNode::getStringValue() {
    if (type != JsonType::STRING) {
        throw std::invalid_argument("Json Value not found");
    }
    return stringValue;
}

std::vector<Json::JsonNode *> Json::JsonNode::getArray() { return arrayValue; }

Json::JsonNode *Json::JsonNode::getArrayElement(int index) {
    if (type != JsonType::ARRAY || index >= arrayValue.size()) {
        throw std::invalid_argument("Json Value not found");
    }

    return arrayValue[index];
}

Json::JsonNode *Json::JsonNode::getObjectValue(const std::string &key) {
    if (type != JsonType::OBJECT || objectValue.count(key) == 0) {
        throw std::invalid_argument("Json Value not found");
    }

    return objectValue[key];
}

// Json
Json::Json::Json(const std::string &body) {
    data = nullptr;
    parseJsonFromString(body);
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
        Logger::log(jsonToken);
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
                if (!stk.empty() && stk.top() == '"') {
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
                    if (!stk.empty() && stk.top() == '"') {
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

        if (!stk.empty() || !keyStringCompleted) {
            throw std::invalid_argument("Invalid Json Object: While parsing");
        }

        // extract the value
        std::string value = "";

        bool valueStringCompleted = false;
        while (i <= n - 2 && !valueStringCompleted) {
            char ch = jsonString[i];

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
                    if (!stk.empty() && stk.top() == '"') {
                        stk.pop();
                        value.push_back(ch);
                    } else {
                        stk.push(ch);
                        value.push_back(ch);
                    }
                    break;

                case ',':
                    if (stk.empty()) {
                        valueStringCompleted = true;
                    } else {
                        value.push_back(ch);
                    }
                    break;

                default:
                    value.push_back(ch);
                    break;
            }
            i++;
        }

        if (!stk.empty() && !valueStringCompleted) {
            throw std::invalid_argument("Invalid Json Object: While parsing");
        }

        // std::cout << key << ":" << value << std::endl;
        keyAndValuePairs.push_back({key, value});
    }

    return keyAndValuePairs;
}

void Json::Json::parseJsonFromString(const std::string &jsonString) {
    // TODO: Delete the tree if present
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