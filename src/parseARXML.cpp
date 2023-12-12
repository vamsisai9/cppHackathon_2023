#include <tinyxml2.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <yaml-cpp/yaml.h>

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

std::vector<tinyxml2::XMLElement*> canFrameElements;
std::vector<tinyxml2::XMLElement*> IsignalPDUlements;
std::vector<std::string> iSignalPDUElementStrings;

std::vector<std::string> iSignalElementStrings;
std::vector<std::string> compuMethods;

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

void findAndSetTimePeriodValue(tinyxml2::XMLElement* parentElement, YAML::Node& mappingNode) {
    if (!parentElement) return;

    for (tinyxml2::XMLElement* element = parentElement->FirstChildElement(); element != nullptr; element = element->NextSiblingElement()) {
        if (std::string(element->Value()) == "TRANSMISSION-MODE-TRUE-TIMING") {
            tinyxml2::XMLElement* cyclicTimingElement = element->FirstChildElement("CYCLIC-TIMING");
            if (cyclicTimingElement) {
                tinyxml2::XMLElement* timePeriodElement = cyclicTimingElement->FirstChildElement("TIME-PERIOD");
                if (timePeriodElement) {
                    tinyxml2::XMLElement* valueElement = timePeriodElement->FirstChildElement("VALUE");
                    if (valueElement && valueElement->GetText()) {
                        mappingNode["TimePeriod"] = valueElement->GetText();
                        return; // Found and processed the element, exit the loop
                    } else {
                        std::cout << "VALUE element not found or is empty." << std::endl;
                    }
                } else {
                    std::cout << "TIME-PERIOD element not found under CYCLIC-TIMING." << std::endl;
                }
            } else {
                std::cout << "CYCLIC-TIMING element not found under TRANSMISSION-MODE-TRUE-TIMING." << std::endl;
            }
            break; // Exit loop after checking TRANSMISSION-MODE-TRUE-TIMING
        }
        // Recursively search in child elements
        findAndSetTimePeriodValue(element, mappingNode);
    }
}

YAML::Node processCompuValues(tinyxml2::XMLElement* parentElement, const std::string& valueType) {
    tinyxml2::XMLElement* valueElement = parentElement->FirstChildElement(valueType.c_str());
    YAML::Node valuesNode;
    if (valueElement) {
        for (tinyxml2::XMLElement* vElement = valueElement->FirstChildElement("V"); vElement != nullptr; vElement = vElement->NextSiblingElement("V")) {
            if (vElement->GetText()) {
                valuesNode.push_back(vElement->GetText());
            }
        }
    }
    return valuesNode;
}


void processCompuRationalCoeffs(tinyxml2::XMLElement* compuScale, YAML::Node& scaleNode, const std::string& numeratorKey, const std::string& denominatorKey) {
    tinyxml2::XMLElement* rationalCoeffsElement = compuScale->FirstChildElement("COMPU-RATIONAL-COEFFS");
    if (rationalCoeffsElement) {
        scaleNode[numeratorKey] = processCompuValues(rationalCoeffsElement, "COMPU-NUMERATOR");
        scaleNode[denominatorKey] = processCompuValues(rationalCoeffsElement, "COMPU-DENOMINATOR");
    }
}

void processCompuScales(tinyxml2::XMLElement* computeElement, const std::string& scaleType, YAML::Node& computeNode) {
    tinyxml2::XMLElement* compuScaleElement = computeElement->FirstChildElement(scaleType.c_str());
    if (compuScaleElement) {
        tinyxml2::XMLElement* compuScalesElement = compuScaleElement->FirstChildElement("COMPU-SCALES");
        if (compuScalesElement) {
            tinyxml2::XMLElement* compuScale = compuScalesElement->FirstChildElement("COMPU-SCALE");
            if (compuScale) {
                if (scaleType == "COMPU-INTERNAL-TO-PHYS") {
                    processCompuRationalCoeffs(compuScale, computeNode, "phy-to-int-numerator", "phy-to-int-denominator");
                } else if (scaleType == "COMPU-PHYS-TO-INTERNAL") {
                    processCompuRationalCoeffs(compuScale, computeNode, "int-to-phy-numerator", "int-to-phy-denominator");
                }
            }
        }
    } else {
        computeNode["phy-to-int-numerator"] = YAML::Node(YAML::NodeType::Null);
        computeNode["phy-to-int-denominator"] = YAML::Node(YAML::NodeType::Null);
        computeNode["int-to-phy-numerator"] = YAML::Node(YAML::NodeType::Null);
        computeNode["int-to-phy-denominator"] = YAML::Node(YAML::NodeType::Null);
    }
}


