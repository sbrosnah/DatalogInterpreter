#ifndef RELATION_H
#define RELATION_H

#include <set>
#include "Tuple.h"
#include <string>
#include "Header.h"
#include <vector>
#include <set>
#include <map>
#include <sstream>


class Relation {
private:
    std::string name;
    std::set<Tuple> tuples;
    Header* header;
    std::map<std::string, std::vector<std::string>> variablesWithValues;
    std::vector<int> commonIndicesFirst;
    std::vector<int> commonIndicesSecond;
    bool CheckEquality(Tuple tupleOne, Tuple tupleTwo);
    Tuple CreateJoinedTuple(Tuple tupleOne, Tuple tupleTwo);
    void JoinHeaders(std::vector<std::string> secondHeader);
    void JoinTuples(std::set<Tuple> oldTuples, std::set<Tuple> secondTuples);
    bool isRule = false;
public:
    Relation(){};
    ~Relation();
    void SetName(std::string name);
    std::string GetName();
    std::vector<std::string> GetHeader();
    void SetHeader(std::vector<std::string> header);
    void PushTuple(Tuple tuple);
    std::set<Tuple> GetTuples();
    void SetTuples(std::set<Tuple> tuples);
    //std::set<Tuple> Select(std::vector<int> positions, std::vector<std::string> values);
    void Select(std::vector<int> positions, std::vector<std::string> values);
    //std::set<Tuple> Select(std::vector<int> positions);
    void Select(std::vector<int> positions);
    void Project(std::map<std::string, std::vector<int>> variableOccurranceIndices);
    void ProjectTuples(std::map<std::string, std::vector<int>> variableOccurranceIndices);
    void Rename(std::vector<std::string> variables);
    void SetMap(std::map<std::string, std::vector<int>> variableOccurranceIndices);
    void Join(std::vector<std::string> secondHeader, std::set<Tuple> secondTuples);
    bool Union(std::set<Tuple>& tuplesToAdd);
    std::string ToString();
    void ToggleIsRule();
};

#endif