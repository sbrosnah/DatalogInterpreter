#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "DatalogProgram.h"
#include "Database.h"
#include "Graph.h"
#include <sstream>
#include <iostream>
#include <stack>

class Interpreter {
public:
    Interpreter(DatalogProgram* datalogProgram);
    ~Interpreter();
    void Run();
    void ToString();
    std::vector<Relation*> ruleResults;
    int iterations = 0;
    Graph* graph = nullptr;
    Graph* reverseGraph = nullptr;
private:
    DatalogProgram* program;
    Database* database;
    Relation* EvaluatePredicate(Predicate* query);
    void EvaluateSchemes();
    void EvaluateFacts();
    void EvaluateRules();
    bool EvaluateRulesHelper(std::set<int> currSCC);
    Relation* Join(std::vector<Relation*> intermediateRelations);
    void EvaluateHeadPredicate(Predicate* headPredicate, Relation* finalRelation);
    void EvaluateQueries();
    std::vector<std::string> ConvertParameters(std::vector<Parameter*> parameters);
    std::stringstream sg;
    std::stringstream ss;
    void FindSCCs();
    void LoadSCCs(std::vector<std::vector<int>> trees);
    bool CheckTrivial(int node);
    std::vector<std::set<int>> SCCs;
    void BuildGraph();
    void BuildReverseGraph();
    std::vector<int> DepthFirstSearch(Graph* graph, int startNode, std::vector<int>& postOrder);
    std::stack<int> DepthFirstSearchForest(Graph* graph, std::stack<int> postOrder, std::vector<std::vector<int>>& trees);
    std::set<int> nodes;
    std::map<int, Rule*> assignedRules;
    std::map<std::string, std::set<int>> headPredicatesWithValue;
    void AssignNodesAndEdges();
    int findLowestUnvisitedNode(std::set<int> adjacentNodes, Graph *graph, bool& nodesRemain);
    void Explore(int nextNode, Graph* graph, std::vector<int>& postOrder, std::vector<int>& tree);
    void AddToPostOrder(const std::vector<int>& tree, std::stack<int>& postOrder);
    std::stack<int> InitializePostOrder();
};

#endif