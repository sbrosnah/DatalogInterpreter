#include "ID_Automaton.h"

void ID_Automaton::S0(const std::string &input) {
    if(isalpha(input[index])){
        S1(input);
    } else {
        Serr();
    }
}

void ID_Automaton::S1(const std::string &input) {
    for(int i = index; i < static_cast<int>(input.size()); i++) {
        index++;
        if(std::isalpha(input[i]) || std::isdigit(input[i])){
            inputRead++;
        } else {
            break;
        }
    }

}