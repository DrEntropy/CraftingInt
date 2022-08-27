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
    
    Scanner(std::string& code, std::function<void(int, std::string)> errorCallback ):source{code}, errorCallback {errorCallback} {
        
    }
    
    std::vector<Token> scanTokens()
    {
        while (!isAtEnd())
        {
            start = current;
            scanToken();
        }
        tokens.emplace_back(TokenType::EOF_T, "", Value(), line);
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
                if(std::isdigit(c))
                    number();
                else if(isAlpha(c))
                    identifier();
                else
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
    
    char peekNext()
    {
        if (current + 1 >= source.length()) return '\0';
            return source.at(current + 1);
    }
    
    void addToken(TokenType type)
    {
        addToken(type, Value());
    }
    
    void addToken(TokenType type, Value literal)
    {
        std::string text = source.substr(start, current-start);
        tokens.emplace_back(type, text, literal, line);
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
        while (peek() != '"' && !isAtEnd())
        {
           if (peek() == '\n') line++;
           advance();
        }
        
        if (isAtEnd()) {
          errorCallback(line, "Unterminated string.");
          return;
        }

        // The closing ".
        advance();

        // Trim the surrounding quotes.
        std::string value = source.substr(start + 1, current - start - 2);
        addToken(TokenType::STRING, value);
        
    }
    
    void number()
    {
        while (std::isdigit(peek())) advance();

        // Look for a fractional part.
        if (peek() == '.' && std::isdigit(peekNext())) {
          // Consume the "."
          advance();

          while (std::isdigit(peek())) advance();
        }

        addToken(TokenType::NUMBER,
            std::stod(source.substr(start, current-start)));
    }
    
    void identifier()
    {
          while (isAlphaNumeric(peek())) advance();
        
          std::string text = source.substr(start, current-start);
          TokenType type;
          try
          {
             type = keywords.at(text);
          }
          catch (const std::out_of_range&)
          {
            type = TokenType::IDENTIFIER;
          }
     
          addToken(type);
 
    }
    
    bool isAtEnd()
    {
        return current >= source.length();
    }
    
// just to make sure i know what is what, i know c++ defines these but ..
    bool isAlpha(char c) {
      return (c >= 'a' && c <= 'z') ||
             (c >= 'A' && c <= 'Z') ||
              c == '_';
    }

    bool isAlphaNumeric(char c) {
      return isAlpha(c) || std::isdigit(c);
    }
    
    
// member vars
    std::string& source;
    std::function<void(int, std::string)> errorCallback;
    std::vector<Token> tokens;
    
    int start{0};
    int current{0};
    int line{1};
};


#endif /* Scanner_h */
