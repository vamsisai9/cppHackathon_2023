#include "XMLProcessing.h"
#include "UtilityFunctions.h"
#include "YAMLProcessing.h"


// Implementations of storeISignalIPDUSubtree, storeISignalSubtree, etc.
// Implementations of printXMLElements and findCANFrameBehavior
void storeISignalIPDUSubtree(tinyxml2::XMLElement* element) {
    tinyxml2::XMLPrinter printer;
    element->Accept(&printer); // Visit this element and all of its children
    iSignalPDUElementStrings.push_back(printer.CStr());
}

void storeISignalSubtree(tinyxml2::XMLElement* element) {
    tinyxml2::XMLPrinter printer;
    element->Accept(&printer); // Visit this element and all of its children
    iSignalElementStrings.push_back(printer.CStr());
}

void storecompuMethodsSubtree(tinyxml2::XMLElement* element) {
    tinyxml2::XMLPrinter printer;
    element->Accept(&printer); // Visit this element and all of its children
    compuMethods.push_back(printer.CStr());
}

void printXMLElements(const std::vector<tinyxml2::XMLElement*>& elements) {
    for (const auto& element : elements) {
        if (element) {
            // Print the name of the element
            std::cout << "Element Name: " << element->Value() << std::endl;

            // Print the text content of the element, if any
            if (element->GetText()) {
                std::cout << "  Text: " << element->GetText() << std::endl;
            }

            // Optionally, iterate through and print child elements
            for (tinyxml2::XMLElement* child = element->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
                std::cout << "  Child Element: " << child->Value();
                if (child->GetText()) {
                    std::cout << ", Content: " << child->GetText();
                }
                std::cout << std::endl;
            }
        }
    }
}


void findCANFrameBehavior(tinyxml2::XMLElement* element, YAML::Node& rootYAML) {
    if (!element) {
        return;
    }

    if (std::string(element->Value()) == "CAN-FRAME-TRIGGERING") {

        YAML::Node canFramesNode = rootYAML["CanFrames"];
        processCANFrameTriggering(element,canFramesNode);


    }

    if (std::string(element->Value()) == "CAN-FRAME") {
        canFrameElements.push_back(element);

    }

    if (std::string(element->Value()) == "I-SIGNAL-I-PDU") {
        storeISignalIPDUSubtree(element);

    }

    if (std::string(element->Value()) == "I-SIGNAL") {
        storeISignalSubtree(element);

    }

    if (std::string(element->Value()) == "COMPU-METHOD") {
        storecompuMethodsSubtree(element);

    }

    if (std::string(element->Value()) == "CAN-FRAME-RX-BEHAVIOR" || std::string(element->Value()) == "CAN-FRAME-TX-BEHAVIOR") {
        // Optionally process or print something for these elements
    }

    // Recursively check all child elements
    for (tinyxml2::XMLElement* child = element->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()) {
        findCANFrameBehavior(child,rootYAML);
    }



}


void parseARXML(const std::string& arxmlFile) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(arxmlFile.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error loading file: " << arxmlFile << std::endl;
        return;
    }

    tinyxml2::XMLElement* root = doc.RootElement();
    if (!root) {
        std::cerr << "No root element found in the file." << std::endl;
        return;
    }

    YAML::Node rootYAML;

    findCANFrameBehavior(root,rootYAML);

     // Output to see the result
        std::cout << rootYAML << std::endl;
        std::ofstream fout("output.yaml");
        fout << rootYAML;
}

