#include "./tests.h"

#include "../src/Logger/Logger.h"

int main() {
    LOGGER_NOTE("\n\nTesting Project");

    Test::TrimTests();
    Test::JsonTests();
    Test::HTMLTemplateTests();

    return 0;
}