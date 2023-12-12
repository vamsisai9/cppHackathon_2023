cmake_minimum_required(VERSION 3.10)
project(cPPHackathon)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories for the entire project
include_directories(${CMAKE_SOURCE_DIR}/inc)
include_directories(${CMAKE_SOURCE_DIR}/tinyxml2)

# Add the yaml-cpp subdirectory
add_subdirectory(yaml-cpp)

# Include Google Test
add_subdirectory(googletest)

# Add the src subdirectory
add_subdirectory(src)

# Enable testing
enable_testing()

# Add the tests subdirectory
add_subdirectory(tests)
