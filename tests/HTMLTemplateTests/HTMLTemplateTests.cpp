#include <fstream>

#include "../tests.h"

Json::Json getTestJsonData() {
    // First read json data for the template
    // Read json from a file
    std::string rootDir = Utils::getCurrentDirectory();
    std::ifstream JsonFile(rootDir +
                           "/tests/HTMLTemplateTests/sampleTemplateJson.json");

    if (!JsonFile.is_open()) {
        LOGGER_ERROR("Error in opening file!!");

        Json::Json data("{}");
        return data;
    }

    std::string jsonValue((std::istreambuf_iterator<char>(JsonFile)),
                          (std::istreambuf_iterator<char>()));

    // close the file
    JsonFile.close();

    // Creating a json object from the json file
    Json::Json data(jsonValue);
    return data;
}

void Test::HTMLTemplateTests() {
    LOGGER_MINIMAL("\n----------------- HTML Template Tests -----------------");
    int totalTestFails = 0;

    // TODO: Write test
    {
        // Read the file
        std::string rootDir = Utils::getCurrentDirectory();
        std::string templateURL =
            rootDir + "/tests/HTMLTemplateTests/sampleTemplate.html";

        // Read json data and pass to the template
        Json::Json data = getTestJsonData();
        http::HTMLTemplate html_template(templateURL, data);

        // Get the compiled html
        std::string HTMLContent = html_template.getHtml();

        // Print json data
        // LOGGER("Json Data");
        // LOGGER(data.getData().toString());

        // TODO: Check if the html is compiled
        // LOGGER("\n\nHtml Content");
        // LOGGER(HTMLContent);
    }

    LOGGER_NOTE("Total Fails: " + std::to_string(totalTestFails));
}