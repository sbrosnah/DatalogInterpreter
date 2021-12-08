#ifndef TOKEN_H
#define TOKEN_H
#include <string>

enum class TokenType {
    COLON,
    COLON_DASH,
    STRING,
    COMMA,
    COMMENT,
    PERIOD,
    Q_MARK,
    LEFT_PAREN,
    RIGHT_PAREN,
    MULTIPLY,
    ADD,
    SCHEMES,
    FACTS,
    RULES,
    QUERIES,
    ID,
    UNDEFINED,
    _EOF
};

class Token
{
public:
    TokenType type;
    std::string description;
    int line;

    Token(TokenType type, std::string description, int line);
    ~Token() {}
    std::string TokenToString(TokenType type);
    std::string ToString();
};

#endif // TOKEN_H

