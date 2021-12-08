#ifndef RULE_H
#define RULE_H
#include "Predicate.h"
#include <string>
#include <vector>

class Rule {
public:
    Predicate* headPredicate;
    std::vector<Predicate*> bodyPredicates;
    std::string ToString();
};

#endif