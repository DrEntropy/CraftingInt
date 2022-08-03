//
//  Token.h
//  tlox
//
//  Created by Ronald Legere on 7/25/22.
//

#ifndef Token_h
#define Token_h

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

#endif /* Token_h */
