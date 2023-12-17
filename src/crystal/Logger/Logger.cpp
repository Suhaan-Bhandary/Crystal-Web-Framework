#include "Logger.h"

#include <iostream>

void Crystal::Logger::log(const std::string& message) {
    std::cout << message << std::endl;
}

// TODO: output file
std::string Crystal::Logger::__filename = "";

// Variable to silence errors
bool Crystal::Logger::isErrorSilenced = false;
