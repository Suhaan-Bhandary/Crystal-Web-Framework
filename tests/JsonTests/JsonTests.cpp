#include <fstream>
#include <iostream>
#include <string>

#include "../../src/Json/Json.h"
#include "../../src/Logger/Logger.h"
#include "../../src/Utils/Utils.h"
#include "../tests.h"

void Test::JsonTests() {
    LOGGER_MINIMAL("\n--------------------- Json Tests ---------------------");

    int totalTestFails = 0;

    // *Below the parenthesis is used to create a simple scope

    // Empty Json
    {
        Json::Json jsonObject("");

        if (jsonObject.getData().getType() != JsonType::NULL_) {
            totalTestFails += 1;
            LOGGER_ERROR("Test Failed: Empty Json");
        }
    }

    // Number as Json
    {
        Json::Json jsonObject("100");

        if (jsonObject.getData().getType() != JsonType::NUMBER ||
            jsonObject.getData().get<Json::Number>() != 100) {
            totalTestFails += 1;
            LOGGER_ERROR("Test Failed: Number Json");
        }
    }

    // Double as Json
    {
        Json::Json jsonObject("100.25");

        if (jsonObject.getData().getType() != JsonType::FRACTION ||
            jsonObject.getData().get<Json::Fraction>() != 100.25) {
            totalTestFails += 1;
            LOGGER_ERROR("Test Failed: Double Json");
        }
    }

    // bool as Json
    {
        Json::Json jsonObject("true");

        if (jsonObject.getData().getType() != JsonType::BOOL ||
            jsonObject.getData().get<Json::Bool>() != true) {
            totalTestFails += 1;
            LOGGER_ERROR("Test Failed: Bool Json");
        }
    }

    // string as Json
    {
        Json::Json jsonObject("\"Http Server C++\"");

        if (jsonObject.getData().getType() != JsonType::STRING ||
            jsonObject.getData().get<Json::String>() != "Http Server C++") {
            totalTestFails += 1;
            LOGGER_ERROR("Test Failed: String Json");
        }
    }

    // Array as json
    // Array of number
    {
        int arrSize = 5;
        std::string jsonValue = "[1,2,3,4,5]";
        int compareArray[] = {1, 2, 3, 4, 5};

        Json::Json jsonObject(jsonValue.c_str());

        if (jsonObject.getData().getType() == JsonType::ARRAY) {
            // Check if array is similar
            bool arrayIsSimilar = true;

            arrayIsSimilar =
                jsonObject.getData().get<Json::Array>().size() == arrSize;
            for (int i = 0; i < arrSize; i++) {
                if (jsonObject.getData().get(i).get<Json::Number>() !=
                    compareArray[i]) {
                    arrayIsSimilar = false;
                    break;
                }
            }

            if (!arrayIsSimilar) {
                totalTestFails += 1;
                LOGGER_ERROR("Test Failed: Number Array Json");
            }
        } else {
            totalTestFails += 1;
            LOGGER_ERROR("Test Failed: Number Array Json");
        }
    }

    // Array of double
    {
        int arrSize = 6;
        std::string jsonValue = "[1.1,2.5,5.6,3.5,4.4,5.8]";
        double compareArray[] = {1.1, 2.5, 5.6, 3.5, 4.4, 5.8};

        Json::Json jsonObject(jsonValue.c_str());

        if (jsonObject.getData().getType() == JsonType::ARRAY) {
            // Check if array is similar
            bool arrayIsSimilar = true;

            arrayIsSimilar =
                jsonObject.getData().get<Json::Array>().size() == arrSize;

            for (int i = 0; i < arrSize; i++) {
                if (jsonObject.getData().get(i).get<Json::Fraction>() !=
                    compareArray[i]) {
                    arrayIsSimilar = false;
                    break;
                }
            }

            if (!arrayIsSimilar) {
                totalTestFails += 1;
                LOGGER_ERROR("Test Failed: Double Array Json");
            }
        } else {
            totalTestFails += 1;
            LOGGER_ERROR("Test Failed: Double Array Json");
        }
    }

    // Array of string
    {
        int arrSize = 5;
        std::string jsonValue = "[\"hello\",\"world\",\",\",\"Json\",\"Here\"]";
        std::string compareArray[] = {"hello", "world", ",", "Json", "Here"};

        Json::Json jsonObject(jsonValue.c_str());

        if (jsonObject.getData().getType() == JsonType::ARRAY) {
            // Check if array is similar
            bool arrayIsSimilar = true;

            arrayIsSimilar =
                jsonObject.getData().get<Json::Array>().size() == arrSize;

            for (int i = 0; i < arrSize; i++) {
                if (jsonObject.getData().get(i).get<Json::String>() !=
                    compareArray[i]) {
                    arrayIsSimilar = false;
                    break;
                }
            }

            if (!arrayIsSimilar) {
                totalTestFails += 1;
                LOGGER_ERROR("Test Failed: String Array Json");
            }
        } else {
            totalTestFails += 1;
            LOGGER_ERROR("Test Failed: String Array Json");
        }
    }

    // Array of bool
    {
        int arrSize = 5;
        std::string jsonValue = "[true,false,false,true,true]";
        bool compareArray[] = {true, false, false, true, true};

        Json::Json jsonObject(jsonValue.c_str());

        if (jsonObject.getData().getType() == JsonType::ARRAY) {
            // Check if array is similar
            bool arrayIsSimilar = true;
            arrayIsSimilar =
                jsonObject.getData().get<Json::Array>().size() == arrSize;

            for (int i = 0; i < arrSize; i++) {
                if (jsonObject.getData().get(i).get<Json::Bool>() !=
                    compareArray[i]) {
                    arrayIsSimilar = false;
                    break;
                }
            }

            if (!arrayIsSimilar) {
                totalTestFails += 1;
                LOGGER_ERROR("Test Failed: Bool Array Json");
            }
        } else {
            totalTestFails += 1;
            LOGGER_ERROR("Test Failed: Bool Array Json");
        }
    }

    // Object as Json
    {
        std::string jsonValue = "{\"language\": \"C++\", \"number\": 1}";
        Json::Json jsonObject(jsonValue.c_str());

        if (jsonObject.getData().getType() != JsonType::OBJECT ||
            jsonObject.getData().get("language").get<Json::String>() != "C++" ||
            jsonObject.getData().get("number").get<Json::Number>() != 1) {
            totalTestFails += 1;
            LOGGER_ERROR("Test Failed: Object as Json");
        }
    }

    // Array of Array Json
    {
        int arrSize = 5;
        std::string jsonValue = "[1,[1,2,3,[6]],3,4,5]";

        Json::Json jsonObject(jsonValue.c_str());

        if (jsonObject.getData().getType() == JsonType::ARRAY) {
            // get the value 6
            if (jsonObject.getData().get(1).get(3).get(0).get<Json::Number>() !=
                6) {
                totalTestFails += 1;
                LOGGER_ERROR("Test Failed: Array Array Json");
            }
        } else {
            totalTestFails += 1;
            LOGGER_ERROR("Test Failed: Array Array Json");
        }
    }

    // Complex Json Object from Json File
    {
        // Read json from a file
        std::string rootDir = Utils::getCurrentDirectory();
        std::ifstream JsonFile(rootDir + "/tests/JsonTests/testData.json");

        if (!JsonFile.is_open()) {
            LOGGER_ERROR("Error in opening file!!");
            return;
        }

        std::string jsonValue((std::istreambuf_iterator<char>(JsonFile)),
                              (std::istreambuf_iterator<char>()));

        // close the file
        JsonFile.close();

        // Creating a json object from the json file
        Json::Json jsonObject(jsonValue.c_str());

        // Print string
        // LOGGER_ERROR(jsonObject.getJsonString());

        bool isJsonParsedCorrectly = true;

        if (jsonObject.getData().getType() != JsonType::OBJECT) {
            isJsonParsedCorrectly = false;
        }

        if (isJsonParsedCorrectly &&
            jsonObject.getData().get("language").get<Json::String>() != "C++") {
            isJsonParsedCorrectly = false;
        }

        if (isJsonParsedCorrectly &&
            jsonObject.getData().get("tests").get(0).get<Json::String>() !=
                "trim") {
            isJsonParsedCorrectly = false;
        }

        if (isJsonParsedCorrectly &&
            jsonObject.getData()
                    .get("project")
                    .get("creator")
                    .get<Json::String>() != "Suhaan Ramdas Bhandary") {
            isJsonParsedCorrectly = false;
        }

        if (isJsonParsedCorrectly && jsonObject.getData()
                                             .get("project")
                                             .get("stars")
                                             .get<Json::Number>() != 6) {
            isJsonParsedCorrectly = false;
        }

        if (isJsonParsedCorrectly &&
            !jsonObject.getData()
                 .get("project")
                 .get("isInDevelopment")
                 .isNull() &&
            jsonObject.getData()
                    .get("project")
                    .get("isInDevelopment")
                    .get<Json::Bool>() != true) {
            isJsonParsedCorrectly = false;
        }

        if (isJsonParsedCorrectly && jsonObject.getData()
                                             .get("systems")
                                             .get(0)
                                             .get("name")
                                             .get<Json::String>() != "ubuntu") {
            isJsonParsedCorrectly = false;
        }

        if (!isJsonParsedCorrectly) {
            totalTestFails += 1;
            LOGGER_ERROR("Test Failed: Object as Json");
        }
    }

    // Invalid Json
    {
        try {
            std::string jsonValue = "{\"\"}";
            Json::Json jsonObject(jsonValue.c_str());

            totalTestFails += 1;
            LOGGER_ERROR("Test Failed: Invalid Json");
        } catch (const std::exception &e) {
            // LOGGER_ERROR(e.what());
            // If it catches it means it is working correctly
        }
    }

    // Test for json object creation
    {
        try {
            Json::Json jsonObject;
            jsonObject.getData().set(Json::Object());

            jsonObject.getData().set("name", "suhaan");
            jsonObject.getData().set("email", "suhaan");

            jsonObject.getData().set("colors", Json::Array());

            jsonObject.getData().get("colors").push("red");
            jsonObject.getData().get("colors").push("blue");
            jsonObject.getData().get("colors").push("green");

            // LOGGER(jsonObject.getJsonString());

            if (jsonObject.getData().get("colors").get(0).get<Json::String>() !=
                "red") {
                totalTestFails += 1;
                LOGGER_ERROR("Test Failed: Json Object Creation");
            }

        } catch (const std::exception &e) {
            totalTestFails += 1;
            LOGGER_ERROR(e.what());
            LOGGER_ERROR("Test Failed: Json Object Creation");
        }
    }

    // Try copy constructor
    {
        std::string jsonValue = "100";
        Json::Json jsonObject(jsonValue.c_str());

        Json::Json copiedObject = jsonObject;

        if (copiedObject.getData().getType() != JsonType::NUMBER ||
            copiedObject.getData().get<Json::Number>() != 100) {
            totalTestFails += 1;
            LOGGER_ERROR("Test Failed: Copy Constructor");
        }
    }

    // Try empty object
    {
        try {
            std::string jsonValue = "{\n   \n  \n \n}";
            Json::Json jsonObject(jsonValue.c_str());
        } catch (const std::exception &e) {
            totalTestFails += 1;
            LOGGER_ERROR("Test Failed: Emtpy Object");
        }
    }

    LOGGER_NOTE("\nTotal Fails: " + std::to_string(totalTestFails));
}