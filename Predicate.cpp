#include "Predicate.h"

Predicate::~Predicate() {
    while(parameters.size() > 0) {
        Parameter* parameter = parameters.back();
        parameters.pop_back();
        delete parameter;
    }
}

std::string Predicate::ToString() {
    std::string predicate;
    std::string parameterToAdd = "";
    predicate = id + "(";
    for(unsigned long int i = 0; i < parameters.size(); i++) {
        parameterToAdd = parameters.at(i)->ToString();
        if(i == (parameters.size() - 1)){
            predicate += parameterToAdd;
        } else {
            predicate += (parameterToAdd + ",");
        }
    }
    predicate += ")";
    return predicate;
}