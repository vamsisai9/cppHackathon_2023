#ifndef YAMLPROCESSING_H
#define YAMLPROCESSING_H

#include <tinyxml2.h>
#include <yaml-cpp/yaml.h>
#include <iostream>

void processCompuRationalCoeffs(tinyxml2::XMLElement* compuScale, YAML::Node& scaleNode, const std::string& numeratorKey, const std::string& denominatorKey);
void processCompuScales(tinyxml2::XMLElement* computeElement, const std::string& scaleType, YAML::Node& computeNode);
void processAndPrintComputeMethodElements(const std::string& computeName, YAML::Node& signalNode);
void processAndPrintISignalElements(const std::string& iSignal, YAML::Node& signalNode);
void processISignalToPDUMappings(tinyxml2::XMLElement* parentElement, YAML::Node& mappingNode);
void processAndPrintISignalPDUElements(const std::string& pduRef, YAML::Node& mappingNode);
void processCANFrameTriggering(tinyxml2::XMLElement* canFrameTriggering, YAML::Node& canFramesNode);

#endif // YAMLPROCESSING_H
