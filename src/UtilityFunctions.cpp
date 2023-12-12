#include "UtilityFunctions.h"
#include <sstream>

std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> elements;
    std::stringstream strStream(str);
    std::string item;
    while (std::getline(strStream, item, delimiter)) {
        elements.push_back(item);
    }
    return elements;
}

// Utility function to get child element text. Returns empty string if not found.
std::string getChildElementText(tinyxml2::XMLElement* parent, const std::string& childName) {
    if (parent) {
        tinyxml2::XMLElement* child = parent->FirstChildElement(childName.c_str());
        if (child && child->GetText()) {
            return child->GetText();
        }
    }
    return "";
}

