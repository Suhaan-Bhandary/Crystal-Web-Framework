// The ifndef is used so that the same File is not loaded again
#ifndef INCLUDED_LOGGER
#define INCLUDED_LOGGER

#include <iostream>
#include <string>
class Logger {
   private:
    static std::string __filename;

   public:
    static bool isErrorSilenced;

    static void log(const std::string& message);

    template <typename... Args>
    static void log(const char* filename, const int& line, const Args&... args);

    template <typename... Args>
    static void logNote(const char* filename, const int& line,
                        const Args&... args);

    template <typename... Args>
    static void logWarn(const char* filename, const int& line,
                        const Args&... args);

    template <typename... Args>
    static void logError(const char* filename, const int& line,
                         const Args&... args);
};

// C++ Variadic Template has to be defined inside the .h file itself
// https://stackoverflow.com/questions/25091516/c-template-variadic-function-undefined-reference
const std::string blue("\033[0;34m");
const std::string yellow("\033[0;33m");
const std::string red("\033[0;31m");
const std::string reset("\033[0m");

template <typename... Args>
void Logger::log(const char* filename, const int& line, const Args&... args) {
    std::cout << "[" << filename << ":" << line << "] ";
    ((std::cout << args << " "), ...);
    std::cout << std::endl;
}

template <typename... Args>
void Logger::logNote(const char* filename, const int& line,
                     const Args&... args) {
    std::cout << blue;
    std::cout << "[" << filename << ":" << line << "] ";
    ((std::cout << args << " "), ...);
    std::cout << reset << std::endl;
}

template <typename... Args>
void Logger::logWarn(const char* filename, const int& line,
                     const Args&... args) {
    std::cout << yellow;
    std::cout << "[" << filename << ":" << line << "] ";
    ((std::cout << args << " "), ...);
    std::cout << reset << std::endl;
}

template <typename... Args>
void Logger::logError(const char* filename, const int& line,
                      const Args&... args) {
    if(isErrorSilenced) return;
    std::cout << red;
    std::cout << "[" << filename << ":" << line << "] ";
    ((std::cout << args << " "), ...);
    std::cout << reset << std::endl;
}

#ifdef DEVELOPMENT_ENVIRONMENT
#define LOGGER_MINIMAL(message) Logger::log(message)
#define LOGGER(...) Logger::log(__FILE__, __LINE__, __VA_ARGS__)
#define LOGGER_NOTE(...) Logger::logNote(__FILE__, __LINE__, __VA_ARGS__)
#define LOGGER_WARNING(...) Logger::logWarn(__FILE__, __LINE__, __VA_ARGS__)
#define LOGGER_ERROR(...) Logger::logError(__FILE__, __LINE__, __VA_ARGS__)
#else
#define LOGGER_MINIMAL(message)
#define LOGGER(...)
#define LOGGER_NOTE(...)
#define LOGGER_WARNING(...)
#define LOGGER_ERROR(...) Logger::logError(__FILE__, __LINE__, __VA_ARGS__)
#endif

#endif
