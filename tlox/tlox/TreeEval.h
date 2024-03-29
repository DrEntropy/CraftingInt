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
#include "ValueHelpers.h"
#include "Environment.h"
#include "RuntimeError.h"
#include <string>
#include <vector>

Value evaluate(Expr& expr, Environment& env);

std::pair<Value,bool> execute(Stmt& stmt, std::shared_ptr<Environment> env);


template <typename StatementPtr>
std::pair<Value,bool> executeBlock(std::vector<StatementPtr>& block, std::shared_ptr<Environment> env)
{
    std::pair<Value, bool> evalue;
    for(auto& statement : block)
    {
        if(statement)
           evalue = execute(*statement,env);
    }
    // return only last one.
    return evalue;
}

Value interpret(std::vector<std::unique_ptr<Stmt>>& statements, std::function<void(RunTimeError)> error_fun, std::shared_ptr<Environment> env);

class TreeEval : public Expr::Visitor, public Stmt::Visitor
{
public:
    TreeEval(std::shared_ptr<Environment> env) : environment {env}  {}
    
    void visit(Assign& el);
    void visit(Binary& el);
    void visit(Call& el);
    void visit(Function& function);
    void visit(AnonFunction& functionExpr);
    void visit(Grouping& el);
    void visit(Literal& el);
    void visit(Unary& el);
    void visit(ExprStmt& el);
    void visit(Print& el);
    void visit(Var& el);
    void visit(Variable& el);
    void visit(Block& el);
    void visit(Break& el);
    void visit(Return& el);
    void visit(If& el);
    void visit(Logical& el);
    void visit(While& el);

    
    std::string toString()
    {
        return Stringify (value);
    }
    
    Value value;
    bool broke{false}; // used to signfify a break statement was hit
    
private:
    std::shared_ptr<Environment> environment;
};



#endif /* TreeEval_hpp */
