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

std::map<TokenType, std::string> tokenStrings {
    {TokenType::LEFT_PAREN, "LEFT_PAREN"}, {TokenType::RIGHT_PAREN, "RIGHT_PAREN"},
    {TokenType::LEFT_BRACE, "LEFT_BRACE"}, {TokenType::RIGHT_BRACE, "RIGHT_BRACE"},
    {TokenType::COMMA, "COMMA"}, {TokenType::DOT, "DOT"},
    {TokenType::MINUS, "MINUS"}, {TokenType::PLUS, "PLUS"},
    {TokenType::SEMICOLON, "SEMICOLON"}, {TokenType::SLASH, "SLASH"},
    {TokenType::STAR, "STAR"}, {TokenType::BANG, "BANG"},
    {TokenType::BANG_EQUAL, "BANG_EQUAL"}, {TokenType::EQUAL, "EQUAL"},
    {TokenType::EQUAL_EQUAL, "EQUAL_EQUAL"}, {TokenType::GREATER, "GREATER"},
    {TokenType::GREATER_EQUAL, "GREATER_EQUAL"}, {TokenType::LESS, "LESS"},
    {TokenType::LESS_EQUAL, "LESS_EQUAL"}, {TokenType::IDENTIFIER, "IDENTIFIER"},
    {TokenType::STRING, "STRING"}, {TokenType::NUMBER, "NUMBER"},
    {TokenType::AND, "AND"}, {TokenType::CLASS, "CLASS"},
    {TokenType::ELSE, "ELSE"}, {TokenType::FALSE_T, "FALSE_T"},
    {TokenType::FUN, "FUN"}, {TokenType::FOR, "FOR"},
    {TokenType::IF, "IF"}, {TokenType::NIL, "NIL"},
    {TokenType::OR, "OR"}, {TokenType::PRINT, "PRINT"},
    {TokenType::RETURN, "RETURN"}, {TokenType::SUPER, "SUPER"},
    {TokenType::THIS, "THIS"}, {TokenType::TRUE_T, "TRUE_T"},
    {TokenType::VAR, "VAR"}, {TokenType::WHILE, "WHILE"},
    {TokenType::EOF_T, "EOF_T"}
};


// keywords . i realize this is redundant , fix later.

std::map<std::string, TokenType> keywords {
    {"and",    TokenType::AND},
    {"class",  TokenType::CLASS},
    {"else",   TokenType::ELSE},
    {"false",  TokenType::FALSE_T},
    {"for",    TokenType::FOR},
    {"fun",    TokenType::FUN},
    {"if",     TokenType::IF},
    {"nil",    TokenType::NIL},
    {"or",     TokenType::OR},
    {"print",  TokenType::PRINT},
    {"return", TokenType::RETURN},
    {"super",  TokenType::SUPER},
    {"this",   TokenType::THIS},
    {"true",   TokenType::TRUE_T},
    {"var",    TokenType::VAR},
    {"while",  TokenType::WHILE}
};

using Literal = std::variant<std::monostate, double, std::string>;


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
    Token(TokenType type, std::string lexeme, Literal literal, int line):
    type{type}, lexeme{lexeme}, literal{literal}, line {line} {}
    
    std::string toString()
    {
        return tokenStrings[type] + " " + lexeme + " "  +
                std::visit(Literal_to_string_vis(), literal);
    }
    
private:
    int line;
    TokenType type;
    std::string lexeme;
    Literal literal;
};

#endif /* Token_h */
