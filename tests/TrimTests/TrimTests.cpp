#include "../../src/Logger/Logger.h"
#include "../../src/Utils/Utils.h"
#include "../tests.h"

void Test::TrimTests() {
    Logger::log("\n--------------------- Trim Tests ---------------------");

    int totalTestFails = 0;

    // If no trim is required return same string
    if (Utils::trim("test this") != "test this") {
        totalTestFails += 1;
        Logger::log("Test Failed: No Trim");
    }

    // Empty Trim
    if (Utils::trim("") != "") {
        totalTestFails += 1;
        Logger::log("Test Failed: Empty Trim");
    }

    // Full Trim
    if (Utils::trim("    \n\n\r") != "") {
        totalTestFails += 1;
        Logger::log("Test Failed: Full Trim");
    }

    // Left Trim
    if (Utils::trim("    test this") != "test this") {
        totalTestFails += 1;
        Logger::log("Test Failed: Left Trim");
    }

    // Right Trim
    if (Utils::trim("test this    ") != "test this") {
        totalTestFails += 1;
        Logger::log("Test Failed: Right Trim");
    }

    // Both Trim
    if (Utils::trim("    test this    ") != "test this") {
        totalTestFails += 1;
        Logger::log("Test Failed: Both Trim");
    }

    Logger::log("Total Fails: " + std::to_string(totalTestFails));
}