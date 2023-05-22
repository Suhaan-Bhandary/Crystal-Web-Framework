#include "./tests.h"

#include "../src/Logger/Logger.h"

int main() {
    Logger::log("\n\nTesting Project");

    Test::JsonTests();
    Test::TrimTests();

    return 0;
}