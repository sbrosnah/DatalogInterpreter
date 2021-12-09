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
    if (graph != nullptr){
        delete graph;
    }
    if (reverseGraph != nullptr){
        delete reverseGraph;
    }
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
    ss << std::endl;
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
    std::cout << sg.str();
    std::cout << ss.str();
}


void Interpreter::EvaluateRules() {
    try{
        FindSCCs();
    } catch(std::string err){
        std::cout << err << std::endl;
    }
    bool ruleAdded = true;
    ss << "Rule Evaluation" << std::endl;
    std::set<int> currSCC;
    for(unsigned int i = 0; i < SCCs.size(); i++){
        bool trivial = false;
        currSCC = SCCs.at(i);
        ss << "SCC: ";
        int sccSize = currSCC.size();
        std::set<int>::iterator it = currSCC.begin();
        std::string nodeString;
        for(int i = 0; i < sccSize; i++){
            nodeString += "R";
            nodeString += std::to_string(*it);
            if(i < sccSize - 1){
                nodeString += ",";
            } else {
                nodeString += "\n";
            }
            it++;
        }
        ss << nodeString;
        if(currSCC.size() == 1){
            int node = *currSCC.begin();
            trivial = CheckTrivial(node);
        }
        if(trivial && currSCC.size() == 1){
            iterations = 1;
            EvaluateRulesHelper(currSCC);
        } else {
            iterations = 0;
            ruleAdded = true;
            while(ruleAdded){
                try{
                    iterations += 1;
                    ruleAdded = EvaluateRulesHelper(currSCC);
                } catch(std::string err){
                    std::cout << err << std::endl;
                }
            }
        }
        ss << iterations << " passes: " << nodeString;
    }
}

bool Interpreter::CheckTrivial(int node){
    bool isTrivial = false;
    if(graph->edges[node].find(node) == graph->edges[node].end()){
        isTrivial = true;
    }
    return isTrivial;
}

