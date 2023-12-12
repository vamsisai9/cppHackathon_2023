#include "XMLProcessing.h"
#include "YAMLProcessing.h"
#include <iostream>

// Ensure that parseARXML is declared in one of the included headers.
// If not, declare it appropriately.

int main(int argc, char* argv[]) {
    std::string arxmlPath;

    if (argc > 1) {
        // The first command-line argument is taken as the path to the ARXML file
        arxmlPath = argv[1];
    } else {
        std::cerr << "Usage: " << argv[0] << " <path_to_input.arxml>" << std::endl;
        return 1;  // Exit if no path is provided
    }

    parseARXML(arxmlPath);  // Call your parsing function with the provided path
    return 0;
}
