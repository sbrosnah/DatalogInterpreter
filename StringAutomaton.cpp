#include "StringAutomaton.h"

void StringAutomaton::S0(const std::string &input) {
    if(input[index] == '\'') {
        index++;
        inputRead++;
        S1(input);
    } else {
        Serr();
    }
}

void StringAutomaton::S1(const std::string &input) {

    if(input[index] == '\'' && (input[index + 1] != '\'')){
        //our string is done
        inputRead++;
    } else if(input[index] == '\'' && input[index + 1] == '\''){
        //it is an apostrophe and our string isn't done
        index +=2;
        inputRead +=2;
        S1(input);
    } else if(index >= (static_cast<int>(input.size()) - 1)){
        this->type = TokenType::UNDEFINED;
        if(input[index] == '\n'){
            newLines++;
        } else {
            inputRead++;
        }

    } else {
        if(input[index] == '\n') {
            newLines++;
        } else {
            inputRead++;
        }
        index++;
        S1(input);
    }
}

void StringAutomaton::S2(const std::string &input) {


}
