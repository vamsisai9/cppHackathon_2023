# Assuming this is tests/CMakeLists.txt


set(SOURCE_FILES1
     test_main.cpp
    ../tinyxml2/tinyxml2.cpp)  # Adjust the path to tinyxml2.cpp if necessary

# Add the executable
add_executable(cPPHackathon_test1 ${SOURCE_FILES1})
# Link the test executable with Google Test and other necessary libraries
target_link_libraries(cPPHackathon_test1 gtest gtest_main yaml-cpp)

# Register the test executable with CTest
add_test(NAME cPPHackathon_test COMMAND cPPHackathon_test1)
