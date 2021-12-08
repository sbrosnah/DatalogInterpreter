#include "Token.h"
#include <sstream>

Token::Token(TokenType type, std::string description, int line) {
    this->type = type;
    this->description = description;
    this->line = line;
}

std::string Token::TokenToString(TokenType type) {
    switch(type) {
        case TokenType::COLON: return "COLON"; break;
        case TokenType::PERIOD: return "PERIOD"; break;
        case TokenType::Q_MARK: return "Q_MARK"; break;
        case TokenType::QUERIES: return "QUERIES"; break;
        case TokenType::MULTIPLY: return "MULTIPLY"; break;
        case TokenType::ADD: return "ADD"; break;
        case TokenType::LEFT_PAREN: return "LEFT_PAREN"; break;
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN"; break;
        case TokenType::COMMA: return "COMMA"; break;
        case TokenType::COLON_DASH: return "COLON_DASH"; break;
        case TokenType::SCHEMES: return "SCHEMES"; break;
        case TokenType::FACTS: return "FACTS"; break;
        case TokenType::RULES: return "RULES"; break;
        case TokenType::STRING: return "STRING"; break;
        case TokenType::COMMENT: return "COMMENT"; break;
        case TokenType::ID: return "ID"; break;
        case TokenType::_EOF: return "EOF"; break;
        case TokenType::UNDEFINED: return "UNDEFINED"; break;
        default: return "UNDEFINED"; break;
    }
}

std::string Token::ToString() {
    std::stringstream oss;
    oss << "(" << TokenToString(type) << ",\"" << description << "\"," << line << ")";
    return oss.str();
}
