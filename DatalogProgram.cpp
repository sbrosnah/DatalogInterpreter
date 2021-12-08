#include "DatalogProgram.h"

DatalogProgram::~DatalogProgram(){
    while(schemes.size() > 0){
        Predicate* predicate = schemes.back();
        schemes.pop_back();
        delete predicate;
    }
    while(facts.size() > 0){
        Predicate* predicate = facts.back();
        facts.pop_back();
        delete predicate;
    }
    while(rules.size() > 0){
        Rule* rule = rules.back();
        rules.pop_back();
        delete rule;
    }
    while(queries.size() > 0){
        Predicate* predicate = queries.back();
        queries.pop_back();
        delete predicate;
    }
}

std::string DatalogProgram::ToString() {
    if(success) {
        ss << "Success!" << std::endl;
        ss << "Schemes(" << schemes.size() << "):" << std::endl;
        for(long unsigned int i = 0; i < schemes.size(); i++) {
            ss << "  " << schemes.at(i)->ToString() << std::endl;
        }
        ss << "Facts(" << facts.size() << "):" << std::endl;
        for(long unsigned int i = 0; i < facts.size(); i++) {
            ss << "  " << facts.at(i)->ToString() << "." << std::endl;
        }
        ss << "Rules(" << rules.size() << "):" << std::endl;
        for(long unsigned int i = 0; i < rules.size(); i++) {
            ss << "  " << rules.at(i)->ToString() << std::endl;
        }
        ss << "Queries(" << queries.size() << "):" << std::endl;
        for(long unsigned int i = 0; i < queries.size(); i++) {
            ss << "  " << queries.at(i)->ToString() << "?" << std::endl;
        }
        ss << "Domain(" << domains.size() << "):" << std::endl;
        for(auto it = domains.begin(); it != domains.end(); it++) {
            ss << "  " << *it << std::endl;
        }
    } else {
        ss << "Failure!" << std::endl;
        ss << "  " << errorString;
    }
    return ss.str();
}

void DatalogProgram::SetErrorString(std::string errorString){
    this->errorString = errorString;
}
