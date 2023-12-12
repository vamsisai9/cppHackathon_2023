#ifndef XMLPROCESSING_H
#define XMLPROCESSING_H

#include <tinyxml2.h>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>

void storeISignalIPDUSubtree(tinyxml2::XMLElement* element);
void storeISignalSubtree(tinyxml2::XMLElement* element);
void storecompuMethodsSubtree(tinyxml2::XMLElement* element);
void printXMLElements(const std::vector<tinyxml2::XMLElement*>& elements);
void findCANFrameBehavior(tinyxml2::XMLElement* element, YAML::Node& rootYAML);
void parseARXML(const std::string& arxmlFile);

extern std::vector<tinyxml2::XMLElement*> canFrameElements;
extern std::vector<tinyxml2::XMLElement*> IsignalPDUlements;
extern std::vector<std::string> iSignalPDUElementStrings;
extern std::vector<std::string> iSignalElementStrings;
extern std::vector<std::string> compuMethods;

#endif // XMLPROCESSING_H
