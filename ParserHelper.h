#ifndef PARSER_HELPER_H
#define PARSER_HELPER_H
#include <vector>
#include "Token.h"
#include "Parameter.h"
#include "DatalogProgram.h"
#include "Predicate.h"
#include "Rule.h"

class ParserHelper {
public:
    ParserHelper(){};
    ~ParserHelper(){};
    void ParseScheme();
    void ParseDatalogProgram(const std::vector<Token*>& filteredTokens, DatalogProgram*& datalogP);
    void ParseSchemeList();
    void ParseFactList();
    void ParseFact();
    void ParseRuleList();
    void ParseRule();
    void ParseQueryList();
    void ParseQuery();
    void ParseIdList(Predicate*& predicate);
    void ParseStringList(Predicate*& predicate);
    void ParseHeadPredicate(Predicate*& predicate);
    void ParsePredicate(Predicate*& predicate);
    void ParsePredicateList(std::vector<Predicate*>& bodyPredicates);
    void ParseParameter(Parameter*& parameter);
    void ParseParameterList(std::vector<Parameter*>& parameters);
    int GetCurrIndex();
private:
    std::vector<Token *> tokens;
    int currIndex = 0;
    DatalogProgram* datalogProgram;
};

#endif