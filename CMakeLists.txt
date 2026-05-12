cmake_minimum_required(VERSION 3.20)
project(fools-gambit)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json for better IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(fools-gambit
    src/main.cpp
)

# Include Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

# Test executable
add_executable(fools-gambit-tests
    tests/test_main.cpp
)

target_link_libraries(fools-gambit-tests
    GTest::gtest_main
)

gtest_discover_tests(fools-gambit-tests)
