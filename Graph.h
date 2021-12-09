#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <set>
#include <vector>
#include <string>
#include <sstream>

class Graph {
public:
    Graph(){};
    ~Graph(){};
    std::map<int, std::set<int>> edges;
    std::map<int, bool> visitedNodes;
    std::string toString();
};

#endif