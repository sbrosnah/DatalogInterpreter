#ifndef DATABASE_H
#define DATABASE_H

#include <map>
#include <string>
#include "Relation.h"

class Database {
private:
    std::map<std::string, Relation*> relations;
public:
    Database(){};
    ~Database();
    Relation* GetRelation(std::string name);
    void PushRelation(Relation* relation);
    void PushTuple(std::string relationName, Tuple tuple);
    void EditRelation(Relation* relation);
    std::map<std::string, Relation*> GetRelations();
};

#endif