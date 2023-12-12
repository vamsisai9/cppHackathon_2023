#include <gtest/gtest.h>
#include <tinyxml2.h>
#include <yaml-cpp/yaml.h>
#include <string>
#include "UtilityFunctions.h"
#include <sstream>



// Function to be tested
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

// Test Fixture for setting up XML elements
class ProcessCompuValuesTest : public ::testing::Test {
protected:
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* parentElement;

    void SetUp() override {
        parentElement = doc.NewElement("COMPU-NUMERATOR");
        doc.InsertFirstChild(parentElement);
    }
};

// Test Case
TEST_F(ProcessCompuValuesTest, ReturnsCorrectValues) {
    auto* vElement1 = doc.NewElement("V");
    vElement1->SetText("0");
    parentElement->InsertEndChild(vElement1);

    auto* vElement2 = doc.NewElement("V");
    vElement2->SetText("1");
    parentElement->InsertEndChild(vElement2);

    YAML::Node result = processCompuValues(parentElement, "V");

    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].as<std::string>(), "0");
    EXPECT_EQ(result[1].as<std::string>(), "1");
}

// Test Fixture for utility functions
class UtilityFunctionTests : public ::testing::Test {
protected:
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* parentElement;

    void SetUp() override {
        parentElement = doc.NewElement("Parent");
        doc.InsertFirstChild(parentElement);
    }
};

// Test Cases for splitString
TEST_F(UtilityFunctionTests, SplitStringBasic) {
    std::vector<std::string> result = splitString("one,two,three", ',');
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "one");
    EXPECT_EQ(result[1], "two");
    EXPECT_EQ(result[2], "three");
}

TEST_F(UtilityFunctionTests, SplitStringNoDelimiter) {
    std::vector<std::string> result = splitString("singleword", ',');
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0], "singleword");
}

TEST_F(UtilityFunctionTests, SplitStringConsecutiveDelimiters) {
    std::vector<std::string> result = splitString("one,,two", ',');
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "one");
    EXPECT_EQ(result[1], "");
    EXPECT_EQ(result[2], "two");
}

TEST_F(UtilityFunctionTests, SplitStringDelimiterStartEnd) {
    std::vector<std::string> result = splitString(",one,two,", ',');
    ASSERT_EQ(result.size(), 4);
    EXPECT_EQ(result[0], "");
    EXPECT_EQ(result[1], "one");
    EXPECT_EQ(result[2], "two");
    EXPECT_EQ(result[3], "");
}

TEST_F(UtilityFunctionTests, SplitStringEmpty) {
    std::vector<std::string> result = splitString("", ',');
    ASSERT_TRUE(result.empty());
}

// Test Cases for getChildElementText
TEST_F(UtilityFunctionTests, GetChildElementTextExisting) {
    auto* child = doc.NewElement("Child");
    child->SetText("Hello");
    parentElement->InsertEndChild(child);

    std::string text = getChildElementText(parentElement, "Child");
    EXPECT_EQ(text, "Hello");
}

TEST_F(UtilityFunctionTests, GetChildElementTextNonExisting) {
    std::string text = getChildElementText(parentElement, "Child");
    EXPECT_TRUE(text.empty());
}

TEST_F(UtilityFunctionTests, GetChildElementTextMultipleChildren) {
    auto* child1 = doc.NewElement("Child1");
    child1->SetText("Text1");
    parentElement->InsertEndChild(child1);

    auto* target = doc.NewElement("Target");
    target->SetText("Found");
    parentElement->InsertEndChild(target);

    std::string text = getChildElementText(parentElement, "Target");
    EXPECT_EQ(text, "Found");
}

TEST_F(UtilityFunctionTests, GetChildElementTextNestedChild) {
    auto* child = doc.NewElement("Child");
    auto* nested = doc.NewElement("Nested");
    nested->SetText("Deep");
    child->InsertEndChild(nested);
    parentElement->InsertEndChild(child);

    std::string text = getChildElementText(parentElement, "Nested");
    EXPECT_TRUE(text.empty());
}

TEST_F(UtilityFunctionTests, GetChildElementTextNullParent) {
    std::string text = getChildElementText(nullptr, "AnyChild");
    EXPECT_TRUE(text.empty());
}



// Main function
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