void processAndPrintComputeMethodElements(const std::string& computeName, YAML::Node& signalNode) {
    for (const auto& elementString : compuMethods) {
        tinyxml2::XMLDocument doc;
        doc.Parse(elementString.c_str());
        tinyxml2::XMLElement* computeElement = doc.RootElement();

        if (computeElement && std::string(computeElement->Value()) == "COMPU-METHOD") {
            std::string shortName = getChildElementText(computeElement, "SHORT-NAME");

            if (shortName == computeName) {
                YAML::Node computeNode;
                tinyxml2::XMLElement* categoryCompuElement = computeElement->FirstChildElement("CATEGORY");

                 if (categoryCompuElement && categoryCompuElement->GetText()) {
                    std::cout << "Category: " << categoryCompuElement->GetText() << std::endl;
                    computeNode["Category"] = categoryCompuElement->GetText();

                }
                processCompuScales(computeElement, "COMPU-INTERNAL-TO-PHYS", computeNode);

                processCompuScales(computeElement, "COMPU-PHYS-TO-INTERNAL", computeNode);

                computeNode["Range"] = {};
                signalNode["CompuMethod"] = computeNode;
            }
        } else {
            std::cout << "Error: Document could not be parsed or root element is not COMPU-METHOD." << std::endl;
        }
    }
}









void processAndPrintISignalElements(const std::string& iSignal, YAML::Node& signalNode) {
    for (const auto& elementString : iSignalElementStrings) {
        tinyxml2::XMLDocument doc;
        doc.Parse(elementString.c_str());
        tinyxml2::XMLElement* iSignalElement = doc.RootElement();

        if (iSignalElement && std::string(iSignalElement->Value()) == "I-SIGNAL") {
            std::string shortName = getChildElementText(iSignalElement, "SHORT-NAME");

            if (shortName == iSignal) {
                // Process DataTypePolicy

                tinyxml2::XMLElement* dataTypeElement = iSignalElement->FirstChildElement("DATA-TYPE-POLICY");
                if (dataTypeElement && dataTypeElement->GetText()) {
                    std::cout << "DataTypePolicy: " << dataTypeElement->GetText() << std::endl;
                    signalNode["DataTypePolicy"] = dataTypeElement->GetText();
                }

                // Process SignalLength
                tinyxml2::XMLElement* signalLengthElement = iSignalElement->FirstChildElement("LENGTH");
                if (signalLengthElement && signalLengthElement->GetText()) {
                    std::cout << "SignalLength: " << signalLengthElement->GetText() << std::endl;
                    signalNode["SignalLength"] = signalLengthElement->GetText();
                }

                // Process InitValue
                tinyxml2::XMLElement* initValueElement = iSignalElement->FirstChildElement("INIT-VALUE");
                if (initValueElement) {
                    tinyxml2::XMLElement* numericalValueSpecElement = initValueElement->FirstChildElement("NUMERICAL-VALUE-SPECIFICATION");
                    if (numericalValueSpecElement) {
                        tinyxml2::XMLElement* valueElement = numericalValueSpecElement->FirstChildElement("VALUE");
                        if (valueElement && valueElement->GetText()) {
                            std::cout << "InitValue: " << valueElement->GetText() << std::endl;
                            signalNode["InitValue"] = valueElement->GetText();
                        }
                    }
                }

                tinyxml2::XMLElement* nwRepresentationProps = iSignalElement->FirstChildElement("NETWORK-REPRESENTATION-PROPS");
                if (nwRepresentationProps) {
                    tinyxml2::XMLElement* swDataDefPropsVariants = nwRepresentationProps->FirstChildElement("SW-DATA-DEF-PROPS-VARIANTS");
                    if (swDataDefPropsVariants) {
                        tinyxml2::XMLElement* swDataDefPropsConditional = swDataDefPropsVariants->FirstChildElement("SW-DATA-DEF-PROPS-CONDITIONAL");
                        if (swDataDefPropsConditional) {
                            // Process BaseTypeRef
                            tinyxml2::XMLElement* baseTypeRefElement = swDataDefPropsConditional->FirstChildElement("BASE-TYPE-REF");
                            if (baseTypeRefElement && baseTypeRefElement->GetText()) {
                                std::string baseType = baseTypeRefElement->GetText();
                                // Extract the base type name (e.g., "uint16") from the path
                                std::string baseTypeName = baseType.substr(baseType.find_last_of('/') + 1);
                                std::cout << "BaseType: " << baseTypeName << std::endl;
                                signalNode["BaseType"] = baseTypeName;
                            }

                            // Process CompuMethodRef
                            tinyxml2::XMLElement* compuMethodRefElement = swDataDefPropsConditional->FirstChildElement("COMPU-METHOD-REF");
                            if (compuMethodRefElement && compuMethodRefElement->GetText()) {
                                std::string compuMethod = compuMethodRefElement->GetText();
                                // Extract the compu method name (e.g., "LongARq") from the path
                                std::string compuMethodName = compuMethod.substr(compuMethod.find_last_of('/') + 1);
                                std::cout << "CompuMethod: " << compuMethodName << std::endl;

                                processAndPrintComputeMethodElements(compuMethodName,signalNode);

                            }
                        }
                    }
                }
            }
        } else {
            std::cout << "Error: Document could not be parsed or root element is not I-SIGNAL." << std::endl;
        }
    }
}


