#include "ColonDashAutomaton.h"

void ColonDashAutomaton::S0(const std::string& input) {
    index = 0;
    if (input[index] == ':') {
        inputRead = 1;
        index++;
        S1(input);
    }
    else {
        Serr();
    }
}

void ColonDashAutomaton::S1(const std::string& input) {
    if (input[index] == '-') {
        inputRead++;
    }
    else {
        Serr();
    }
}