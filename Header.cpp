#include "Header.h"

void Header::SetAttributes(std::vector<std::string> attributes) {
    this->attributes = attributes;
}

std::vector<std::string> Header::GetAttributes() {
    std::vector<std::string> attributesCopy = this->attributes;
    return attributes;
}

void Header::ProjectAttributes(std::map<std::string, std::vector<int>> variableOccurranceIndices){
    std::vector<std::string> newAttributes;
    for(std::map<std::string, std::vector<int>>::iterator it = variableOccurranceIndices.begin(); it != variableOccurranceIndices.end(); it++){
        newAttributes.push_back(it->first);
    }
    attributes.clear();
    for(unsigned int i = 0; i < newAttributes.size(); i++){
        attributes.push_back(newAttributes.at(i));
    }
}

void Header::EditAttribute(int index, std::string attribute){
    attributes.at(index) = attribute;
}

void Header::AddAttribute(std::string attribute) {
    attributes.push_back(attribute);
}