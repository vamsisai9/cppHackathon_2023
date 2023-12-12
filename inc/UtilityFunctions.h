#ifndef UTILITYFUNCTIONS_H
#define UTILITYFUNCTIONS_H

#include <tinyxml2.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>

std::vector<std::string> splitString(const std::string& str, char delimiter);
std::string getChildElementText(tinyxml2::XMLElement* parent, const std::string& childName);

#endif // UTILITYFUNCTIONS_H
