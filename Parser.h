#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <iostream>
#include "Token.h"
#include "ParserHelper.h"
#include "DatalogProgram.h"

class Parser {
public:
    Parser();
    ~Parser();
    //TODO: Modify this to return a Datalog object
    DatalogProgram* Run(const std::vector<Token*>& tokens);
    std::vector<Token*> filteredTokens;
    void ToString();
private:
    void FilterTokens(const std::vector<Token*>& tokens);
    ParserHelper* helper;
    void OutputError(int e);
    DatalogProgram* datalogP;
};

#endif