cmake_minimum_required(VERSION 3.16)

project(libdsa VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Library
add_library(dsa STATIC
    src/vector.c
    src/linkedlist.c
    src/stack.c
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

    add_executable(test_linkedlist
        tests/test_linkedlist.c
    )

    target_link_libraries(test_linkedlist PRIVATE dsa)

    add_test(
        NAME linkedlist_tests
        COMMAND test_linkedlist
    )

    add_executable(test_stack tests/test_stack.c)
    target_link_libraries(test_stack PRIVATE dsa)
    add_test(NAME stack_tests COMMAND test_stack)

    add_custom_target(check
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -C $<CONFIG>
        DEPENDS test_vector test_linkedlist test_stack
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        USES_TERMINAL
    )
endif()