bool Interpreter::EvaluateRulesHelper(std::set<int> currSCC) {
    bool addedTuple = false;
    for(std::set<int>::iterator it = currSCC.begin(); it != currSCC.end(); it++){
        bool added = false;
        std::vector<Relation*> intermediateRelations;
        std::string ruleName = assignedRules[*it]->headPredicate->ToString();
        for(unsigned int j = 0; j < assignedRules[*it]->bodyPredicates.size(); j++){
            Predicate* currPredicate = assignedRules[*it]->bodyPredicates.at(j);
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
        Predicate* headPredicate = assignedRules[*it]->headPredicate;
        EvaluateHeadPredicate(headPredicate, finalRelation);
        Relation* originalRelation = database->GetRelation(finalRelation->GetName());
        std::set<Tuple> finalRelationTuples = finalRelation->GetTuples();
        added = originalRelation->Union(finalRelationTuples);
        finalRelation->SetTuples(finalRelationTuples);
        finalRelation->SetName(assignedRules[*it]->ToString());
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

std::stack<int> Interpreter::InitializePostOrder(){
    std::stack<int> stack;
    for(signed int i = static_cast<signed int>(program->rules.size()) - 1; i >= 0; i--){
        stack.push(i);
    }
    return stack;
}

void Interpreter::LoadSCCs(std::vector<std::vector<int>> trees){
    std::vector<int> currTree;
    int currNode;
    std::set<int> currSet;
    for(unsigned int i = 0; i < trees.size(); i++){
        currSet.clear();
        currTree = trees.at(i);
        for(unsigned int j = 0; j < currTree.size(); j++){
            currNode = currTree.at(j);
            currSet.insert(currNode);
        }
        SCCs.push_back(currSet);
    }
}

void Interpreter::FindSCCs() {
    BuildGraph();
    ss << graph->toString();
    BuildReverseGraph();
    std::vector<std::vector<int>> trees;
    std::stack<int> initialPostOrder = InitializePostOrder();
    std::stack<int> reverseGraphPostOrder = DepthFirstSearchForest(reverseGraph, initialPostOrder, trees);
    trees.clear();
    std::stack<int> forwardGraphPostOrder = DepthFirstSearchForest(graph, reverseGraphPostOrder, trees);
    LoadSCCs(trees);
}

void Interpreter::BuildGraph() {
    graph = new Graph();
    for(unsigned int i = 0; i < program->rules.size(); i++){
        Rule* currRule = program->rules.at(i);
        assignedRules[i] = currRule;
        std::string headPredicateId = currRule->headPredicate->id;
        headPredicatesWithValue[headPredicateId].insert(i);
    }
    AssignNodesAndEdges();
}

void Interpreter::AssignNodesAndEdges(){
    for(unsigned int i = 0; i < program->rules.size(); i++){
        Rule* currRule = program->rules.at(i);
        int originNode = i;
        for(unsigned int j = 0; j < currRule->bodyPredicates.size(); j++){
            Predicate* currPredicate = currRule->bodyPredicates.at(j);
            std::string currId = currPredicate->id;
            std::set<int> adjacentNodes = headPredicatesWithValue[currId];
            if(!adjacentNodes.empty()){
                for(std::set<int>::iterator it = adjacentNodes.begin(); it != adjacentNodes.end(); it++){
                    //graph->SetEdge(originNode, *it);
                    graph->edges[originNode].insert(*it);
                }
            } else {
                //graph->SetEdge(originNode);
                graph->edges[originNode];
            }
        }
    }
}

void Interpreter::BuildReverseGraph() {
    reverseGraph = new Graph();
    for(std::map<int, std::set<int>>::iterator it = graph->edges.begin(); it != graph->edges.end(); it++){
        for(std::set<int>::iterator itr = it->second.begin(); itr != it->second.end(); itr++){
            reverseGraph->edges[*itr].insert(it->first);
        }
    }
}

std::vector<int> Interpreter::DepthFirstSearch(Graph* graph, int startNode, std::vector<int>& postOrder) {
    int currNode = startNode;
    std::vector<int> tree;
    Explore(currNode, graph, postOrder, tree);
    return tree;
}

void Interpreter::Explore(int currNode, Graph* graph, std::vector<int>& postOrder, std::vector<int>& tree){
    graph->visitedNodes[currNode] = true;
    tree.push_back(currNode);
    std::set<int> adjacentNodes = graph->edges[currNode];
    bool nodesRemain = true;
    while(nodesRemain){
        int nextNode = findLowestUnvisitedNode(adjacentNodes, graph, nodesRemain);
        if(nextNode > 0){
            Explore(nextNode, graph, postOrder, tree);
            postOrder.push_back(currNode);
        } else {
            postOrder.push_back(currNode);
        }
    }
}

int Interpreter::findLowestUnvisitedNode(std::set<int> adjacentNodes, Graph *graph, bool& nodesRemain){
    int nodeToReturn = -1;
    nodesRemain = false;
    for(std::set<int>::iterator it = adjacentNodes.begin(); it != adjacentNodes.end(); it++){
        int currNode = *it;
        if(!graph->visitedNodes[currNode]){
            nodeToReturn = currNode;
            nodesRemain = true;
            break;
        }
    }
    return nodeToReturn;
}

void Interpreter::AddToPostOrder(const std::vector<int>& tree, std::stack<int>& postOrder){
    int node = -1;
    for(unsigned int i = 0; i < tree.size(); i++){
        node = tree.at(i);
        postOrder.push(node);
    }
}

std::stack<int> Interpreter::DepthFirstSearchForest(Graph *graph, std::stack<int> postOrder, std::vector<std::vector<int>>& trees) {
    std::vector<int> currTree;
    std::vector<int> currPostOrder;
    std::stack<int> newPostOrder;
    while(!postOrder.empty()){
        int currNode = postOrder.top();
        postOrder.pop();
        currPostOrder.clear();
        if(graph->visitedNodes[currNode] == false){
            currTree = DepthFirstSearch(graph, currNode, currPostOrder);
            AddToPostOrder(currPostOrder, newPostOrder);
            trees.push_back(currTree);
        }
    }
    return newPostOrder;
}




