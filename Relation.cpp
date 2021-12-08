#include "Relation.h"
#include <sstream>

Relation::~Relation() {
    delete header;
}

void Relation::SetName(std::string name) {
    this->name = name;
}

std::string Relation::GetName() {
    std::string nameToReturn = this->name;
    return name;
}

void Relation::SetHeader(std::vector<std::string> header) {
    this->header = new Header();
    this->header->SetAttributes(header);
}

std::vector<std::string> Relation::GetHeader() {
    std::vector<std::string> headerToReturn = this->header->GetAttributes();
    return headerToReturn;
}

void Relation::PushTuple(Tuple tuple) {
    tuples.insert(tuple);
}

std::set<Tuple> Relation::GetTuples(){
    std::set<Tuple> tuplesCopy = tuples;
    return tuplesCopy;
}
void Relation::SetTuples(std::set<Tuple> tuples){
    this->tuples = tuples;
}

void Relation::Select(std::vector<int> positions, std::vector<std::string> values) {
    std::vector<std::set<Tuple>::iterator> tuplesToErase;
    for(Tuple tuple : tuples) {
        for(unsigned int i = 0; i < positions.size(); i++) {
            int currPosition = positions.at(i);
            std::string currValue = values.at(i);
            std::string tupleVal = tuple.GetValue(currPosition);
            if(tupleVal != currValue){
                std::set<Tuple>::iterator it = tuples.find(tuple);
                tuplesToErase.push_back(it);
                break;
            }
        }
    }
    for(unsigned int i = 0; i < tuplesToErase.size(); i++){
        this->tuples.erase(tuplesToErase.at(i));
    }
    //return setToReturn;
}

void Relation::Select(std::vector<int> positions){
    std::vector<std::set<Tuple>::iterator> tuplesToErase;
    for(Tuple tuple : tuples) {
        std::string stringToMatch = "";
        for(unsigned int i = 0; i < positions.size(); i++) {
            int currPos = positions.at(i);
            if(i == 0){
                stringToMatch = tuple.GetValue(currPos);
            } else {
                std::string currString = tuple.GetValue(currPos);
                if(currString != stringToMatch){
                    std::set<Tuple>::iterator it = tuples.find(tuple);
                    tuplesToErase.push_back(it);
                    break;
                }
            }
        }
    }
    for(unsigned int i = 0; i < tuplesToErase.size(); i++){
        this->tuples.erase(tuplesToErase.at(i));
    }
}

void Relation::Project(std::map<std::string, std::vector<int>> variableOccurranceIndices) {
    //VariableOccuranceIndices is a map that has the variable mapped to a vector of the indices where the variable occurs in the query
    for(std::map<std::string, std::vector<int>>::iterator it = variableOccurranceIndices.begin(); it != variableOccurranceIndices.end(); it++){
        //make sure that there is only one variable appearance and that the associated index is to the first occurring one.
        if(it->second.size() > 1){
            int firstIndex = it->second.at(0);
            it->second.clear();
            it->second.push_back(firstIndex);
        }
    }
    //change the header of my tuple to only contain the first occurance of each variable.
    header->ProjectAttributes(variableOccurranceIndices);
    //call the function that replaces each tuple in the current relation with a tuple only containing the values associated to the variables
    //found in the query.
    ProjectTuples(variableOccurranceIndices);
}

