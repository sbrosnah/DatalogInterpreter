#ifndef DATALOGPROGRAM_H
#define DATALOGPROGRAM_H
#include <vector>
#include <string>
#include <set>
#include <sstream>
#include "Predicate.h"
#include "Rule.h"

class DatalogProgram {
private:
    std::stringstream ss;
    std::string errorString;
public:
    DatalogProgram(){};
    ~DatalogProgram();
    std::vector<Predicate*> schemes;
    std::vector<Predicate*> facts;
    std::vector<Rule*> rules;
    std::vector<Predicate*> queries;
    void SetErrorString(std::string errorString);
    std::string ToString();
    bool success = false;
    std::set<std::string> domains;
};

#endif