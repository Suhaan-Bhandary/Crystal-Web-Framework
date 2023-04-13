// The ifndef is used so that the same File is not loaded again
#ifndef INCLUDED_LOGGER
#define INCLUDED_LOGGER

#include <string>
class Logger {
   private:
    static std::string __filename;

   public:
    static void log(const std::string &message);
    static void log(const std::string &message, const std::string &filename);
};

#endif