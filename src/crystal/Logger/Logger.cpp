#include "Logger.h"

#include <iostream>

void Logger::log(const std::string& message) {
    std::cout << message << std::endl;
}

// TODO: output file
std::string Logger::__filename = "";

// Variable to silence errors
bool Logger::isErrorSilenced = false;
