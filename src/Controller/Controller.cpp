#include "Controller.h"

void Controller::getHome(http::Request &request, http::Response &response) {
    // Send html response to the client
    std::string htmlContent =
        "<html>"
        "<head><title>Home Page</title></head>"
        "<body>"
        "<h1>Home Page!</h1>"
        "<p>HTTP Server made with C++</p>"
        "</body>"
        "</html>\r\n";

    response.sendHTML(htmlContent);
}

void Controller::getNotFound(http::Request &request, http::Response &response) {
    std::string homePageLink = "/";

    // Send html response to the client
    std::string htmlContent =
        "<html>"
        "<head><title>Page Not Found</title></head>"
        "<body>"
        "<h1>404 Error: Page not Found!!</h1>"
        "<a href=\"" +
        homePageLink +
        "\">Home Page</a>"
        "</body>"
        "</html>\r\n";

    response.sendHTML(htmlContent);
}
