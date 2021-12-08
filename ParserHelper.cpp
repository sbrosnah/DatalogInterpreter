#include "ParserHelper.h"
#include <iostream>
using namespace std;

void ParserHelper::ParseDatalogProgram(const std::vector<Token*>& filteredTokens, DatalogProgram*& DatalogP) {
    datalogProgram = DatalogP;
    tokens = filteredTokens;
    if(tokens.at(currIndex)->type == TokenType::SCHEMES){
        currIndex++;
    } else {
        throw 1;
    }
    if(tokens.at(currIndex)->type == TokenType::COLON){
        currIndex++;
    } else {
        throw 2;
    }
    ParseScheme();
    ParseSchemeList();
    if(tokens.at(currIndex)->type == TokenType::FACTS){
        currIndex++;
    } else {
        throw 3;
    }
    if(tokens.at(currIndex)->type == TokenType::COLON){
        currIndex++;
    } else {
        throw 4;
    }
    ParseFactList(); //Parse fact inside of this
    if(tokens.at(currIndex)->type == TokenType::RULES){
        currIndex++;
    } else {
        throw 5;
    }
    if(tokens.at(currIndex)->type == TokenType::COLON){
        currIndex++;
    } else {
        throw 6;
    }
    ParseRuleList(); //Parse rule inside of this
    if(tokens.at(currIndex)->type == TokenType::QUERIES){
        currIndex++;
    } else {
        throw 7;
    }
    if(tokens.at(currIndex)->type == TokenType::COLON){
        currIndex++;
    }else {
        throw 8;
    }
    ParseQuery();
    ParseQueryList();
    if(tokens.at(currIndex)->type != TokenType::_EOF){
        throw 9;
    }
}

void ParserHelper::ParseScheme() {
    Predicate* scheme = new Predicate();
    datalogProgram->schemes.push_back(scheme);
    if (tokens.at(currIndex)->type == TokenType::ID) {
        scheme->id = tokens.at(currIndex)->description;
        currIndex++;
    } else {
        throw 21;
    }
    if (tokens.at(currIndex)->type == TokenType::LEFT_PAREN) {
        currIndex++;
    } else {
        throw 22;
    }
    if (tokens.at(currIndex)->type == TokenType::ID) {
        Parameter* parameter = new Parameter();
        parameter->p = tokens.at(currIndex)->description;
        scheme->parameters.push_back(parameter);
        currIndex++;
    } else {
        throw 23;
    }
    ParseIdList(scheme);
    if (tokens.at(currIndex)->type == TokenType::RIGHT_PAREN) {
        currIndex++;
    } else {
        throw 24;
    }
}

int ParserHelper::GetCurrIndex() {
    return currIndex;
}

