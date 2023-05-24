// The ifndef is used so that the same File is not loaded again
#ifndef INCLUDED_JSON_Node
#define INCLUDED_JSON_Node

#include <iostream>
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
   private:
    JsonType type;

    long long intValue;
    double doubleValue;
    bool boolValue;
    std::string stringValue;

    std::vector<JsonNode *> arrayValue;
    std::unordered_map<std::string, JsonNode *> objectValue;

   public:
    JsonNode();
    ~JsonNode();

    // Getter functions
    JsonType getType();
    long long getIntValue();
    double getDoubleValue();
    bool getBoolValue();
    std::string getStringValue();

    std::vector<JsonNode *> getArray();
    JsonNode *getArrayElement(int index);
    JsonNode *getObjectValue(const std::string &key);

    // Clear the node and set it to given type
    void clearJsonNodeAndSetNewType(JsonType newType);

    // Initially in the node we can set the data type to other
    void setValue(long long value);
    void setValue(double value);
    void setValue(bool value);
    void setValue(const std::string &value);
    void setValue(const char *value);

    // Create array and object
    void setAsArray();
    void setAsObject();

    // Insert element in array
    void insertInArray(long long value);
    void insertInArray(double value);
    void insertInArray(bool value);
    void insertInArray(const std::string &value);
    void insertInArray(const char *value);

    void insertEmptyArray();
    void insertEmptyObject();

    // Insert key value in object
    void insertInObject(const std::string &key, long long value);
    void insertInObject(const std::string &key, double value);
    void insertInObject(const std::string &key, bool value);
    void insertInObject(const std::string &key, const std::string &value);
    void insertInObject(const std::string &key, const char *value);
    void insertEmptyArray(const std::string &key);
    void insertEmptyObject(const std::string &key);

    // String representation
    std::string getJsonString(int indentationLevel, int indentSpaces);

    // friend class declaration, so that it can access private variables
    friend class Json;
};

}  // namespace Json

#endif