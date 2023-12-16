cmake_minimum_required(VERSION 3.22)

add_executable(tester 
    tests.cpp 
    tests.h 
    HTMLTemplateTests/HTMLTemplateTests.cpp 
    JsonTests/JsonTests.cpp 
    TrimTests/TrimTests.cpp
)

target_link_libraries(tester PRIVATE CRYSTAL)

add_test(NAME CRYSTAL.test COMMAND tester)
