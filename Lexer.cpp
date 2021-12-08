#include "Lexer.h"
#include "ColonAutomaton.h"
#include "ColonDashAutomaton.h"
#include "StringAutomaton.h"
#include "CommaAutomaton.h"
#include "CommentAutomaton.h"
#include "BlockCommentAutomaton.h"
#include "MatcherFSA.h"
#include "ID_Automaton.h"
#include <iostream>
using namespace std;

Lexer::Lexer() {
    CreateAutomata();
}

Lexer::~Lexer() {
    // TODO: need to clean up the memory in `automata` and `tokens`
    DeleteAutomata();
    for(auto token : tokens) {
        delete token;
    }
}

void Lexer::DeleteAutomata() {
    for (auto a : automata) {
        delete a;
    }
}

void Lexer::CreateAutomata() {
    automata.push_back(new ColonAutomaton());
    automata.push_back(new ColonDashAutomaton());
    automata.push_back(new CommaAutomaton());
    automata.push_back(new MatcherFSA(".", TokenType::PERIOD));
    automata.push_back(new MatcherFSA("?", TokenType::Q_MARK));
    automata.push_back(new MatcherFSA("(", TokenType::LEFT_PAREN));
    automata.push_back(new MatcherFSA(")", TokenType::RIGHT_PAREN));
    automata.push_back(new MatcherFSA("*", TokenType::MULTIPLY));
    automata.push_back(new MatcherFSA("+", TokenType::ADD));
    automata.push_back(new MatcherFSA("Schemes", TokenType::SCHEMES));
    automata.push_back(new MatcherFSA("Facts", TokenType::FACTS));
    automata.push_back(new MatcherFSA("Queries", TokenType::QUERIES));
    automata.push_back(new MatcherFSA("Rules", TokenType::RULES));
    automata.push_back(new ID_Automaton());
    automata.push_back(new StringAutomaton());
    automata.push_back(new BlockCommentAutomaton);
    automata.push_back(new CommentAutomaton());
    // TODO: Add the other needed automata here
}

void Lexer::Run(std::string& input) {
    // TODO: convert this pseudo-code with the algorithm into actual C++ code

    this->lineNumber = 1;

    long unsigned int maxRead = 0;
    Automaton* maxAutomaton;
    long unsigned int inputRead = 0;

    while(input.size() > 0) {
        maxRead = 0;
        maxAutomaton = automata[0];
        /*
        while(input[0] == '\n') {
            input = input.substr(1);
            lineNumber++;
            if(input == "") {
                break;
            }
        }
         */
        while(input[0] == ' ' || input[0] == '\t' || input[0] == '\n'){
            if(input[0] == '\n'){
                lineNumber++;
            }
            input = input.substr(1);
            if(input == "") {
                break;
            }
        }
        if(input == ""){
            break;
        }

        for (auto automaton: automata) {
            inputRead = automaton->Start(input);
            inputRead += automaton->NewLinesRead();
            if (inputRead > maxRead) {
                maxRead = inputRead;
                maxAutomaton = automaton;
            }
        }
        string description = "";
        if (maxRead > 0) {
            description = input.substr(0,maxRead);
            Token *newToken = maxAutomaton->CreateToken(description, lineNumber);
            lineNumber += maxAutomaton->NewLinesRead();
            tokens.push_back(newToken);
            //cout << newToken->ToString() << endl;
        } else {
            maxRead = 1;
            description = input[0];
            //Token *newToken = maxAutomaton->CreateUndefinedToken(description,lineNumber);
            Token *token = new Token(TokenType::UNDEFINED, description, lineNumber);
            tokens.push_back(token);
            //cout << token->ToString() << endl;
        }
        input = input.substr(maxRead);
    }
    Token *eofToken = new Token(TokenType::_EOF, "", lineNumber);
    tokens.push_back(eofToken);
    //

}

void Lexer::ToString() {
    for(auto token : tokens) {
        cout << token->ToString() << endl;
    }
    cout << "Total Tokens = " << tokens.size();
}



/*
set lineNumber to 1
// While there are more characters to tokenize
loop while input.size() > 0 {
    set maxRead to 0
    set maxAutomaton to the first automaton in automata

    // TODO: you need to handle whitespace inbetween tokens

    // Here is the "Parallel" part of the algorithm
    //   Each automaton runs with the same input
    foreach automaton in automata {
        inputRead = automaton.Start(input)
        if (inputRead > maxRead) {
            set maxRead to inputRead
            set maxAutomaton to automaton
        }
    }
    // Here is the "Max" part of the algorithm
    if maxRead > 0 {
        set newToken to maxAutomaton.CreateToken(...)
            increment lineNumber by maxAutomaton.NewLinesRead()
            add newToken to collection of all tokens
    }
    // No automaton accepted input
    // Create single character undefined token
    else {
        set maxRead to 1
            set newToken to a  new undefined Token
            (with first character of input)
            add newToken to collection of all tokens
    }
    // Update `input` by removing characters read to create Token
    remove maxRead characters from input
}
add end of file token to all tokens
*/
