cmake_minimum_required(VERSION 3.22)

project(CRYSTAL)

# Setting C++ version
set (CMAKE_CXX_STANDARD 17)

# Adding Project files
file(GLOB_RECURSE SRCS "src/*.cpp")
file(GLOB_RECURSE HDRS "src/*.h")

add_library(${PROJECT_NAME} 
    ${SRCS} 
    ${HDRS} 
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src)


# Only build the example if Crystal is directly called add tests and examples else exclude
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    # Building example
    add_subdirectory(example)

    # Building test
    enable_testing()
    add_subdirectory(tests)
endif()

# Define Build type if not present
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()


# set(WFLAGS "-Wall -Wextra -Werror -Wshadow")
set(CMAKE_CXX_FLAGS "-DFREQUENT_RESTART")
set(CMAKE_CXX_FLAGS_DEBUG "${WFLAGS} -Og -g -DDEVELOPMENT_ENVIRONMENT")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Adding SQLite3 library
find_package(SQLite3 REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${SQLite3_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PRIVATE ${SQLite3_LIBRARY_DIR})
target_link_libraries(${PROJECT_NAME} ${SQLite3_LIBRARIES})


