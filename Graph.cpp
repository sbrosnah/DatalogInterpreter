#include "Graph.h"

std::string Graph::toString() {
    std::stringstream ss;
    ss << "Dependency Graph" << std::endl;
    for(std::map<int, std::set<int>>::iterator it = edges.begin(); it != edges.end(); it++){
        ss << "R" << it->first << ":";
        std::set<int>::iterator itr = it->second.begin();
        int setSize = it->second.size();
        for(int i = 0; i < setSize; i++){
            ss << "R" << std::to_string(*itr);
            if(i < setSize - 1){
                ss << ",";
            }
            itr++;
        }
        ss << std::endl;
    }
    ss << std::endl;
    return ss.str();
}
