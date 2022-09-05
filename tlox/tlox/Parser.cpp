//
//  Parser.cpp
//  tlox
//
//  Created by Ronald Legere on 8/24/22.
//

#include "Parser.h"


std::shared_ptr<Expr> Parser::expression()
{
    return assignment();
}

std::shared_ptr<Expr> Parser::assignment()
{
 
    std::shared_ptr<Expr> expr = equality();

    if (match({TokenType::EQUAL})) {
      Token equals = previous();
      std::shared_ptr<Expr> value = assignment();
      
      
      if (auto var = dynamic_cast<Variable * > (expr.get())) {
        Token name = var->name;
        return std::make_shared<Assign>(name, value);
      }

      error(equals, "Invalid assignment target.");
    }

    return expr;
}

std::shared_ptr<Expr> Parser::equality()
{
    std::shared_ptr<Expr> expr = comparison();
    
    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL}))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = comparison();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::comparison()
{
    std::shared_ptr<Expr> expr = term();
    
    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL}))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = term();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
    
}


std::shared_ptr<Expr> Parser::term()
{
    std::shared_ptr<Expr> expr = factor();
    
    while (match({TokenType::MINUS, TokenType::PLUS}))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = factor();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
    
}

std::shared_ptr<Expr> Parser::factor()
{
    std::shared_ptr<Expr> expr = unary();
    
    while (match({TokenType::STAR, TokenType::SLASH }))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
    
}

std::shared_ptr<Expr> Parser::unary()
{
    if(match({TokenType::BANG, TokenType::MINUS}))
    {
        Token op = previous();
        std::shared_ptr<Expr> right = unary();
        return std::make_shared<Unary>(op, right);
    }
    return primary();
}

std::shared_ptr<Expr> Parser::primary()
{
    if (match({TokenType::FALSE_T})) return std::make_shared<Literal>(false);
    if (match({TokenType::TRUE_T})) return std::make_shared<Literal>(true);
    if (match({TokenType::NIL})) return std::make_shared<Literal>(Value()); // monostate is nil
    
    if (match({TokenType::NUMBER,TokenType::STRING})) return std::make_shared<Literal>(previous().literal);
    
    if (match({TokenType::IDENTIFIER})) return std::make_shared<Variable>(previous());
    
    if (match({TokenType::LEFT_PAREN}))
    {
        std::shared_ptr<Expr> grouped = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_shared<Grouping>(grouped);
    }
    
    if (peek().type == TokenType::SEMICOLON)
    {
        // null expression this is a place holder
        return std::make_shared<Literal>(Value());
    }
    // doesn't match anything, thats an error.
    throw error(peek(), "Expecting Expression.");
}


Token Parser::consume(TokenType type, std::string message)
{
    if (check(type)) return advance();
    throw error(peek(), message);
}


bool Parser::match(std::initializer_list<TokenType> types)
{
    for (TokenType type : types )
    {
        if(check(type))
        {
            advance();
            return true;
        }
    }
    
    return false;
}


bool Parser::check(TokenType type)
{
    if(isAtEnd()) return false;
    return peek().type == type;
}


ParseError Parser::error(Token token, std::string message)
{
    errorCallback(token, message);
    return ParseError();
}


void Parser::synchronize()
{
    advance();
    while (!isAtEnd()) {
       if (previous().type == TokenType::SEMICOLON) return;

       switch (peek().type) {
           case TokenType::CLASS:
           case TokenType::FUN:
           case TokenType::VAR:
           case TokenType::FOR:
           case TokenType::IF:
           case TokenType::WHILE:
           case TokenType::PRINT:
           case TokenType::RETURN:
               return;
           default:
               break;
       }

       advance();
     }
}

std::unique_ptr<Stmt> Parser::statement()
{
    // NOTE parse errors are not caught so crash the program for now.
    if(match({TokenType::PRINT}))
        return printStatement();
    
    return expressionStatement();
        
}

std::unique_ptr<Stmt> Parser::printStatement()
{
    std::shared_ptr<Expr> value = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after Value.");
    return std::make_unique<Print>(value);
        
}

// umm, its really almost the same
std::unique_ptr<Stmt> Parser::expressionStatement()
{
    std::shared_ptr<Expr> value = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after Value.");
    return std::make_unique<ExprStmt>(value);
        
}

std::unique_ptr<Stmt> Parser::declaration()
{
    try
    {
        if(match({TokenType::VAR}))
            return varDeclaration();
        return statement();
    
    }
    catch (ParseError error)
    {
          //synchronize();
          return nullptr;
    }
}

std::unique_ptr<Stmt> Parser::varDeclaration()
{
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
    std::shared_ptr<Expr> initializer;
    
    if (match({TokenType::EQUAL})) {
          initializer = expression();
    }

    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_unique<Var> (name, initializer);
}


std::vector<std::unique_ptr<Stmt>> Parser::parse()
{
    std::vector<std::unique_ptr<Stmt>> statements{};
    while (!isAtEnd())
        statements.push_back(declaration());
    
    return statements;
}



