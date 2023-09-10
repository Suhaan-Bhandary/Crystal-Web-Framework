// The ifndef is used so that the same File is not loaded again
#ifndef INCLUDED_LOGGER
#define INCLUDED_LOGGER

#include <string>
class Logger {
   private:
    static std::string __filename;

   public:
    static void log(const std::string& message);

    static void log(const std::string& message, const char* filename, int line);

    static void logNote(const std::string& message, const char* filename,
                        int line);

    static void logWarn(const std::string& message, const char* filename,
                        int line);

    static void logError(const std::string& message, const char* filename,
                         int line);
};

#define LOGGER(message) Logger::log(message, __FILE__, __LINE__)
#define LOGGER_MINIMAL(message) Logger::log(message)
#define LOGGER_NOTE(message) Logger::logNote(message, __FILE__, __LINE__)
#define LOGGER_WARNING(message) Logger::logWarn(message, __FILE__, __LINE__)
#define LOGGER_ERROR(message) Logger::logError(message, __FILE__, __LINE__)

#endif