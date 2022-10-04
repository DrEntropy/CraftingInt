//
//  Token.cpp
//  tlox
//
//  Created by Ronald Legere on 8/17/22.
//

#include "Token.h"


std::map<TokenType, std::string> tokenStrings = {
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
    {TokenType::EOF_T, "EOF_T"}, {TokenType::BREAK, "BREAK"}
};


// keywords . i realize this is redundant , fix later.

std::map<std::string, TokenType> keywords = {
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
    {"while",  TokenType::WHILE},
    {"break",  TokenType::BREAK}
};
