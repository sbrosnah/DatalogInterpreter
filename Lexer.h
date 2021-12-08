#ifndef LEXER_H
#define LEXER_H
#include <vector>
#include <sstream>
#include "Automaton.h"
#include "Token.h"

class Lexer
{
private:
    std::vector<Automaton*> automata;
    int lineNumber = 0;

    void CreateAutomata();
    void DeleteAutomata();

    // TODO: add any other private methods here (if needed)

public:
    std::vector<Token*> tokens;
    Lexer();
    ~Lexer();

    void Run(std::string& input);
    void ToString();

};

#endif // LEXER_H

