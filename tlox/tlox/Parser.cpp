//
//  Parser.cpp
//  tlox
//
//  Created by Ronald Legere on 8/24/22.
//

#include "Parser.hpp"


std::shared_ptr<Expr> Parser::expression()
{
    
}

std::shared_ptr<Expr> Parser::equality()
{
    
}

std::shared_ptr<Expr> Parser::comparison()
{
    
}


std::shared_ptr<Expr> Parser::term()
{
    
}

std::shared_ptr<Expr> Parser::factor()
{
    
}

std::shared_ptr<Expr> Parser::unary()
{
    
}

std::shared_ptr<Expr> Parser::primary()
{
    
}



bool Parser::match(std::initializer_list<TokenType>)
{
    
}


bool Parser::check(TokenType)
{
    
    
}


ParseError Parser::error(Token token, std::string message)
{
    errorCallback(token, message);
    return ParseError();
}
