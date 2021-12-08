#include "Database.h"

Database::~Database() {
    std::map<std::string, Relation*>::iterator it = relations.begin();
    while(!relations.empty()) {
        std::string key = it->second->GetName();
        it++;
        delete relations[key];
        relations.erase(key);
    }
}

Relation* Database::GetRelation(std::string name) {
    Relation* relation = relations[name];
    return relation;
}

void Database::PushRelation(Relation *relation) {
    relations[relation->GetName()] = relation;
}

void Database::EditRelation(Relation *relation) {
    relations[relation->GetName()] = relation;
}

std::map<std::string, Relation*> Database::GetRelations() {
    std::map<std::string, Relation*> relationsCopy = relations;
    return relationsCopy;
}

void Database::PushTuple(std::string relationName, Tuple tuple) {
    Relation* foundRelation = relations[relationName];
    foundRelation->PushTuple(tuple);

}