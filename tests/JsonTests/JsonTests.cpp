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

        if (jsonObject.data->getType() != Json::JsonType::NULL_VALUE) {
            totalTestFails += 1;
            Logger::log("Test Failed: Empty Json");
        }
    }

    // Number as Json
    {
        std::string jsonValue = "100";
        Json::Json jsonObject(jsonValue);

        if (jsonObject.data->getType() != Json::JsonType::INT ||
            jsonObject.data->getIntValue() != 100) {
            totalTestFails += 1;
            Logger::log("Test Failed: Number Json");
        }
    }

    // Double as Json
    {
        std::string jsonValue = "100.25";
        Json::Json jsonObject(jsonValue);

        if (jsonObject.data->getType() != Json::JsonType::DOUBLE ||
            jsonObject.data->getDoubleValue() != 100.25) {
            totalTestFails += 1;
            Logger::log("Test Failed: Double Json");
        }
    }

    // bool as Json
    {
        std::string jsonValue = "true";
        Json::Json jsonObject(jsonValue);

        if (jsonObject.data->getType() != Json::JsonType::BOOL ||
            jsonObject.data->getBoolValue() != true) {
            totalTestFails += 1;
            Logger::log("Test Failed: Bool Json");
        }
    }

    // string as Json
    {
        std::string jsonValue = "\"Http Server C++\"";
        Json::Json jsonObject(jsonValue);

        if (jsonObject.data->getType() != Json::JsonType::STRING ||
            jsonObject.data->getStringValue() != "Http Server C++") {
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

        if (jsonObject.data->getType() == Json::JsonType::ARRAY) {
            // Check if array is similar
            bool arrayIsSimilar = true;

            arrayIsSimilar = jsonObject.data->getArray().size() == arrSize;
            for (int i = 0; i < arrSize; i++) {
                Json::JsonNode *element = jsonObject.data->getArrayElement(i);
                if (element->getIntValue() != compareArray[i]) {
                    arrayIsSimilar = false;
                    break;
                }
            }

            if (!arrayIsSimilar) {
                totalTestFails += 1;
                Logger::log("Test Failed: Number Array Json");
            }
        } else {
            totalTestFails += 1;
            Logger::log("Test Failed: Number Array Json");
        }
    }

    // Array of double
    {
        int arrSize = 6;
        std::string jsonValue = "[1.1,2.5,5.6,3.5,4.4,5.8]";
        double compareArray[] = {1.1, 2.5, 5.6, 3.5, 4.4, 5.8};

        Json::Json jsonObject(jsonValue);

        if (jsonObject.data->getType() == Json::JsonType::ARRAY) {
            // Check if array is similar
            bool arrayIsSimilar = true;

            arrayIsSimilar = jsonObject.data->getArray().size() == arrSize;

            for (int i = 0; i < arrSize; i++) {
                Json::JsonNode *element = jsonObject.data->getArrayElement(i);
                if (element->getDoubleValue() != compareArray[i]) {
                    arrayIsSimilar = false;
                    break;
                }
            }

            if (!arrayIsSimilar) {
                totalTestFails += 1;
                Logger::log("Test Failed: Double Array Json");
            }
        } else {
            totalTestFails += 1;
            Logger::log("Test Failed: Double Array Json");
        }
    }

    // Array of string
    {
        int arrSize = 5;
        std::string jsonValue = "[\"hello\",\"world\",\",\",\"Json\",\"Here\"]";
        std::string compareArray[] = {"hello", "world", ",", "Json", "Here"};

        Json::Json jsonObject(jsonValue);

        if (jsonObject.data->getType() == Json::JsonType::ARRAY) {
            // Check if array is similar
            bool arrayIsSimilar = true;

            arrayIsSimilar = jsonObject.data->getArray().size() == arrSize;
            for (int i = 0; i < arrSize; i++) {
                Json::JsonNode *element = jsonObject.data->getArrayElement(i);
                if (element->getStringValue() != compareArray[i]) {
                    arrayIsSimilar = false;
                    break;
                }
            }

            if (!arrayIsSimilar) {
                totalTestFails += 1;
                Logger::log("Test Failed: String Array Json");
            }
        } else {
            totalTestFails += 1;
            Logger::log("Test Failed: String Array Json");
        }
    }

    // Array of bool
    {
        int arrSize = 5;
        std::string jsonValue = "[true,false,false,true,true]";
        bool compareArray[] = {true, false, false, true, true};

        Json::Json jsonObject(jsonValue);

        if (jsonObject.data->getType() == Json::JsonType::ARRAY) {
            // Check if array is similar
            bool arrayIsSimilar = true;

            arrayIsSimilar = jsonObject.data->getArray().size() == arrSize;
            for (int i = 0; i < arrSize; i++) {
                Json::JsonNode *element = jsonObject.data->getArrayElement(i);
                if (element->getBoolValue() != compareArray[i]) {
                    arrayIsSimilar = false;
                    break;
                }
            }

            if (!arrayIsSimilar) {
                totalTestFails += 1;
                Logger::log("Test Failed: Bool Array Json");
            }
        } else {
            totalTestFails += 1;
            Logger::log("Test Failed: Bool Array Json");
        }
    }

    // Object as Json
    {
        std::string jsonValue = "{\"language\": \"C++\", \"number\": 1}";
        Json::Json jsonObject(jsonValue);

        if (jsonObject.data->getType() != Json::JsonType::OBJECT ||
            jsonObject.data->getObjectValue("language")->getStringValue() !=
                "C++" ||
            jsonObject.data->getObjectValue("number")->getIntValue() != 1) {
            totalTestFails += 1;
            Logger::log("Test Failed: Object as Json");
        }
    }

    // Array of Array Json
    {
        int arrSize = 5;
        std::string jsonValue = "[1,[1,2,3,[6]],3,4,5]";

        Json::Json jsonObject(jsonValue);

        if (jsonObject.data->getType() == Json::JsonType::ARRAY) {
            // get the value 6
            if (jsonObject.data->getArrayElement(1)
                    ->getArrayElement(3)
                    ->getArrayElement(0)
                    ->getIntValue() != 6) {
                totalTestFails += 1;
                Logger::log("Test Failed: Array Array Json");
            }
        } else {
            totalTestFails += 1;
            Logger::log("Test Failed: Array Array Json");
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

        // Print string
        Logger::log(jsonObject.getJsonString());

        bool isJsonParsedCorrectly = true;

        if (jsonObject.data->getType() != Json::JsonType::OBJECT) {
            isJsonParsedCorrectly = false;
        }

        if (isJsonParsedCorrectly &&
            jsonObject.data->getObjectValue("language")->getStringValue() !=
                "C++") {
            isJsonParsedCorrectly = false;
        }

        if (isJsonParsedCorrectly && jsonObject.data->getObjectValue("tests")
                                             ->getArrayElement(0)
                                             ->getStringValue() != "trim") {
            isJsonParsedCorrectly = false;
        }

        if (isJsonParsedCorrectly &&
            jsonObject.data->getObjectValue("project")
                    ->getObjectValue("creator")
                    ->getStringValue() != "Suhaan Ramdas Bhandary") {
            isJsonParsedCorrectly = false;
        }

        if (isJsonParsedCorrectly && jsonObject.data->getObjectValue("project")
                                             ->getObjectValue("stars")
                                             ->getIntValue() != 6) {
            isJsonParsedCorrectly = false;
        }

        if (isJsonParsedCorrectly && jsonObject.data->getObjectValue("project")
                                             ->getObjectValue("isInDevelopment")
                                             ->getBoolValue() != true) {
            isJsonParsedCorrectly = false;
        }

        if (isJsonParsedCorrectly && jsonObject.data->getObjectValue("systems")
                                             ->getArrayElement(0)
                                             ->getObjectValue("name")
                                             ->getStringValue() != "ubuntu") {
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