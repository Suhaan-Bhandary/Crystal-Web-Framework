#include "Logger.h"

#include <iostream>

const std::string blue("\033[0;34m");
const std::string yellow("\033[0;33m");
const std::string red("\033[0;31m");
const std::string reset("\033[0m");

void Logger::log(const std::string& message) {
    std::cout << message << std::endl;
}

void Logger::log(const std::string& message, const char* filename, int line) {
    std::cout << "[" << filename << ":" << line << "] " << message << std::endl;
}

void Logger::logNote(const std::string& message, const char* filename,
                     int line) {
    std::cout << blue;
    std::cout << "[" << filename << ":" << line << "] " << message;
    std::cout << reset << std::endl;
}

void Logger::logWarn(const std::string& message, const char* filename,
                     int line) {
    std::cout << yellow;
    std::cout << "[" << filename << ":" << line << "] " << message;
    std::cout << reset << std::endl;
}

void Logger::logError(const std::string& message, const char* filename,
                      int line) {
    std::cout << red;
    std::cout << "[" << filename << ":" << line << "] " << message;
    std::cout << reset << std::endl;
}

// TODO: output file
std::string Logger::__filename = "";