#include "CommentAutomaton.h"

void CommentAutomaton::S0(const std::string& input) {
    if(input[index] == '#'){
        index++;
        inputRead++;
        S1(input);
    } else {
        Serr();
    }
}

void CommentAutomaton::S1(const std::string &input) {
    if(input[index] == '|'){
        Serr();
    } else {
        S2(input);
    }
}

void CommentAutomaton::S2(const std::string &input) {
    if(input[index] != '\n'){
        inputRead++;
        index++;
        S2(input);
    }
}