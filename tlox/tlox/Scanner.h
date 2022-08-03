//
//  Scanner.h
//  tlox
//
//  Created by Ronald Legere on 7/25/22.
//

#ifndef Scanner_h
#define Scanner_h

#include "Token.h"
#include <string>
#include <vector>

class Scanner
{
public:
    
    Scanner(std::string& code,std::function<void(int, std::string)> errorCallback ):source{code}, errorCallback {errorCallback} {
        
    }
    
    std::vector<Token> scanTokens()
    {
        while (!isAtEnd())
        {
            start = current;
            scanToken();
        }
        Token eofToken {TokenType::EOF_T, "", Literal(), line};
        tokens.push_back(eofToken);
        return tokens;
    }
    

private:
    void scanToken()
    {
        char c = advance();
        switch (c) {
            case '(': addToken(TokenType::LEFT_PAREN); break;
            case ')': addToken(TokenType::RIGHT_PAREN); break;
            case '{': addToken(TokenType::LEFT_BRACE); break;
            case '}': addToken(TokenType::RIGHT_BRACE); break;
            case ',': addToken(TokenType::COMMA); break;
            case '.': addToken(TokenType::DOT); break;
            case '-': addToken(TokenType::MINUS); break;
            case '+': addToken(TokenType::PLUS); break;
            case ';': addToken(TokenType::SEMICOLON); break;
            case '*': addToken(TokenType::STAR); break;
                
            //two character cases
            case '!':
                addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
                break;
            case '=':
                addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
                break;
            case '<':
                addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
                break;
            case '>':
                addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
                break;
                
            // the slash:
            case '/':
                if (match('/')) {
                   // A comment goes until the end of the line. TO BE TESTED
                    while (peek() != '\n' && !isAtEnd()) advance();
                } else {
                    addToken(TokenType::SLASH);
                }
                break;
            
            // whitespace:
            case ' ':
            case '\r':
            case '\t':
            // Ignore whitespace.
                break;

             case '\n':
               line++;
               break;
                
            // literals
            case '"': string(); break;
                
            default:
                errorCallback(line, "Unexpected Character.");
                break;
          }
    }
    
    char advance()
    {
        return source.at(current++);
    }
    
    char peek()
    {
      if (isAtEnd()) return '\0';  // end of the line
      return source.at(current);
    }
    
    void addToken(TokenType type)
    {
        addToken(type, Literal());
    }
    
    void addToken(TokenType type, Literal literal)
    {
        std::string text = source.substr(start, current);
        tokens.push_back(Token{type, text, literal, line});
    }
    
    // look ahead 1
    bool match(char expected)
    {
        if (isAtEnd()) return false;
        if (source.at(current) != expected) return false;
        current++; //consume
        return true;
      }
    
    
    void string()
    {
        // TODO
        
    }
    
    bool isAtEnd()
    {
        return current >= source.length();
    }
    
    std::string& source;
    std::function<void(int, std::string)> errorCallback;
    std::vector<Token> tokens;
    
    int start{0};
    int current{0};
    int line{1};
};


#endif /* Scanner_h */
