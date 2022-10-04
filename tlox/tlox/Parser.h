//
//  Parser.hpp
//  tlox
//
//  Created by Ronald Legere on 8/24/22.
//

#ifndef Parser_hpp
#define Parser_hpp

#include "Expr.h"
#include "Stmt.h"
#include <vector>


class ParseError {}; // placeholder

class Parser
{
public:
    Parser(std::vector<Token> tokens, std::function<void(Token, std::string)> errorCallback):tokens{tokens}, errorCallback{errorCallback}{};
    
    //std::shared_ptr<Expr> parse();
    std::vector<std::unique_ptr<Stmt>> parse();
    
private:
    
    std::shared_ptr<Expr> expression();
    std::shared_ptr<Expr> assignment();
    std::shared_ptr<Expr> orExpression();
    std::shared_ptr<Expr> andExpression();
    std::shared_ptr<Expr> equality();
    std::shared_ptr<Expr> comparison();
    std::shared_ptr<Expr> term();
    std::shared_ptr<Expr> factor();
    std::shared_ptr<Expr> unary();
    std::shared_ptr<Expr> primary();
    
    std::unique_ptr<Stmt> declaration(bool breakable);
    std::unique_ptr<Stmt> varDeclaration();
    std::unique_ptr<Stmt> ifStatement();
    std::unique_ptr<Stmt> whileStatement();
    std::unique_ptr<Stmt> breakStatement();
    std::unique_ptr<Stmt> forStatement();
    std::unique_ptr<Stmt> statement(bool breakable);
    std::unique_ptr<Stmt> expressionStatement();
    std::unique_ptr<Stmt> printStatement();
    
    std::vector<std::shared_ptr<Stmt>> blockStatements();
    
    bool match(std::initializer_list<TokenType>);
    bool check(TokenType);
    
    void synchronize();
    ParseError error(Token token, std::string message);
    
    Token advance()
    {
        if (!isAtEnd()) current++;
            return previous();
    }
    
    bool isAtEnd()
    {
        return peek().type == TokenType::EOF_T;
    }
    
    Token peek()
    {
        return tokens.at(current);
    }
    
    Token previous()
    {
        return tokens.at(current-1);
    }
    
    Token consume(TokenType type, std::string message); 
    
    std::vector<Token> tokens;
    int current{0};
    
    std::function<void(Token, std::string)> errorCallback;
    
};

#endif /* Parser_hpp */