void Relation::ProjectTuples(std::map<std::string, std::vector<int>> variableOccurranceIndices){
    std::set<Tuple> newTuples;
    std::set<int> indices;

    //Remember, we have already checked that each mapped variable has only one value in the vector associated to it.
    //create a set of the indices so that we can quickly check if the index of a value in a tuple is associated with the index of the variable.
    bool isHeadPredicateEvaluation = false;
    for(std::map<std::string, std::vector<int>>::iterator it = variableOccurranceIndices.begin(); it != variableOccurranceIndices.end(); it++){
        if(it->second.size() > 1){
            isHeadPredicateEvaluation = true;
        }
        indices.insert(it->second.at(0));
    }


    //loop through all of my tuples
    for(Tuple tuple : this->tuples){
        //Get the values in the current tuple of the iteration.
        std::vector<std::string> oldValues = tuple.GetValues();
        //create a new tuple so that we can push only the variable values into it.
        Tuple newTuple;
        //loop through the tuple values
        if(!isHeadPredicateEvaluation){
            for(unsigned int i = 0; i < oldValues.size(); i++){
                //check the set of indices to see if the current index of the values is the index of a variable, if so, Add it to our new tuple.
                if(indices.find(i) != indices.end()){
                    newTuple.AddValue(oldValues.at(i));
                }
            }
        } else {
            unsigned int arraySize = variableOccurranceIndices.size();
            std::vector<std::string> values;
            std::vector<std::string> oldHeader = header->GetAttributes();
            for(unsigned int i = 0; i < arraySize; i++){
                values.push_back("");
            }
            for(unsigned int i = 0; i < oldHeader.size(); i++){
                if(variableOccurranceIndices.find(oldHeader.at(i)) != variableOccurranceIndices.end()){
                    values.at(variableOccurranceIndices[oldHeader.at(i)].at(1)) = oldValues.at(i);
                }
            }
            for(unsigned int i = 0; i < arraySize; i++){
                newTuple.AddValue(values.at(i));
            }
        }
        //insert the new tuple into a set of tuples so that we don't get any repeating ones.
        newTuples.insert(newTuple);
    }
    //clear all of the tuples currently associated with the current relation. Then replace them with our new tuples.
    this->tuples.clear();
    for(Tuple tuple : newTuples){
        this->tuples.insert(tuple);
    }
}

void Relation::Rename(std::vector<std::string> variables) {
    std::vector<std::string> oldHeader = header->GetAttributes();
    std::set<std::string> contained;
    for(unsigned int i = 0; i < oldHeader.size(); i++){
        //We already know that our map size and our header size are equal because all variables will be projected and only occur once.
        if(contained.find(variables.at(i)) == contained.end()){
            header->EditAttribute(i, variables.at(i));
            contained.insert(variables.at(i));
        } else {
            i++;
        }

    }
}

void Relation::ToggleIsRule() {
    if(isRule == false){
        isRule = true;
    } else {
        isRule = false;
    }
}

std::string Relation::ToString() {
    std::stringstream ss;
    ss << this->name;
    if(!isRule){
        ss << " ";
    } else {
        ss << std::endl;
    }
    if(tuples.empty()){
        if(!isRule){
            ss << "No" << std::endl;
        }
    } else {
        if(!isRule){
            ss << "Yes(" << tuples.size() << ")" << std::endl;
        }
        if(!variablesWithValues.empty()){
            std::vector<std::string> variables = header->GetAttributes();
            for(Tuple tuple : tuples){
                std::vector<std::string> values = tuple.GetValues();
                ss << "  ";
                for(unsigned int i = 0; i < variables.size(); i++){
                    ss << variables.at(i) << "=" << values.at(i);
                    if(i < variables.size() -1){
                        ss << ", ";
                    } else {
                        ss << std::endl;
                    }
                }
            }
        }
    }
    return ss.str();
}

void Relation::SetMap(std::map<std::string, std::vector<int>> variableOccurranceIndices){
    if(!variableOccurranceIndices.empty()){
        std::vector<int> indices;
        std::vector<std::string> variables;
        for(std::map<std::string, std::vector<int>>::iterator it = variableOccurranceIndices.begin(); it != variableOccurranceIndices.end(); it++){
            indices.push_back(it->second.at(0));
            variables.push_back(it->first);
        }
        for(Tuple tuple : tuples){
            std::vector<std::string> values = tuple.GetValues();
            std::vector<std::string> associatedValues;
            for(unsigned int i = 0; i < indices.size(); i++){
                std::string valueToPush = values.at(i);
                variablesWithValues[variables.at(i)].push_back(valueToPush);
            }
        }
    }
}

