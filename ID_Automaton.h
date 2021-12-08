#ifndef ID_AUTOMATON
#define ID_AUTOMATON

#include "Automaton.h"

class ID_Automaton : public Automaton
{
private:
    void S1(const std::string& input);
public:
    ID_Automaton() : Automaton(TokenType::ID) {}  // Call the base constructor
    virtual ~ID_Automaton(){}


    void S0(const std::string& input);

};

#endif