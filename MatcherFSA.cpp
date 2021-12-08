#include "MatcherFSA.h"

void MatcherFSA::S0(const std::string &input) {
    /*
    if(input[index] == '.'){
        this->type = TokenType::PERIOD;
        inputRead++;
    } else if (input[index] == '?'){
        this->type = TokenType::Q_MARK;
        inputRead++;
    } else if (input[index] == '('){
        this->type = TokenType::LEFT_PAREN;
        inputRead++;
    } else if (input[index] == ')'){
        this->type = TokenType::RIGHT_PAREN;
        inputRead++;
    } else if (input[index] == '*') {
        this->type = TokenType::MULTIPLY;
        inputRead++;
    } else if (input[index] == '+'){
        this->type = TokenType::ADD;
        inputRead++;
    } else {
        Serr();
    }
     */
    bool isMatch = false;
    for(int i = 0; i < static_cast<int>(toMatch.size()); i++) {
        if(input[i] == toMatch[i]) {
            inputRead++;
            if(input.substr(0, i + 1) == toMatch) {
                isMatch = true;
            }
            if(isMatch == true) {
                break;
            }
        } else {
            Serr();
            break;
        }
    }
}