void Relation::Join(std::vector<std::string> secondHeader, std::set<Tuple> secondTuples){
    JoinHeaders(secondHeader);
    std::set<Tuple> oldTuples = tuples;
    tuples.clear();
    JoinTuples(oldTuples, secondTuples);
    commonIndicesFirst.clear();
    commonIndicesSecond.clear();
}

void Relation::JoinTuples(std::set<Tuple> oldTuples, std::set<Tuple> secondTuples){
    for(Tuple tuple : oldTuples){
        for (Tuple secondTuple : secondTuples){
            if(commonIndicesFirst.size() > 0){
                bool equal = CheckEquality(tuple, secondTuple);
                if(equal){
                    tuples.insert(CreateJoinedTuple(tuple, secondTuple));
                }
            } else {
                Tuple newTuple;
                newTuple.SetValues(tuple.GetValues());
                std::vector<std::string> secondTupleValues = secondTuple.GetValues();
                for(unsigned int i = 0; i < secondTupleValues.size(); i++){
                    newTuple.AddValue(secondTupleValues.at(i));
                }
                tuples.insert(newTuple);
            }
        }
    }
}

Tuple Relation::CreateJoinedTuple(Tuple tupleOne, Tuple tupleTwo){
    std::set<int> commonIndices;
    for(unsigned int i = 0; i < commonIndicesSecond.size(); i++){
        commonIndices.insert(commonIndicesSecond.at(i));
    }
    std::vector<std::string> newValues = tupleOne.GetValues();
    std::vector<std::string> secondValues = tupleTwo.GetValues();
    for(unsigned int i = 0; i < secondValues.size(); i++){
        if(commonIndices.find(i) == commonIndices.end()){
            newValues.push_back(secondValues.at(i));
        }
    }
    Tuple newTuple;
    newTuple.SetValues(newValues);
    return newTuple;
}

bool Relation::CheckEquality(Tuple tupleOne, Tuple tupleTwo){
    std::vector<std::string> tupleValsOne = tupleOne.GetValues();
    std::vector<std::string> tupleValsTwo = tupleTwo.GetValues();
    //because we know that commonIndicesFirst and commonIndicesSecond will be the same size and the index of the index correlate to the same
    //value, we only need to iterate through one of them.
    bool areEqual = true;
    for(unsigned int i = 0; i < commonIndicesFirst.size(); i++){
        if(tupleValsOne.at(commonIndicesFirst.at(i)) != tupleValsTwo.at(commonIndicesSecond.at(i))){
            areEqual = false;
        }
    }
    return areEqual;
}

void Relation::JoinHeaders(std::vector<std::string> secondHeader){
    std::map<std::string, int> currHeaderInd;
    std::vector<std::string> thisHeader = header->GetAttributes();

    for(unsigned int i = 0; i < thisHeader.size(); i++){
        currHeaderInd[thisHeader.at(i)] = i;
    }
    for(unsigned int i = 0; i < secondHeader.size(); i++){
        std::string currAttribute = secondHeader.at(i);
        std::map<std::string, int>::iterator it;
        it = currHeaderInd.find(currAttribute);
        if(it != currHeaderInd.end()){
            commonIndicesFirst.push_back(it->second);
            commonIndicesSecond.push_back(i);
        } else {
            header->AddAttribute(currAttribute);
        }
    }
}

bool Relation::Union(std::set<Tuple>& tuplesToAdd){
    bool addedTuples = false;
    std::set<Tuple> tuplesAdded;
    for(Tuple tuple : tuplesToAdd){
        bool added = false;
        added = tuples.insert(tuple).second;
        if(added){
            addedTuples = true;
            tuplesAdded.insert(tuple);
        }
    }
    tuplesToAdd = tuplesAdded;
    return addedTuples;
}



