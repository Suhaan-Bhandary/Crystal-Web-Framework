cmake_minimum_required(VERSION 3.22)

file(GLOB_RECURSE SRCS_EXAMPLE_APP "app/*.cpp")
file(GLOB_RECURSE HDRS_EXAMPLE_APP "app/*.h")

add_executable(CRYSTAL_HOME 
    ${SRCS_EXAMPLE_APP}
    ${HDRS_EXAMPLE_APP}
)

target_link_libraries(CRYSTAL_HOME PRIVATE CRYSTAL)
