#include "Interpreter.h"
#include "Relation.h"
#include <vector>
#include <iostream>

Interpreter::Interpreter(DatalogProgram* datalogProgram) {
    this->program = datalogProgram;
    database = new Database();
}

Interpreter::~Interpreter() {
    delete database;
}

void Interpreter::Run() {
    try{
        EvaluateSchemes();
        EvaluateFacts();
        EvaluateRules();
        EvaluateQueries();
    } catch (std::string error){
        std::cout << error << std::endl;
    }
}

std::vector<std::string> Interpreter::ConvertParameters(std::vector<Parameter *> parameters) {
    std::vector<std::string> header;
    for(unsigned int i = 0; i < parameters.size(); i ++){
        std::string currString = parameters.at(i)->p;
        header.push_back(currString);
    }
    return header;
}

void Interpreter::EvaluateSchemes() {
    for(unsigned int i = 0; i < program->schemes.size(); i++) {
        Relation* newRelation = new Relation();
        newRelation->SetName(program->schemes.at(i)->id);
        std::vector<std::string> header = ConvertParameters(program->schemes.at(i)->parameters);
        newRelation->SetHeader(header);
        database->PushRelation(newRelation);
    }
}

void Interpreter::EvaluateFacts() {
    for(unsigned int i = 0; i < program->facts.size(); i++) {
        std::string relationName = program->facts.at(i)->id;
        Tuple tuple;
        std::vector<std::string> values = ConvertParameters(program->facts.at(i)->parameters);
        tuple.SetValues(values);
        database->PushTuple(relationName, tuple);
    }
}

void Interpreter::EvaluateQueries() {
    ss << "Query Evaluation" << std::endl;
    for(unsigned int i = 0; i < program->queries.size(); i++) {
        Predicate* query = program->queries.at(i);
        Relation* currRelation = EvaluatePredicate(query);
        ss << currRelation->ToString();
    }
}

void Interpreter::EvaluateHeadPredicate(Predicate *headPredicate, Relation *finalRelation){
    std::string relationName = headPredicate->id;
    finalRelation->SetName(relationName);
    std::vector<Parameter *> parameters = headPredicate->parameters;
    std::vector<std::string> variables;
    std::map<std::string, std::vector<int>> variableOccurranceIndices;
    std::vector<std::string> relationHeader = finalRelation->GetHeader();

    for(unsigned int i = 0; i < parameters.size(); i++){
        variables.push_back(parameters.at(i)->p);
    }

    for(unsigned int i = 0; i < relationHeader.size(); i++){
        for(unsigned int j = 0; j < variables.size(); j++){
            if(relationHeader.at(i) == variables.at(j)){
                std::vector<int> indices;
                indices.push_back(i);
                indices.push_back(j);
                variableOccurranceIndices[relationHeader.at(i)] = indices;
            }
        }
    }
    Relation* relationAddedTo = database->GetRelation(relationName);
    std::vector<std::string> newHeader = relationAddedTo->GetHeader();
    finalRelation->ProjectTuples(variableOccurranceIndices);
    finalRelation->SetHeader(newHeader);
}

