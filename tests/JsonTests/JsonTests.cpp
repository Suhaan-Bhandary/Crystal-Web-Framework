#include <fstream>
#include <iostream>
#include <string>

#include "../../src/Json/Json.h"
#include "../../src/Logger/Logger.h"
#include "../../src/Utils/Utils.h"
#include "../tests.h"

void Test::JsonTests() {
    Logger::log("\n--------------------- Json Tests ---------------------");

    int totalTestFails = 0;

    // *Below the parenthesis is used to create a simple scope

    // Empty Json
    {
        std::string jsonValue = "";
        Json::Json jsonObject(jsonValue);

        if (jsonObject.data->type != Json::JsonType::NULL_VALUE) {
            totalTestFails += 1;
            Logger::log("Test Failed: Empty Json");
        }
    }

    // Number as Json
    {
        std::string jsonValue = "100";
        Json::Json jsonObject(jsonValue);

        if (jsonObject.data->type != Json::JsonType::INT ||
            jsonObject.data->intValue != 100) {
            totalTestFails += 1;
            Logger::log("Test Failed: Number Json");
        }
    }

    // Double as Json
    {
        std::string jsonValue = "100.25";
        Json::Json jsonObject(jsonValue);

        if (jsonObject.data->type != Json::JsonType::DOUBLE ||
            jsonObject.data->doubleValue != 100.25) {
            totalTestFails += 1;
            Logger::log("Test Failed: Double Json");
        }
    }

    // bool as Json
    {
        std::string jsonValue = "true";
        Json::Json jsonObject(jsonValue);

        if (jsonObject.data->type != Json::JsonType::BOOL ||
            jsonObject.data->boolValue != true) {
            totalTestFails += 1;
            Logger::log("Test Failed: Bool Json");
        }
    }

    // string as Json
    {
        std::string jsonValue = "\"Http Server C++\"";
        Json::Json jsonObject(jsonValue);

        if (jsonObject.data->type != Json::JsonType::STRING ||
            jsonObject.data->stringValue != "Http Server C++") {
            totalTestFails += 1;
            Logger::log("Test Failed: String Json");
        }
    }

    // Array as json
    // Array of number
    {
        int arrSize = 5;
        std::string jsonValue = "[1,2,3,4,5]";
        int compareArray[] = {1, 2, 3, 4, 5};

        Json::Json jsonObject(jsonValue);

        if (jsonObject.data->type != Json::JsonType::ARRAY) {
            // Check if array is similar
            bool arrayIsSimilar = true;

            arrayIsSimilar = jsonObject.data->arrayValue.size() == arrSize;
            for (int i = 0; i < arrSize; i++) {
                Json::JsonNode *element = jsonObject.data->arrayValue[i];
                if (element->intValue != compareArray[i]) {
                    arrayIsSimilar = false;
                    break;
                }
            }

            if (!arrayIsSimilar) {
                totalTestFails += 1;
                Logger::log("Test Failed: Number Array Json");
            }
        }
    }

    // Array of double
    {
        int arrSize = 5;
        std::string jsonValue = "[1.1,2.5,5.6,3.5,4.4,5.8]";
        double compareArray[] = {1.1, 2.5, 5.6, 3.5, 4.4, 5.8};

        Json::Json jsonObject(jsonValue);

        if (jsonObject.data->type != Json::JsonType::ARRAY) {
            // Check if array is similar
            bool arrayIsSimilar = true;

            arrayIsSimilar = jsonObject.data->arrayValue.size() == arrSize;
            for (int i = 0; i < arrSize; i++) {
                Json::JsonNode *element = jsonObject.data->arrayValue[i];
                if (element->doubleValue != compareArray[i]) {
                    arrayIsSimilar = false;
                    break;
                }
            }

            if (!arrayIsSimilar) {
                totalTestFails += 1;
                Logger::log("Test Failed: Double Array Json");
            }
        }
    }

    // Array of string
    {
        int arrSize = 5;
        std::string jsonValue = "[\"hello\",\"world\",\",\",\"Json\",\"Here\"]";
        std::string compareArray[] = {"hello", "world", ",", "Json", "Here"};

        Json::Json jsonObject(jsonValue);

        if (jsonObject.data->type != Json::JsonType::ARRAY) {
            // Check if array is similar
            bool arrayIsSimilar = true;

            arrayIsSimilar = jsonObject.data->arrayValue.size() == arrSize;
            for (int i = 0; i < arrSize; i++) {
                Json::JsonNode *element = jsonObject.data->arrayValue[i];
                if (element->stringValue != compareArray[i]) {
                    arrayIsSimilar = false;
                    break;
                }
            }

            if (!arrayIsSimilar) {
                totalTestFails += 1;
                Logger::log("Test Failed: String Array Json");
            }
        }
    }

    // Array of bool
    {
        int arrSize = 5;
        std::string jsonValue = "[true,false,false,true,true]";
        bool compareArray[] = {true, false, false, true, true};

        Json::Json jsonObject(jsonValue);

        if (jsonObject.data->type != Json::JsonType::ARRAY) {
            // Check if array is similar
            bool arrayIsSimilar = true;

            arrayIsSimilar = jsonObject.data->arrayValue.size() == arrSize;
            for (int i = 0; i < arrSize; i++) {
                Json::JsonNode *element = jsonObject.data->arrayValue[i];
                if (element->boolValue != compareArray[i]) {
                    arrayIsSimilar = false;
                    break;
                }
            }

            if (!arrayIsSimilar) {
                totalTestFails += 1;
                Logger::log("Test Failed: Bool Array Json");
            }
        }
    }

    // Object as Json
    {
        std::string jsonValue = "{\"language\": \"C++\", \"number\": 1}";
        Json::Json jsonObject(jsonValue);

        if (jsonObject.data->type != Json::JsonType::OBJECT ||
            jsonObject.data->objectValue["language"]->stringValue != "C++" ||
            jsonObject.data->objectValue["number"]->intValue != 1) {
            totalTestFails += 1;
            Logger::log("Test Failed: Object as Json");
        }
    }

    // Complex Json Object from Json File
    {
        // Read json from a file
        std::string rootDir = Utils::getCurrentDirectory();
        std::ifstream JsonFile(rootDir + "/tests/JsonTests/testData.json");

        if (!JsonFile.is_open()) {
            Logger::log("Error in opening file!!");
            return;
        }

        std::string jsonValue((std::istreambuf_iterator<char>(JsonFile)),
                              (std::istreambuf_iterator<char>()));

        // close the file
        JsonFile.close();

        // Creating a json object from the json file
        Json::Json jsonObject(jsonValue);

        bool isJsonParsedCorrectly = true;

        if (jsonObject.data->type != Json::JsonType::OBJECT) {
            isJsonParsedCorrectly = false;
        }

        if (isJsonParsedCorrectly &&
            jsonObject.data->objectValue["language"]->stringValue != "C++") {
            isJsonParsedCorrectly = false;
        }

        if (isJsonParsedCorrectly &&
            jsonObject.data->objectValue["tests"]->arrayValue[0]->stringValue !=
                "trim") {
            isJsonParsedCorrectly = false;
        }

        if (isJsonParsedCorrectly &&
            jsonObject.data->objectValue["project"]
                    ->objectValue["creator"]
                    ->stringValue != "Suhaan Ramdas Bhandary") {
            isJsonParsedCorrectly = false;
        }

        if (isJsonParsedCorrectly && jsonObject.data->objectValue["project"]
                                             ->objectValue["stars"]
                                             ->intValue != 6) {
            isJsonParsedCorrectly = false;
        }

        if (isJsonParsedCorrectly && jsonObject.data->objectValue["project"]
                                             ->objectValue["isInDevelopment"]
                                             ->boolValue != true) {
            isJsonParsedCorrectly = false;
        }

        if (isJsonParsedCorrectly && jsonObject.data->objectValue["systems"]
                                             ->arrayValue[0]
                                             ->objectValue["name"]
                                             ->stringValue != "ubuntu") {
            isJsonParsedCorrectly = false;
        }

        if (!isJsonParsedCorrectly) {
            totalTestFails += 1;
            Logger::log("Test Failed: Object as Json");
        }
    }

    // Invalid Json
    {
        try {
            std::string jsonValue = "{\"\"}";
            Json::Json jsonObject(jsonValue);

            totalTestFails += 1;
            Logger::log("Test Failed: Invalid Json");
        } catch (const std::exception &e) {
            // Logger::log(e.what());
            // If it catches it means it is working correctly
        }
    }
    Logger::log("\nTotal Fails: " + std::to_string(totalTestFails));
}