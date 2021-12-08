#include "Rule.h"

std::string Rule::ToString() {
    std::string rule = "";
    std::string bodyPredicateToAdd = "";
    rule = headPredicate->ToString() + " :- ";
    for(unsigned long int i = 0; i < bodyPredicates.size(); i++) {
        bodyPredicateToAdd = bodyPredicates.at(i)->ToString();
        if(i == (bodyPredicates.size() - 1)){
            rule += (bodyPredicateToAdd + ".");
        } else {
            rule += (bodyPredicateToAdd + ",");
        }
    }
    return rule;
}