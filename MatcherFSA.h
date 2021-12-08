#ifndef MATCHERFSA_H
#define MATCHERFSA_H

#include "Automaton.h"

class MatcherFSA : public Automaton
{
private:
    std::string toMatch;
public:
    MatcherFSA(std::string toMatch, TokenType token) : Automaton() {this->toMatch = toMatch; this->type = token;}  // Call the base constructor
    virtual ~MatcherFSA() {}


    void S0(const std::string& input);

};

#endif // MATCHERFSA_H