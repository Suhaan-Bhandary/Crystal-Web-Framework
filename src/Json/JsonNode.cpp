#include "JsonNode.h"

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

// Function to get string representation
std::string Json::JsonNode::getJsonString(int indentationLevel,
                                          int indentSpaces) {
    std::string indentation = std::string(indentationLevel * indentSpaces, ' ');
    std::string nextLevelIndentation =
        std::string((indentationLevel + 1) * indentSpaces, ' ');

    // If root primitive type then return it
    if (type == JsonType::NULL_VALUE) return "null";
    if (type == JsonType::INT) return std::to_string(intValue);
    if (type == JsonType::DOUBLE) {
        return std::to_string(doubleValue);
    }
    if (type == JsonType::BOOL) {
        return boolValue ? "true" : "false";
    }
    if (type == JsonType::STRING) {
        return "\"" + stringValue + "\"";
    }

    // Array
    if (type == JsonType::ARRAY) {
        int n = arrayValue.size();

        std::string arrayJsonString = "";
        arrayJsonString += "[\n";

        for (int i = 0; i < n; i++) {
            arrayJsonString +=
                (nextLevelIndentation +
                 arrayValue[i]->getJsonString(indentationLevel + 1, 4));

            // append a comma if it is not last element
            if (i != n - 1) arrayJsonString += ",\n";
        }

        arrayJsonString += "\n" + indentation + "]";

        return arrayJsonString;
    }

    // Object
    if (type == JsonType::OBJECT) {
        int n = objectValue.size();
        int counter = 0;

        std::string objectJsonString = "";

        objectJsonString += "{\n";

        for (auto p : objectValue) {
            std::string key = p.first;
            JsonNode *value = p.second;
            counter++;

            objectJsonString += nextLevelIndentation + "\"" + key + "\"";

            objectJsonString += ": ";
            objectJsonString += value->getJsonString(indentationLevel + 1, 4);

            if (counter != n) {
                objectJsonString += ",\n";
            }
        }

        objectJsonString += "\n" + indentation + "}";

        return objectJsonString;
    }

    return "null";
}
