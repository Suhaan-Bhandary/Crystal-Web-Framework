// The ifndef is used so that the same File is not loaded again
#ifndef INCLUDED_HTMLTemplate
#define INCLUDED_HTMLTemplate

#include <string>
#include <unordered_map>

#include "../Json/Json.h"

namespace http {
class HTMLTemplate {
    // Compiled html will be stored in this variable
    std::string compiledHTML;

    // Function to compile html
    void compileHTMLFromTemplate(const std::string &templatePath,
                                 Json::Json &data);

    // Function to read html from file
    std::string readHTMLFromFile(const std::string &templatePath);

    // Function to compile the html text
    std::string compileHTMLText(const std::string &HTMLContent,
                                Json::Json &data);

   public:
    HTMLTemplate(const std::string &templatePath, Json::Json &data);

    // Function returns the compiled html file from the template
    std::string getHtml();
};
}  // namespace http

#endif