void processISignalToPDUMappings1(tinyxml2::XMLElement* parentElement, YAML::Node& mappingNode) {

        if (!parentElement) return;

        for (tinyxml2::XMLElement* mappingElement = parentElement->FirstChildElement();
         mappingElement != nullptr;
         mappingElement = mappingElement->NextSiblingElement()) {
             std::cout << mappingElement->Value()<< std::endl;
         if (std::string(mappingElement->Value()) == "I-SIGNAL-TO-I-PDU-MAPPING") {

            std::string shortName = getChildElementText(mappingElement, "SHORT-NAME");
            std::string startPosition = getChildElementText(mappingElement, "START-POSITION");
            std::string transferProperty = getChildElementText(mappingElement, "TRANSFER-PROPERTY");
            std::string packingByteOrder = getChildElementText(mappingElement, "PACKING-BYTE-ORDER");

            std::string iSignalId = getChildElementText(mappingElement, "I-SIGNAL-REF");

            YAML::Node signalNode;

            signalNode["StartPosition"] = startPosition.empty() ? 0 : std::stoi(startPosition);  // Assuming it's an integer
            signalNode["TransferProperty"] = transferProperty;
            signalNode["PackingByteOrder"] = packingByteOrder;

            size_t lastSlashSignal = iSignalId.find_last_of('/');

            if (lastSlashSignal != std::string::npos && lastSlashSignal + 1 < iSignalId.size()) {
                    std::string extractedText = iSignalId.substr(lastSlashSignal + 1);
                    signalNode["name"] = extractedText;
                    processAndPrintISignalElements(extractedText,signalNode);
                }

            // Create a new YAML node for this mapping and populate it


              // Add more fields as needed...

            // Add this signal node to the mappings node
            mappingNode["Signals"].push_back(signalNode);
         }


    }
}
void processISignalToPDUMappings(tinyxml2::XMLElement* parentElement, YAML::Node& mappingNode) {
    if (!parentElement) return;

    for (tinyxml2::XMLElement* mappingElement = parentElement->FirstChildElement();
         mappingElement != nullptr;
         mappingElement = mappingElement->NextSiblingElement()) {
             std::cout << mappingElement->Value()<< std::endl;
         if (std::string(mappingElement->Value()) == "I-SIGNAL-TO-PDU-MAPPINGS") {

            std::cout << "PDU LENGTH: " << mappingElement->Value() << std::endl;
                processISignalToPDUMappings1(mappingElement, mappingNode);

         }


    }
}


void processAndPrintISignalPDUElements(const std::string& pduRef, YAML::Node& mappingNode) {
    for (const auto& elementString : iSignalPDUElementStrings) {
        tinyxml2::XMLDocument doc;
        doc.Parse(elementString.c_str());
        tinyxml2::XMLElement* iSignalElement = doc.RootElement();

        if (iSignalElement && std::string(iSignalElement->Value()) == "I-SIGNAL-I-PDU") {
            std::string shortName = getChildElementText(iSignalElement, "SHORT-NAME");

            if (shortName == pduRef) {
                tinyxml2::XMLElement* pduLengthElement = iSignalElement->FirstChildElement("LENGTH");
                if (pduLengthElement && pduLengthElement->GetText()) {
                    std::cout << "PDU LENGTH: " << pduLengthElement->GetText() << std::endl;
                    mappingNode["PduLength"] = pduLengthElement->GetText();
                }

                // Use the helper function to find and set the time period value
                findAndSetTimePeriodValue(iSignalElement, mappingNode);
                processISignalToPDUMappings(iSignalElement, mappingNode);
            }
        } else {
            std::cout << "Error: Document could not be parsed or root element is not I-SIGNAL-I-PDU." << std::endl;
        }
    }
}



