#include "JsonNode.h"

#include "../Logger/Logger.h"

// Json Node
// Setting all the values to nullptr
Json::JsonNode::JsonNode() { type = JsonType::NULL_VALUE; }

Json::JsonNode::~JsonNode() {
    // Logger::log("\nJsonNode Destructor");
    // Logger::log(getJsonString(0, 4));

    if (type == JsonType::ARRAY) {
        for (auto element : arrayValue) delete element;
        arrayValue.clear();
    } else if (type == JsonType::OBJECT) {
        for (auto pair : objectValue) delete pair.second;
        objectValue.clear();
    }
}

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

// Clear the node and set it to given type
void Json::JsonNode::clearJsonNodeAndSetNewType(JsonType newType) {
    // Clear the array or object if it has it
    if (type == JsonType::ARRAY) {
        for (auto element : arrayValue) delete element;
        arrayValue.clear();
    } else if (type == JsonType::OBJECT) {
        for (auto pair : objectValue) delete pair.second;
        objectValue.clear();
    }

    // Set the new type
    type = newType;
}

// Initially in the node we can set the data type to other
void Json::JsonNode::setValue(long long value) {
    clearJsonNodeAndSetNewType(JsonType::INT);
    intValue = value;
}

void Json::JsonNode::setValue(double value) {
    clearJsonNodeAndSetNewType(JsonType::DOUBLE);
    doubleValue = value;
}

void Json::JsonNode::setValue(bool value) {
    clearJsonNodeAndSetNewType(JsonType::BOOL);
    boolValue = value;
}

void Json::JsonNode::setValue(const std::string &value) {
    clearJsonNodeAndSetNewType(JsonType::STRING);
    stringValue = value;
}

void Json::JsonNode::setValue(const char *value) {
    clearJsonNodeAndSetNewType(JsonType::STRING);
    stringValue = (std::string)value;
}

// Create array
void Json::JsonNode::setAsArray() {
    clearJsonNodeAndSetNewType(JsonType::ARRAY);
}

// Create object
void Json::JsonNode::setAsObject() {
    clearJsonNodeAndSetNewType(JsonType::OBJECT);
}

// Insert in array
void Json::JsonNode::insertInArray(long long value) {
    if (type != JsonType::ARRAY) {
        throw std::invalid_argument("Json not an Array");
    }

    JsonNode *node = new JsonNode();
    node->setValue(value);
    arrayValue.push_back(node);
}

void Json::JsonNode::insertInArray(double value) {
    if (type != JsonType::ARRAY) {
        throw std::invalid_argument("Json not an Array");
    }

    JsonNode *node = new JsonNode();
    node->setValue(value);
    arrayValue.push_back(node);
}

void Json::JsonNode::insertInArray(bool value) {
    if (type != JsonType::ARRAY) {
        throw std::invalid_argument("Json not an Array");
    }

    JsonNode *node = new JsonNode();
    node->setValue(value);
    arrayValue.push_back(node);
}

void Json::JsonNode::insertInArray(const char *value) {
    if (type != JsonType::ARRAY) {
        throw std::invalid_argument("Json not an Array");
    }

    JsonNode *node = new JsonNode();
    node->setValue((std::string)value);
    arrayValue.push_back(node);
}

void Json::JsonNode::insertInArray(const std::string &value) {
    if (type != JsonType::ARRAY) {
        throw std::invalid_argument("Json not an Array");
    }

    JsonNode *node = new JsonNode();
    node->setValue((std::string)value);
    arrayValue.push_back(node);
}

void Json::JsonNode::insertEmptyArray() {
    if (type != JsonType::ARRAY) {
        throw std::invalid_argument("Json not an Array");
    }

    JsonNode *node = new JsonNode();
    node->type = JsonType::ARRAY;
    arrayValue.push_back(node);
}

void Json::JsonNode::insertEmptyObject() {
    if (type != JsonType::ARRAY) {
        throw std::invalid_argument("Json not an Array");
    }

    JsonNode *node = new JsonNode();
    node->type = JsonType::OBJECT;
    arrayValue.push_back(node);
}

// Insert key value in object
void Json::JsonNode::insertInObject(const std::string &key, long long value) {
    if (type != JsonType::OBJECT) {
        throw std::invalid_argument("Json not an Object");
    }

    JsonNode *node = new JsonNode();
    node->setValue(value);
    objectValue[key] = node;
}

void Json::JsonNode::insertInObject(const std::string &key, double value) {
    if (type != JsonType::OBJECT) {
        throw std::invalid_argument("Json not an Object");
    }

    JsonNode *node = new JsonNode();
    node->setValue(value);
    objectValue[key] = node;
}

void Json::JsonNode::insertInObject(const std::string &key, bool value) {
    if (type != JsonType::OBJECT) {
        throw std::invalid_argument("Json not an Object");
    }

    JsonNode *node = new JsonNode();
    node->setValue(value);
    objectValue[key] = node;
}

void Json::JsonNode::insertInObject(const std::string &key,
                                    const std::string &value) {
    if (type != JsonType::OBJECT) {
        throw std::invalid_argument("Json not an Object");
    }

    JsonNode *node = new JsonNode();
    node->setValue(value);
    objectValue[key] = node;
}

void Json::JsonNode::insertInObject(const std::string &key, const char *value) {
    if (type != JsonType::OBJECT) {
        throw std::invalid_argument("Json not an Object");
    }

    JsonNode *node = new JsonNode();
    node->setValue((std::string)value);
    objectValue[key] = node;
}

void Json::JsonNode::insertEmptyArray(const std::string &key) {
    if (type != JsonType::OBJECT) {
        throw std::invalid_argument("Json not an Object");
    }

    JsonNode *node = new JsonNode();
    node->type = JsonType::ARRAY;
    objectValue[key] = node;
}

void Json::JsonNode::insertEmptyObject(const std::string &key) {
    if (type != JsonType::OBJECT) {
        throw std::invalid_argument("Json not an Object");
    }

    JsonNode *node = new JsonNode();
    node->type = JsonType::OBJECT;
    objectValue[key] = node;
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
