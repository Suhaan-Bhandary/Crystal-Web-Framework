#include "./tests.h"

#include "crystal.h"

int main() {
    LOGGER_NOTE("\n\nTesting Project");

    Test::TrimTests();
    Test::JsonTests();
    Test::HTMLTemplateTests();

    return 0;
}
