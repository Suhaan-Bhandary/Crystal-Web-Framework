// The ifndef is used so that the same File is not loaded again
#ifndef INCLUDED_Response
#define INCLUDED_Response

#include <string>
#include <unordered_map>

#include "../Json/Json.h"

namespace Crystal {

class Cookie {
   public:
    // Required Data
    std::string name, value;

    // --- Optional Data ---

    // Indicates the maximum lifetime of the cookie as an HTTP-date timestamp.
    std::string expires = "";

    // Indicates the number of "seconds" until the cookie expires.
    // A zero or negative number will expire the cookie immediately. If both
    // Expires and Max-Age are set, Max-Age has precedence.
    int maxAge = 0;

    // defines the host to which the cookie will be sent
    std::string domain = "";

    // Forbids JavaScript from accessing the cookie, eg: Document.cookie
    bool isHttpOnly = false;

    // Used to determine if it is only to be send by https
    bool isSecure = false;

    // Partitioned [Experimental]
    bool isPartitioned = false;

    // Path
    std::string path = "";

    // Same Site can have values ["Strict", "Lax", "None"]
    std::string sameSite = "Lax";

    // Methods
    Cookie(const std::string &name, const std::string &value,
           const std::string &expires, int maxAge, const std::string &domain,
           bool isHttpOnly, bool isSecure, bool isPartitioned,
           const std::string &path, const std::string &sameSite);

    std::string getHeader();
};

class Response {
    int client_socket;
    int statusCode = 200;  // Default status code is 200
    const static std::unordered_map<int, std::string> statusCodes;
    std::string eTag = "";

    // Cookies data
    std::unordered_map<std::string, Cookie> cookieData;

    // Function returns the cookies header using cookie data
    std::string getCookiesHeader();

   public:
    Response(int client_socket);
    void sendHTML(const std::string &html);
    void send404();
    void sendTemplate(const std::string &templateName, Json::Json &data);
    void sendPublicFile(const std::string &relativePathToPublic);
    void sendJson(const std::string &jsonString);
    void sendResponse(const std::string &response_body,
                      const std::string &response_type);

    void setStatusCode(int status);
    void redirect(const std::string &redirect_url);

    // Cookie functions
    void setCookie(const std::string &name, const std::string &value,
                   const std::string &expires, const std::string &domain = "",
                   bool isHttpOnly = false, bool isSecure = false,
                   bool isPartitioned = false, const std::string &path = "",
                   const std::string &sameSite = "");

    void setCookie(const std::string &name, const std::string &value,
                   int maxAgeInSeconds, const std::string &domain = "",
                   bool isHttpOnly = false, bool isSecure = false,
                   bool isPartitioned = false, const std::string &path = "",
                   const std::string &sameSite = "");

    void clearCookie(const std::string &name);

    // Etag
    std::string getETagHeader();
    void setETag(std::string);
};
}  // namespace Crystal

#endif