#include "./HTMLTemplate.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

#include "../Logger/Logger.h"

// Constructor
http::HTMLTemplate::HTMLTemplate(const std::string &templatePath,
                                 const Json::Json &data) {
    compileHTMLFromTemplate(templatePath, data);
}

void http::HTMLTemplate::compileHTMLFromTemplate(
    const std::string &templatePath, const Json::Json &data) {
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
                                                const Json::Json &data) {
    std::string result = "";

    // TODO:
    // ! Currently we are just replacing the variables in the html and not a
    // ! proper template language
    int i = 0;
    while (i < HTMLContent.size()) {
        if (i + 1 != HTMLContent.size() && HTMLContent[i] == '{' &&
            HTMLContent[i + 1] == '{') {
            i += 2;

            std::string key = "";
            while (i + 1 < HTMLContent.size() &&
                   (HTMLContent[i] != '}' || HTMLContent[i + 1] != '}')) {
                key.push_back(HTMLContent[i++]);
            }

            // Check if string data is present or not
            try {
                result += data.data->getObjectValue(key)->getStringValue();
            } catch (const std::exception &e) {
                LOGGER_ERROR(e.what());
            }

            LOGGER_ERROR("(" + key + ")");
            i += 2;
        } else {
            result.push_back(HTMLContent[i++]);
        }
    }

    // !

    return result;
}

// Returns the compiled html
std::string http::HTMLTemplate::getHtml() { return compiledHTML; }