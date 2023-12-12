# Define the source files for the executable
set(SOURCE_FILES
    Main.cpp
    UtilityFunctions.cpp
    XMLProcessing.cpp
    YAMLProcessing.cpp
    globals.cpp
    ../tinyxml2/tinyxml2.cpp)  # Adjust the path to tinyxml2.cpp if necessary

# Add the executable
add_executable(cPPHackathon ${SOURCE_FILES})

# Link against yaml-cpp if required
target_link_libraries(cPPHackathon PRIVATE yaml-cpp)
