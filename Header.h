#ifndef HEADER_H
#define HEADER_H

#include <vector>
#include <string>
#include <set>
#include <map>

class Header {
private:
    std::vector<std::string> attributes;
public:
    Header(){};
    ~Header(){};
    void SetAttributes(std::vector<std::string> attributes);
    void AddAttribute(std::string attribute);
    void GetAttribute(int index);
    void ProjectAttributes(std::map<std::string, std::vector<int>> variableOccurranceIndices);
    std::vector<std::string> GetAttributes();
    void EditAttribute(int index, std::string attribute);
};

#endif