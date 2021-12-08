#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "DatalogProgram.h"
#include "Database.h"
#include <sstream>
#include <iostream>

class Interpreter {
public:
    Interpreter(DatalogProgram* datalogProgram);
    ~Interpreter();
    void Run();
    void ToString();
    std::vector<Relation*> ruleResults;
    int iterations = 0;
private:
    DatalogProgram* program;
    Database* database;
    Relation* EvaluatePredicate(Predicate* query);
    void EvaluateSchemes();
    void EvaluateFacts();
    void EvaluateRules();
    bool EvaluateRulesHelper();
    Relation* Join(std::vector<Relation*> intermediateRelations);
    void EvaluateHeadPredicate(Predicate* headPredicate, Relation* finalRelation);
    void EvaluateQueries();
    std::vector<std::string> ConvertParameters(std::vector<Parameter*> parameters);
    std::stringstream ss;
    void OutputRules();
};

#endif