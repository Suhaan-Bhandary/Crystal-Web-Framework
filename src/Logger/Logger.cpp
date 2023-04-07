#include "Logger.h"

#include <iostream>

void Logger::log(const std::string &message) { std::cout << message << "\n"; }

void Logger::log(const std::string &message, const std::string &filename) {
    std::cout << message << " : " << filename << "\n";
}

// TODO: output file
std::string Logger::__filename = "";