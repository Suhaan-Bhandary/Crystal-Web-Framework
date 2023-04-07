#include "Logger.h"

#include <iostream>

void Logger::log(std::string message) { std::cout << message << "\n"; }

void Logger::log(std::string message, std::string filename) {
    std::cout << message << " : " << filename << "\n";
}

// TODO: output file
std::string Logger::__filename = "";