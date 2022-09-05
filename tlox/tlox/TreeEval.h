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
#include "Environment.h"
#include "RuntimeError.h"
#include <string>
#include <vector>

Value evaluate(Expr& expr, Environment& env);

Value interpret(std::vector<std::unique_ptr<Stmt>>& statements, std::function<void(RunTimeError)> error_fun, Environment& env);

class TreeEval : public Expr::Visitor, public Stmt::Visitor
{
public:
    TreeEval(Environment& env) : environment {env}  {}
    
    void visit(Assign& el);
    void visit(Binary& el);
    void visit(Grouping& el);
    void visit(Literal& el);
    void visit(Unary& el);
    void visit(ExprStmt& el);
    void visit(Print& el);
    void visit(Var& el);
    void visit(Variable& el);

    
    std::string toString()
    {
        return Stringify (value);
    }
    
    Value value;
    
private:
    Environment& environment;
};



#endif /* TreeEval_hpp */
