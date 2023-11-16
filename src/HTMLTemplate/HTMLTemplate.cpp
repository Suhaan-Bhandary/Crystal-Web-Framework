#include "./HTMLTemplate.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

#include "../Logger/Logger.h"

// Constructor
http::HTMLTemplate::HTMLTemplate(const std::string &templatePath,
                                 Json::Json &data) {
    compileHTMLFromTemplate(templatePath, data);
}

void http::HTMLTemplate::compileHTMLFromTemplate(
    const std::string &templatePath, Json::Json &data) {
    // Read HTML Template File
    std::string HTMLContent = readHTMLFromFile(templatePath);

    // compile the Template
    compiledHTML = compileHTMLText(HTMLContent, data);
}

std::string http::HTMLTemplate::readHTMLFromFile(
    const std::string &templatePath) {
    std::ifstream HTMLFile(templatePath);
    if (!HTMLFile.is_open()) {
        LOGGER_ERROR("Error in opening file | readHTMLFromFile!!");
        throw std::invalid_argument(
            "Error in opening file | readHTMLFromFile!!");
    }

    std::string HTMLContent((std::istreambuf_iterator<char>(HTMLFile)),
                            (std::istreambuf_iterator<char>()));

    // close the file
    HTMLFile.close();

    return HTMLContent;
}

// TODO: Implement HTML compile
std::string http::HTMLTemplate::compileHTMLText(const std::string &HTMLContent,
                                                Json::Json &data) {
    std::string result = "";

    // TODO:
    // ! Currently we are just replacing the variables in the html and not a
    // ! proper template language
    int i = 0;
    while (i < HTMLContent.size()) {
        // \{{ this will ignore the {{
        if (i + 2 < HTMLContent.size() && HTMLContent.substr(i, 3) == "\\{{") {
            result += "{{";
            i += 3;
        } else if (i + 1 != HTMLContent.size() &&
                   HTMLContent.substr(i, 2) == "{{") {
            i += 2;

            std::string key = "";
            while (i + 1 < HTMLContent.size() &&
                   HTMLContent.substr(i, 2) != "}}") {
                key.push_back(HTMLContent[i++]);
            }
            i += 2;

            // Check if string data is present or not
            try {
                result += data.getData().get(key).get<Json::String>();
            } catch (const std::exception &e) {
                LOGGER_ERROR(key);
                LOGGER_ERROR(e.what());
            }

        } else {
            result.push_back(HTMLContent[i++]);
        }
    }

    // !

    return result;
}

// Returns the compiled html
std::string http::HTMLTemplate::getHtml() { return compiledHTML; }