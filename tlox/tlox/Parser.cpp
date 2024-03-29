//
//  Parser.cpp
//  tlox
//
//  Created by Ronald Legere on 8/24/22.
//

#include "Parser.h"


std::vector<std::unique_ptr<Stmt>> Parser::parse()
{
    std::vector<std::unique_ptr<Stmt>> statements{};
    while (!isAtEnd())
        statements.push_back(declaration(false));
    
    return statements;
}

std::shared_ptr<Expr> Parser::expression()
{
    return assignment();
}

std::shared_ptr<Expr> Parser::assignment()
{
 
    auto expr = orExpression();

    if (match({TokenType::EQUAL})) {
      Token equals = previous();
      auto value = assignment();
      
      
      if (auto var = dynamic_cast<Variable * > (expr.get())) {
        Token name = var->name;
        return std::make_shared<Assign>(name, value);
      }

      error(equals, "Invalid assignment target.");
    }

    return expr;
}

std::shared_ptr<Expr> Parser::orExpression()
{
    auto expr = andExpression();
    while (match({TokenType::OR})) {
       Token op  = previous();
       auto right = andExpression();
       expr = std::make_shared<Logical>(expr, op, right);
     }
    return expr;
}

std::shared_ptr<Expr> Parser::andExpression()
{
    auto expr = equality();
    while (match({TokenType::AND})) {
       Token op  = previous();
       auto right = equality();
       expr = std::make_shared<Logical>(expr, op, right);
     }
    return expr;
    
}

