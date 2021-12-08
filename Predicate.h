#ifndef PREDICATE_H
#define PREDICATE_H
#include "Parameter.h"
#include <string>
#include <vector>

class Predicate {
public:
    Predicate(){};
    ~Predicate();
    std::string id;
    std::vector<Parameter*> parameters;
    std::string ToString();
};

#endif