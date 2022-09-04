//
//  TreeEval.h
//  tlox
//
//  Created by Ronald Legere on 8/26/22.
//

#ifndef TreeEval_h
#define TreeEval_h

#include "Expr.h"
#include "Stmt.h"
#include "Token.h"
#include "Value.h"
#include <string>
#include <vector>

Value evaluate(Expr& expr);


struct RunTimeError
{
    RunTimeError(Token token, std::string message):token{token}, message{message}{}
    
    Token token;
    std::string message;
};

void interpret(std::vector<std::unique_ptr<Stmt>>& statements, std::function<void(RunTimeError)> error_fun);

class TreeEval : public Expr::Visitor, public Stmt::Visitor
{
public:
    void visit(Binary& el);
    void visit(Grouping& el);
    void visit(Literal& el);
    void visit(Unary& el);
    void visit(ExprStmt& el);
    void visit(Print& el);
    
    std::string toString()
    {
        return Stringify (value);
    }
    
    Value value;
    
private:
    std::function<void(Token, std::string)> error;
 
};



#endif /* TreeEval_hpp */
