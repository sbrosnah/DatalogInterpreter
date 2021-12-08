#include "Parser.h"

Parser::Parser(){
    this->helper = new ParserHelper();
    this->datalogP = new DatalogProgram();
}

Parser::~Parser(){
    delete datalogP;
    delete helper;
}

void Parser::ToString() {
    std::cout << datalogP->ToString();
}

void Parser::FilterTokens(const std::vector<Token*>& tokens) {
    for (auto& token : tokens) {
        if(token->type != TokenType::COMMENT && token->type != TokenType::UNDEFINED) {
            this->filteredTokens.push_back(token);
        }
    }
}

DatalogProgram* Parser::Run(const std::vector<Token *> &tokens) {
    FilterTokens(tokens);
    try{
        helper->ParseDatalogProgram(filteredTokens, datalogP);
        datalogP->success = true;
    } catch (int e) {
        int index = helper->GetCurrIndex();
        Token* currToken = filteredTokens.at(index);
        std::string errorString = currToken->ToString();
        datalogP->SetErrorString(errorString);
        //OutputError(e);
    }
    return datalogP;
}

void Parser::OutputError(int e) {
    std::cout << "error code: " << e << std::endl;
    if (e == 1){
        std::cout << "Missing SCHEMES token in ParseDatalogProgram()" << std::endl;
    } else if (e == 2){
        std::cout << "Missing COLON token after SCHEMES token in ParseDatalogProgram()" << std::endl;
    } else if (e == 3) {
        std::cout << "Missing FACTS token in ParseDatalogProgram()" << std::endl;
    } else if (e == 4) {
        std::cout << "Missing COLON token after FACTS token in ParseDatalogProgram()" << std::endl;
    } else if (e == 5) {
        std::cout << "Missing RULES token in ParseDatalogProgram()" << std::endl;
    } else if (e == 6) {
        std::cout << "Missing COLON token after RULES token in ParseDatalogProgram()" << std::endl;
    } else if (e == 7) {
        std::cout << "Missing QUERIES token in ParseDatalogProgram()" << std::endl;
    } else if (e == 8) {
        std::cout << "Missing COLON token after QUERIES token in ParseDatalogProgram()" << std::endl;
    } else if ( e ==  21) {
        std::cout << "Missing ID token in ParseScheme()" << std::endl;
    } else if (e == 22) {
        std::cout << "Missing LEFT_PAREN token in ParseScheme()" << std::endl;
    } else if (e == 23) {
        std::cout << "Missing second ID token in ParseScheme()" << std::endl;
    } else if (e==24) {
        std::cout << "Missing RIGHT_PAREN token in ParseScheme()" << std::endl;
    } else if (e==41) {
        std::cout << "Missing ID token in ParseFact()" << std::endl;
    } else if (e==42) {
        std::cout << "Missing LEFT_PAREN token in ParseFact()" << std::endl;
    } else if (e == 43) {
        std::cout << "Missing STRING token in ParseFact()" << std::endl;
    } else if (e == 44) {
        std::cout << "Missing RIGHT_PAREN token in ParseFact()" << std::endl;
    } else if (e == 45) {
        std::cout << "Missing PERIOD token in ParseFact()" << std::endl;
    } else if (e == 61) {
        std::cout << "Missing COLON_DASH token in ParseRule()" << std::endl;
    } else if (e == 62) {
        std::cout << "Missing PERIOD token in ParseRule()" << std::endl;
    } else if (e == 81) {
        std::cout << "Missing Q_MARK token in ParseQuery()" << std::endl;
    } else if (e == 231) {
        std::cout << "Missing ID token in ParseIdList()" << std::endl;
    } else if (e == 431) {
        std::cout << "Missing STRING token in ParseStringList()" << std::endl;
    } else if (e == 601) {
        std::cout << "Missing ID token in ParseHeadPredicate()" << std::endl;
    } else if (e == 602) {
        std::cout << "Missing LEFT_PAREN token in ParseHeadPredicate()" << std::endl;
    }else if (e == 603) {
        std::cout << "Missing ID token in ParseHeadPredicate()" << std::endl;
    }else if (e == 604) {
        std::cout << "Missing RIGHT_PAREN token in ParseHeadPredicate()" << std::endl;
    } else if (e == 611) {
        std::cout << "Missing ID token in ParsePredicate()" << std::endl;
    } else if (e == 612) {
        std::cout << "Missing LEFT_PAREN token in ParsePredicate()" << std::endl;
    } else if (e == 613) {
        std::cout << "Missing RIGHT_PAREN token in ParsePredicate()" << std::endl;
    } else if (e == 6121) {
        std::cout << "Token not STRING or ID in ParseParameter()" << std::endl;
    } else if (e == 100) {
        std::cout << "Error with to-string in DatalogProgram object" << std::endl;
    }
}

