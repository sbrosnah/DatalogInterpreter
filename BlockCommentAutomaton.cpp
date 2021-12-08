#include "BlockCommentAutomaton.h"

void BlockCommentAutomaton::S0(const std::string& input) {
    if(input[index] == '#') {
        inputRead++;
        index++;
        S1(input);
    } else {
        Serr();
    }
}

void BlockCommentAutomaton::S1(const std::string& input) {
    if(input[index] == '|'){
        inputRead++;
        index++;
        S2(input);
    } else {
        Serr();
    }
}

void BlockCommentAutomaton::S2(const std::string &input) {
    if(input[index] != '|'){
        if(input[index] == '\n'){
            newLines++;
        } else {
            inputRead++;
        }
        index++;
        if(index < static_cast<int>(input.size())){
            S2(input);
        } else if(index >= static_cast<int>(input.size())){
            this->type = TokenType::UNDEFINED;
        }
    } else {
        index++;
        inputRead++;
        if(index < static_cast<int>(input.size())){
            S3(input);
        } else if(index >= static_cast<int>(input.size())){
            this->type = TokenType::UNDEFINED;
        }
    }
}

void BlockCommentAutomaton::S3(const std::string &input) {
    if(input[index] == '#'){
        inputRead++;
    } else {
        S2(input);
    }
}