void processPDUTOFRAMEMAPPING(tinyxml2::XMLElement* mappingElement, YAML::Node& mappingNode) {
    if (!mappingElement) {
        return;
    }

    tinyxml2::XMLElement* shortNameElement = mappingElement->FirstChildElement("SHORT-NAME");
    tinyxml2::XMLElement* byteOrderElement = mappingElement->FirstChildElement("PACKING-BYTE-ORDER");
    tinyxml2::XMLElement* pduRefElement = mappingElement->FirstChildElement("PDU-REF");
    tinyxml2::XMLElement* startPositionElement = mappingElement->FirstChildElement("START-POSITION");

    if (shortNameElement && shortNameElement->GetText()) {
        mappingNode["name"] = shortNameElement->GetText();
    }
    if (byteOrderElement && byteOrderElement->GetText()) {
        mappingNode["PackingByteOrder"] = byteOrderElement->GetText();
    }
    if (pduRefElement && pduRefElement->GetText()) {
        std::string pduRefText = pduRefElement->GetText();
        size_t lastSlashPos = pduRefText.find_last_of('/');
        if (lastSlashPos != std::string::npos && lastSlashPos + 1 < pduRefText.size()) {
            std::string extractedText = pduRefText.substr(lastSlashPos + 1);

            processAndPrintISignalPDUElements(extractedText,mappingNode);

        }
    }

    if (startPositionElement && startPositionElement->GetText()) {
        mappingNode["StartPosition"] = startPositionElement->GetText();
    }
}

void printCANFrameWithShortName(const std::string& shortName, YAML::Node& frameNode) {
    for (tinyxml2::XMLElement* frameElement : canFrameElements) {
        tinyxml2::XMLElement* shortNameElement = frameElement->FirstChildElement("SHORT-NAME");
        if (shortNameElement && shortNameElement->GetText() && shortName == shortNameElement->GetText()) {
            // Process FRAME-LENGTH
            tinyxml2::XMLElement* frameLengthElement = frameElement->FirstChildElement("FRAME-LENGTH");
            if (frameLengthElement && frameLengthElement->GetText()) {
                frameNode["FrameLength"] = frameLengthElement->GetText();
            }

            // Process each PDU-TO-FRAME-MAPPING
            tinyxml2::XMLElement* mappingsElement = frameElement->FirstChildElement("PDU-TO-FRAME-MAPPINGS");
            if (mappingsElement) {
                for (tinyxml2::XMLElement* mappingElement = mappingsElement->FirstChildElement("PDU-TO-FRAME-MAPPING");
                     mappingElement != nullptr;
                     mappingElement = mappingElement->NextSiblingElement("PDU-TO-FRAME-MAPPING")) {

                    YAML::Node mappingNode;
                    processPDUTOFRAMEMAPPING(mappingElement, mappingNode);
                    frameNode["PDUs"].push_back(mappingNode);
                }
            }

            break; // Assuming only one match is needed
        }
    }
}


void processCANFrameTriggering(tinyxml2::XMLElement* canFrameTriggering, YAML::Node& canFramesNode) {
    if (!canFrameTriggering) {
        return;
    }

    YAML::Node frameNode;

    tinyxml2::XMLElement* identifierElement = canFrameTriggering->FirstChildElement("IDENTIFIER");
    tinyxml2::XMLElement* shortNameElement = canFrameTriggering->FirstChildElement("SHORT-NAME");
    tinyxml2::XMLElement* canAddressingModeElement = canFrameTriggering->FirstChildElement("CAN-ADDRESSING-MODE");



    if (shortNameElement && shortNameElement->GetText()) {
        std::vector<std::string> parts = splitString(shortNameElement->GetText(), '_');
        if (parts.size() >= 2) {
            std::cout << "SHORT-NAME: " << parts[1] << std::endl;
            printCANFrameWithShortName(parts[1],frameNode);
            frameNode["name"] = parts[1];
        }
    }
    if (identifierElement && identifierElement->GetText()) {
        std::cout << "IDENTIFIER: " << identifierElement->GetText() << std::endl;
         frameNode["FrameID"] = identifierElement->GetText();
    }

    if (canAddressingModeElement && canAddressingModeElement->GetText()) {
        std::cout << "IDENTIFIER: " << canAddressingModeElement->GetText() << std::endl;
         frameNode["AddressingMode"] = canAddressingModeElement->GetText();
    }


     canFramesNode.push_back(frameNode);



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
