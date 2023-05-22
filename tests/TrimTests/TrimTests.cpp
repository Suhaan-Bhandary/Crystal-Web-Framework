#include <iostream>

#include "../../src/Logger/Logger.h"
#include "../tests.h"

void Test::TrimTests() {
    Logger::log("\n--------------------- Trim Tests ---------------------");

    int totalTestFails = 0;
    Logger::log("Total Fails: " + std::to_string(totalTestFails));
}