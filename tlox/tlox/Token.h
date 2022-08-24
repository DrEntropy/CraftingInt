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

// NOTE: TRUE , FALSE and EOF tokens are different from the book due to c++ keywords.

enum class TokenType
{
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,
    BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,
    IDENTIFIER, STRING, NUMBER,
    AND, CLASS, ELSE, FALSE_T, FUN, FOR, IF, NIL, OR, PRINT, RETURN, SUPER, THIS, TRUE_T, VAR, WHILE,
    EOF_T
};

// there HAS to be an easier way!

extern std::map<TokenType, std::string> tokenStrings;


// keywords . i realize this is redundant , fix later.

extern std::map<std::string, TokenType> keywords;

using Value = std::variant<std::monostate, double, std::string>;


// Visitor for literal variant. If too much of this refactor
struct Literal_to_string_vis
{
    std::string operator()(std::monostate)
    {
        return "";  
    }
    
    std::string operator()(std::string astring) {
        return astring;
    }
    
    std::string operator()(double value)
    {
        return std::to_string(value);
    }
};

class Token
{
public:
    Token(TokenType type, std::string lexeme, Value literal, int line):
    type{type}, lexeme{lexeme}, literal{literal}, line {line} {}
    
    std::string toString()
    {
        return tokenStrings[type] + " " + lexeme + " "  +
                std::visit(Literal_to_string_vis(), literal);
    }
    
    std::string lexeme;
    
    int line;
    TokenType type;
    
    Value literal;
};

#endif /* Token_h */
