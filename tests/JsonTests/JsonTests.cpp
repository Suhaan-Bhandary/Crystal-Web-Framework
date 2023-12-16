#include <string>

#include "../tests.h"

void Test::JsonTests() {
    LOGGER_MINIMAL("\n--------------------- Json Tests ---------------------");

    int totalTestFails = 0;

    // *Below the parenthesis is used to create a simple scope

    // Official test suit
    std::vector<std::string> testWithNULLResult = {
        "fail1.json",  "fail10.json", "fail11.json", "fail12.json",
        "fail13.json", "fail14.json", "fail15.json", "fail16.json",
        "fail17.json", "fail19.json", "fail2.json",  "fail20.json",
        "fail21.json", "fail22.json", "fail23.json", "fail24.json",
        "fail25.json", "fail26.json", "fail27.json", "fail28.json",
        "fail29.json", "fail3.json",  "fail30.json", "fail31.json",
        "fail32.json", "fail33.json", "fail4.json",  "fail5.json",
        "fail6.json",  "fail7.json",  "fail8.json",  "fail9.json"};

    // silence errors in the fail test
    for (auto &filename : testWithNULLResult) {
        Logger::isErrorSilenced = true;
        std::string src = "tests/JsonTests/official/fail/" + filename;
        Json::Json jsonObject(src.c_str(), true);
        Logger::isErrorSilenced = false;

        if (!jsonObject.getData().isNull()) {
            totalTestFails += 1;
            LOGGER_NOTE("Test Failed: Test with Null Result on file", filename);
        }
    }


    std::vector<std::string> testToPass = {"pass1.json", "pass2.json",
                                           "pass3.json"};
    for (auto &filename : testToPass) {
        std::string src = "tests/JsonTests/official/pass/" + filename;
        Json::Json jsonObject(src.c_str(), true);

        if (jsonObject.getData().isNull()) {
            totalTestFails += 1;
            LOGGER_NOTE("Test Failed: Pass Test on file", filename);
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
                LOGGER_NOTE("Test Failed: Number Array Json");
            }
        } else {
            totalTestFails += 1;
            LOGGER_NOTE("Test Failed: Number Array Json");
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
                LOGGER_NOTE("Test Failed: Double Array Json");
            }
        } else {
            totalTestFails += 1;
            LOGGER_NOTE("Test Failed: Double Array Json");
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
                LOGGER_NOTE("Test Failed: String Array Json");
            }
        } else {
            totalTestFails += 1;
            LOGGER_NOTE("Test Failed: String Array Json");
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
                LOGGER_NOTE("Test Failed: Bool Array Json");
            }
        } else {
            totalTestFails += 1;
            LOGGER_NOTE("Test Failed: Bool Array Json");
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
            LOGGER_NOTE("Test Failed: Object as Json");
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
                LOGGER_NOTE("Test Failed: Array Array Json");
            }
        } else {
            totalTestFails += 1;
            LOGGER_NOTE("Test Failed: Array Array Json");
        }
    }

    // Complex Json Object from Json File
    {
        // Read json from a file
        std::string rootDir = Utils::getCurrentDirectory();

        // Creating a json object from the json file
        std::string fileSrc = rootDir + "/tests/JsonTests/testData.json";
        Json::Json jsonObject(fileSrc.c_str(), true);

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
            LOGGER_NOTE("Test Failed: Object as Json");
        }
    }

    // Invalid Json
    {
        Logger::isErrorSilenced = true;

        std::string jsonValue = "{\"\"}";
        Json::Json jsonObject(jsonValue.c_str());

        Logger::isErrorSilenced = false;

        if (jsonObject.getData().getType() != JsonType::NULL_) {
            totalTestFails += 1;
            LOGGER_NOTE("Test Failed: Invalid Json");
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
                LOGGER_NOTE("Test Failed: Json Object Creation");
            }

        } catch (const std::exception &e) {
            totalTestFails += 1;
            LOGGER_NOTE(e.what());
            LOGGER_NOTE("Test Failed: Json Object Creation");
        }
    }

    // Try copy constructor
    {
        std::string jsonValue = "{\"number\": 100}";
        Json::Json jsonObject(jsonValue.c_str());

        Json::Json copiedObject = jsonObject;

        if (copiedObject.getData().getType() != JsonType::OBJECT &&
            copiedObject.getData().contains("number") &&
            copiedObject.getData().get("number").get<Json::Number>() != 100) {
            totalTestFails += 1;
            LOGGER_NOTE("Test Failed: Copy Constructor");
        }
    }

    // Try empty object
    {
        std::string jsonValue = "{\n   \n  \n \n}";
        Json::Json jsonObject(jsonValue.c_str());

        if(jsonObject.getData().isNull()){
            totalTestFails += 1;
            LOGGER_NOTE("Test Failed: Emtpy Object");
        }
    }

    LOGGER_NOTE("\nTotal Fails: " + std::to_string(totalTestFails));
}