void ParserHelper::ParseSchemeList() {
    if(tokens.at(currIndex)->type == TokenType::ID){
        ParseScheme();
        ParseSchemeList();
    }
}
void ParserHelper::ParseFactList() {
    if(tokens.at(currIndex)->type != TokenType::RULES){
        ParseFact();
        ParseFactList();
    }
}
void ParserHelper::ParseFact() {
    Predicate* fact = new Predicate();
    if(tokens.at(currIndex)->type == TokenType::ID){
        fact->id = tokens.at(currIndex)->description;
        currIndex++;
    }else {
        throw 41;
    }
    if(tokens.at(currIndex)->type == TokenType::LEFT_PAREN){
        currIndex++;
    } else {
        throw 42;
    }
    if(tokens.at(currIndex)->type == TokenType::STRING) {
        Parameter* parameter = new Parameter();
        parameter->p = tokens.at(currIndex)->description;
        fact->parameters.push_back(parameter);
        currIndex++;
    } else {
        throw 43;
    }
    ParseStringList(fact);
    for(long unsigned int i = 0; i < fact->parameters.size(); i++) {
        datalogProgram->domains.insert(fact->parameters.at(i)->p);
    }
    if(tokens.at(currIndex)->type == TokenType::RIGHT_PAREN){
        currIndex++;
    } else {
        throw 44;
    }
    if(tokens.at(currIndex)->type == TokenType::PERIOD){
        currIndex++;
    } else {
        throw 45;
    }
    datalogProgram->facts.push_back(fact);
}
void ParserHelper::ParseRuleList() {
    if(tokens.at(currIndex)->type != TokenType::QUERIES){
        ParseRule();
        ParseRuleList();
    }
}
void ParserHelper::ParseRule() {
    Rule* rule = new Rule();
    datalogProgram->rules.push_back(rule);
    ParseHeadPredicate(rule->headPredicate);
    if(tokens.at(currIndex)->type == TokenType::COLON_DASH){
        currIndex++;
    } else {
        throw 61;
    }
    Predicate* predicate = new Predicate();
    rule->bodyPredicates.push_back(predicate);
    ParsePredicate(predicate);
    ParsePredicateList(rule->bodyPredicates);
    if(tokens.at(currIndex)->type == TokenType::PERIOD){
        currIndex++;
    } else {
        throw 62;
    }
}
void ParserHelper::ParseQuery() {
    Predicate* predicate = new Predicate();
    datalogProgram->queries.push_back(predicate);
    ParsePredicate(predicate);
    if(tokens.at(currIndex)->type == TokenType::Q_MARK){
        currIndex++;
    } else {
        throw 81;
    }
}
void ParserHelper::ParseQueryList() {
    if(tokens.at(currIndex)->type == TokenType::ID) {
        ParseQuery();
        ParseQueryList();
    }
}
void ParserHelper::ParseIdList(Predicate*& predicate) {
    if(tokens.at(currIndex)->type == TokenType::COMMA){
        currIndex++;
        if(tokens.at(currIndex)->type == TokenType::ID){
            Parameter* parameter = new Parameter();
            parameter->p = tokens.at(currIndex)->description;
            predicate->parameters.push_back(parameter);
            currIndex++;
            ParseIdList(predicate);
        } else {
            throw 231;
        }
    }
}
void ParserHelper::ParseStringList(Predicate*& predicate) {
    if(tokens.at(currIndex)->type == TokenType::COMMA){
        currIndex++;
        if(tokens.at(currIndex)->type == TokenType::STRING){
            Parameter* parameter = new Parameter();
            parameter->p = tokens.at(currIndex)->description;
            predicate->parameters.push_back(parameter);
            currIndex++;
            ParseStringList(predicate);
        } else {
            throw 431;
        }
    }
}
void ParserHelper::ParseHeadPredicate(Predicate*& headPredicate){
    Predicate* predicate = new Predicate();
    headPredicate = predicate;
    if(tokens.at(currIndex)->type == TokenType::ID){
        headPredicate->id = tokens.at(currIndex)->description;
        currIndex++;
    } else {
        throw 601;
    }
    if(tokens.at(currIndex)->type == TokenType::LEFT_PAREN){
        currIndex++;
    } else {
        throw 602;
    }
    if(tokens.at(currIndex)->type == TokenType::ID){
        Parameter* parameter = new Parameter();
        parameter->p = tokens.at(currIndex)->description;
        headPredicate->parameters.push_back(parameter);
        currIndex++;
    } else {
        throw 603;
    }
    ParseIdList(headPredicate);
    if(tokens.at(currIndex)->type == TokenType::RIGHT_PAREN){
        currIndex++;
    } else {
        throw 604;
    }
}
void ParserHelper::ParsePredicate(Predicate*& predicate) {
    if(tokens.at(currIndex)->type != TokenType::PERIOD){
        if(tokens.at(currIndex)->type == TokenType::ID){
            predicate->id = tokens.at(currIndex)->description;
            currIndex++;
        } else {
            throw 611;
        }
        if(tokens.at(currIndex)->type == TokenType::LEFT_PAREN){
            currIndex++;
        } else {
            throw 612;
        }
        Parameter* parameter = new Parameter();
        predicate->parameters.push_back(parameter);
        ParseParameter(parameter);
        ParseParameterList(predicate->parameters);
        if(tokens.at(currIndex)->type == TokenType::RIGHT_PAREN){
            currIndex++;
        } else {
            throw 613;
        }
    }
}
void ParserHelper::ParsePredicateList(std::vector<Predicate*>& bodyPredicates){
    if(tokens.at(currIndex)->type == TokenType::COMMA){
        currIndex++;
        Predicate* predicate = new Predicate();
        bodyPredicates.push_back(predicate);
        ParsePredicate(predicate);
        ParsePredicateList(bodyPredicates);
    }
}
void ParserHelper::ParseParameter(Parameter*& parameter){
    if(tokens.at(currIndex)->type == TokenType::STRING){
        parameter->p = tokens.at(currIndex)->description;
        currIndex++;
    } else if (tokens.at(currIndex)->type == TokenType::ID){
        parameter->p = tokens.at(currIndex)->description;
        currIndex++;
    } else {
        throw 6121;
    }
}
void ParserHelper::ParseParameterList(std::vector<Parameter*>& parameters){
    if(tokens.at(currIndex)->type == TokenType::COMMA){
        currIndex++;
        Parameter* parameter = new Parameter();
        parameters.push_back(parameter);
        ParseParameter(parameter);
        ParseParameterList(parameters);
    }
}
