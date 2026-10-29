cmake_minimum_required(VERSION 3.16)

project(libdsa VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Library
add_library(dsa STATIC
    src/vector.c
)

target_include_directories(dsa PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Testing
include(CTest)

if(BUILD_TESTING)
    add_executable(test_vector
        tests/test_vector.c
    )

    target_link_libraries(test_vector PRIVATE dsa)

    add_test(
        NAME vector_tests
        COMMAND test_vector
    )
endif()