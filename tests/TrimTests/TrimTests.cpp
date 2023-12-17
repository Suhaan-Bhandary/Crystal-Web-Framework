#include "../tests.h"

void Test::TrimTests() {
    LOGGER_MINIMAL("\n--------------------- Trim Tests ---------------------");

    int totalTestFails = 0;

    // If no trim is required return same string
    if (Crystal::Utils::trim("test this") != "test this") {
        totalTestFails += 1;
        LOGGER_ERROR("Test Failed: No Trim");
    }

    // Empty Trim
    if (Crystal::Utils::trim("") != "") {
        totalTestFails += 1;
        LOGGER_ERROR("Test Failed: Empty Trim");
    }

    // Full Trim
    if (Crystal::Utils::trim("    \n\n\r") != "") {
        totalTestFails += 1;
        LOGGER_ERROR("Test Failed: Full Trim");
    }

    // Left Trim
    if (Crystal::Utils::trim("    test this") != "test this") {
        totalTestFails += 1;
        LOGGER_ERROR("Test Failed: Left Trim");
    }

    // Right Trim
    if (Crystal::Utils::trim("test this    ") != "test this") {
        totalTestFails += 1;
        LOGGER_ERROR("Test Failed: Right Trim");
    }

    // Both Trim
    if (Crystal::Utils::trim("    test this    ") != "test this") {
        totalTestFails += 1;
        LOGGER_ERROR("Test Failed: Both Trim");
    }

    LOGGER_NOTE("Total Fails: " + std::to_string(totalTestFails));
}