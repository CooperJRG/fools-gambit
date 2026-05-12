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