std::shared_ptr<Expr> Parser::equality()
{
    auto expr = comparison();
    
    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL}))
    {
        Token op = previous();
        auto right = comparison();
        expr = std::make_shared<Binary>(expr, op, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::comparison()
{
    auto expr = term();
    
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
    auto expr = factor();
    
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
    auto expr = unary();
    
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
        auto right = unary();
        return std::make_shared<Unary>(op, right);
    }
    return call();
}

std::shared_ptr<Expr> Parser::finishCall(std::shared_ptr<Expr> callee)
{
    std::vector< std::shared_ptr<Expr> > arguments;
    if(!check(TokenType::RIGHT_PAREN)) {
        do {
            if(arguments.size() >= 255){
                error(peek(), "Can't have more then 255 arguments.");
            }
            arguments.push_back(expression());
        } while (match({TokenType::COMMA}));
    }
    auto paren = consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");
    return std::make_shared<Call>(callee, paren, arguments);
}

std::shared_ptr<Expr> Parser::call()
{
    auto expr = primary();
    
    while(true)
    {
        if (match({TokenType::LEFT_PAREN}))
            expr = finishCall(expr);
        else
            break;
    }
    return expr;
}


std::shared_ptr<Expr> Parser::primary()
{
    if (match({TokenType::FUN})) return anonFunctionExpr();
    if (match({TokenType::FALSE_T})) return std::make_shared<Literal>(false);
    if (match({TokenType::TRUE_T})) return std::make_shared<Literal>(true);
    if (match({TokenType::NIL})) return std::make_shared<Literal>(Value()); // monostate is nil
    
    if (match({TokenType::NUMBER,TokenType::STRING})) return std::make_shared<Literal>(previous().literal);
    
    if (match({TokenType::IDENTIFIER})) return std::make_shared<Variable>(previous());
    
    if (match({TokenType::LEFT_PAREN}))
    {
        auto grouped = expression();
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

std::shared_ptr<Expr> Parser::anonFunctionExpr()
{
    consume(TokenType::LEFT_PAREN, "Expect '(' after fun in anonymouse function.");
    std::vector<Token> parameters{};
    if(!check(TokenType::RIGHT_PAREN))
    {
        do
        {
            if(parameters.size() >= 255)
            {
                error(peek(), "Can't have more then 255 parameters.");
            }
            parameters.push_back(consume(TokenType::IDENTIFIER ,"Expect parameter name."));
            
        } while(match({TokenType::COMMA}));
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");
    
    //parse body
    
    consume(TokenType::LEFT_BRACE, "expect '{' before function body.");
    auto body = blockStatements(false);
    
    return std::make_unique<AnonFunction>(parameters, body);
    // Now build function node for AST
    
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

std::unique_ptr<Stmt> Parser::statement(bool breakable)
{
    // NOTE parse errors are not caught so crash the program for now.
    
    if(match({TokenType::FOR}))
        return forStatement();
    if(match({TokenType::IF}))
        return ifStatement(breakable);
    if(match({TokenType::PRINT}))
        return printStatement();
    if(match({TokenType::RETURN}))
        return returnStatement();
    if(match({TokenType::WHILE}))
        return whileStatement();
    if(match({TokenType::LEFT_BRACE}))
        return std::make_unique<Block>(blockStatements(breakable));
    if(breakable and match({TokenType::BREAK}))
        return breakStatement();
    return expressionStatement();
        
}


// Note that using shared_ptr here is a crutch to avoid complicating my AST generating code.
std::vector<std::shared_ptr<Stmt>> Parser::blockStatements(bool breakable)
{
    std::vector<std::shared_ptr<Stmt> > statements;

     while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
       statements.emplace_back(declaration(breakable));
     }

     consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
     return statements;
}


std::unique_ptr<Stmt> Parser::breakStatement()
{
    consume(TokenType::SEMICOLON, "Expected ';' after break statement");
    return std::make_unique<Break>();
}

std::unique_ptr<Stmt> Parser::returnStatement()
{
    Token keyword = previous();
    std::shared_ptr<Expr> value;
    if(!check(TokenType::SEMICOLON))
    {
        value = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after return value.");
    return std::make_unique<Return>(keyword, value);
}

std::unique_ptr<Stmt> Parser::printStatement()
{
    auto value = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after Value.");
    return std::make_unique<Print>(value);
        
}

// umm, its really almost the same
std::unique_ptr<Stmt> Parser::expressionStatement()
{
    auto value = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after Value.");
    return std::make_unique<ExprStmt>(value);
        
}

std::unique_ptr<Stmt> Parser::declaration(bool breakable)
{
    try
    {
        if(match({TokenType::FUN}))
            return function("function");
        if(match({TokenType::VAR}))
            return varDeclaration();
        return statement(breakable);
    
    }
    catch (ParseError error)
    {
          //synchronize();
          return nullptr;
    }
}

std::unique_ptr<Stmt> Parser::function(std::string kind)
{
    Token name = consume(TokenType::IDENTIFIER, "Expect "+ kind+ " name.");
    consume(TokenType::LEFT_PAREN, "Expect '(' after " + kind +" name.");
    std::vector<Token> parameters{};
    if(!check(TokenType::RIGHT_PAREN))
    {
        do
        {
            if(parameters.size() >= 255)
            {
                error(peek(), "Can't have more then 255 parameters.");
            }
            parameters.push_back(consume(TokenType::IDENTIFIER ,"Expect parameter name."));
            
        } while(match({TokenType::COMMA}));
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");
    
    //parse body
    
    consume(TokenType::LEFT_BRACE, "expect '{' before "+ kind + " body.");
    auto body = blockStatements(false);
    
    return std::make_unique<Function>(name, parameters, body);
    // Now build function node for AST
    
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

std::unique_ptr<Stmt> Parser::ifStatement(bool breakable)
{
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
    std::shared_ptr<Expr> condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");

    std::unique_ptr<Stmt> thenBranch = statement(breakable);
    std::unique_ptr<Stmt> elseBranch;  // null
    if (match({TokenType::ELSE})) {
          elseBranch = statement(breakable);
        }

    return std::make_unique<If>(condition, std::move(thenBranch), std::move(elseBranch));
    
}

std::unique_ptr<Stmt> Parser::whileStatement()
{
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
    auto condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
    auto body = statement(true);

    return std::make_unique<While>(condition, std::move(body));
}


std::unique_ptr<Stmt> Parser::forStatement()
{
    // build a while expression (desugar)
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'for'.");
    
    //Initializer
    std::unique_ptr<Stmt> initializer;
    if (match({TokenType::SEMICOLON})) {
          // null initilzer
    } else if (match({TokenType::VAR})) {
          initializer = varDeclaration();
    } else {
          initializer = expressionStatement();
    }
    
    // Condition
    std::shared_ptr<Expr> condition;
    if (!check(TokenType::SEMICOLON))
    {
      condition = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after loop condition.");
    
    // Increment
    std::shared_ptr<Expr> increment;
    if (!check(TokenType::RIGHT_PAREN))
    {
      increment = expression();
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after for clauses.");
    
    // Body:
    auto body = statement(true);
    if (increment)
    {
        std::vector<std::shared_ptr<Stmt> > parts;
        parts.push_back(std::move(body));  // body is unique ptr.
        parts.push_back(std::make_shared<ExprStmt>(increment));
        body = std::make_unique<Block>(parts);
    }
    
    // build while loop
    if(!condition)
        condition = std::make_shared<Literal>(true);
    
    body = std::make_unique<While>(condition, std::move(body));
    
    // deal with initializer.
    if (initializer)
    {
        std::vector<std::shared_ptr<Stmt> > parts;
        parts.push_back(std::move(initializer));
        parts.push_back(std::move(body));
        body = std::make_unique<Block>(parts);
    }
    return body;
}