Relation* Interpreter::EvaluatePredicate(Predicate *query) {
    std::string relationName = query->id;
    Relation* currRelation = database->GetRelation(relationName);
    Relation* currRelationCopy = new Relation();
    std::vector<int> constantsLocations;
    std::vector<std::string> constants;
    std::vector<int> variableLocations;
    std::vector<std::string> variables;
    std::map<std::string, std::vector<int>> variableOccurranceIndices;
    std::set<Tuple> currSetOfTuples;
    std::string currRelationCopyName = query->ToString() + "?";
    currRelationCopy->SetName(currRelationCopyName);
    currRelationCopy->SetHeader(currRelation->GetHeader());
    currRelationCopy->SetTuples(currRelation->GetTuples());

    for(unsigned int i = 0; i < query->parameters.size(); i++) {
        std::string currParameter = query->parameters.at(i)->p;
        bool isConstant = false;
        //if there is an apostrophe, it is a string
        if (currParameter.at(0) == '\''){
            isConstant = true;
        } //else, it is a constant
        //if it's a constant, we are going to for that single value
        if(isConstant){
            constantsLocations.push_back(i);
            constants.push_back(currParameter);
        } else {
            variableOccurranceIndices[currParameter].push_back(i);
            variables.push_back(currParameter);
        }
    }
    if(constantsLocations.size() > 0){
        currRelationCopy->Select(constantsLocations, constants);
    }
    if(!variableOccurranceIndices.empty()) {
        for(std::map<std::string, std::vector<int>>::iterator it = variableOccurranceIndices.begin(); it != variableOccurranceIndices.end(); it++){
            //if there are variables, then we do select then project at those positions, but no repeats
            std::vector<int> positions = it->second;
            if(positions.size() > 1){
                //the variable is repeated
                currRelationCopy->Select(positions);
            }

        }
        currRelationCopy->Project(variableOccurranceIndices);
        currRelationCopy->Rename(variables);
        currRelationCopy->SetMap(variableOccurranceIndices);
    }
    return currRelationCopy;
}

void Interpreter::ToString() {
    std::cout << ss.str();
}

void Interpreter::OutputRules(){

}

void Interpreter::EvaluateRules() {
    bool ruleAdded = true;
    ss << "Rule Evaluation" << std::endl;
    while(ruleAdded){
        try{
            ruleAdded = EvaluateRulesHelper();
            iterations += 1;
        } catch(std::string err){
            std::cout << err << std::endl;
        }
    }
    ss << std::endl;
    ss << "Schemes populated after " << iterations << " passes through the Rules." << std::endl;
    ss << std::endl;
}

bool Interpreter::EvaluateRulesHelper() {
    bool addedTuple = false;
    for(unsigned int i = 0; i < program->rules.size(); i++){
        bool added = false;
        std::vector<Relation*> intermediateRelations;
        std::string ruleName = program->rules.at(i)->ToString();
        for(unsigned int j = 0; j < program->rules.at(i)->bodyPredicates.size(); j++){
            Predicate* currPredicate = program->rules.at(i)->bodyPredicates.at(j);
            Relation* newRelation;
            newRelation = EvaluatePredicate(currPredicate);
            intermediateRelations.push_back(newRelation);
        }
        Relation* finalRelation = new Relation();
        if(intermediateRelations.size() > 1){
            finalRelation = Join(intermediateRelations);
        } else if(intermediateRelations.size() == 1){
            finalRelation = intermediateRelations.at(0);
        } else {
            throw "No relations with those names";
        }
        Predicate* headPredicate = program->rules.at(i)->headPredicate;
        EvaluateHeadPredicate(headPredicate, finalRelation);
        Relation* originalRelation = database->GetRelation(finalRelation->GetName());
        std::set<Tuple> finalRelationTuples = finalRelation->GetTuples();
        added = originalRelation->Union(finalRelationTuples);
        finalRelation->SetTuples(finalRelationTuples);
        finalRelation->SetName(program->rules.at(i)->ToString());
        finalRelation->ToggleIsRule();
        ss << finalRelation->ToString();
        delete finalRelation;
        if(added == true){
            addedTuple = true;
        }
    }
    return addedTuple;
}

Relation* Interpreter::Join(std::vector<Relation*> intermediateRelations) {
    Relation* firstRelation = intermediateRelations.at(0);
    Relation* secondRelation;
    std::vector<std::string> secondHeader;
    std::set<Tuple> secondTuples;
    for(unsigned int i = 1; i < intermediateRelations.size(); i++){
        secondRelation = intermediateRelations.at(i);
        secondHeader = secondRelation->GetHeader();
        secondTuples = secondRelation->GetTuples();
        firstRelation->Join(secondHeader, secondTuples);
    }
    return firstRelation;
}




