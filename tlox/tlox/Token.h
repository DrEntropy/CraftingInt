//
//  Token.h
//  tlox
//
//  Created by Ronald Legere on 7/25/22.
//

#ifndef Token_h
#define Token_h


#include <variant>
#include <map>
#include <string>

#include "ValueHelpers.h"

// NOTE: TRUE , FALSE and EOF tokens are different from the book due to c++ keywords.

enum class TokenType
{
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,
    BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,
    IDENTIFIER, STRING, NUMBER,
    AND, CLASS, ELSE, FALSE_T, FUN, FOR, IF, NIL, OR, PRINT, RETURN, SUPER, THIS, TRUE_T, VAR, WHILE,
    EOF_T, BREAK
};

// there HAS to be an easier way!

extern std::map<TokenType, std::string> tokenStrings;


// keywords . i realize this is redundant , fix later.

extern std::map<std::string, TokenType> keywords;


class Token
{
public:
    Token(TokenType type, std::string lexeme, Value literal, int line):
    type{type}, lexeme{lexeme}, literal{literal}, line {line} {}
    
    std::string toString()
    {
        return tokenStrings[type] + " " + lexeme + " "  +
        Stringify(literal);
    }
    
    std::string lexeme;
    
    int line;
    TokenType type;
    
    Value literal;
};

#endif /* Token_h */
