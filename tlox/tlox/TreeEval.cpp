//
//  TreeEval.cpp
//  tlox
//
//  Created by Ronald Legere on 8/26/22.
//

#include "TreeEval.h"
#include<iostream>

Value evaluate(Expr& expr)
{
    TreeEval visitor;
    expr.accept(visitor);
    return visitor.value;
}


void execute(Stmt& stmt)
{
    TreeEval visitor;
    stmt.accept(visitor);
}

void interpret(std::vector<std::shared_ptr<Stmt>>& statements, std::function<void(RunTimeError)> error_fun)
{
    try {
        for(auto statement : statements)
        {
            execute(*statement);
        }
    } catch (const RunTimeError& err) {
        error_fun(err);
    }
}


//Helper function, gets a double or throws runtime error if type is wrong
double getDouble(Token token, Value value)
{
    if(std::holds_alternative<double>(value))
       return std::get<double>(value);
    else
       throw RunTimeError(token,"Operand Must Be A Number.");
}

bool isTruthy(Value value)
{
    if(std::holds_alternative<std::monostate>(value))
        return false;
    if(std::holds_alternative<bool>(value))
        return std::get<bool>(value);
    return true;
}


bool isEqual(Value a, Value b)
{
    return a==b;
}

void TreeEval::visit(Binary& el)
{
    Value left = evaluate(*(el.left));
    Value right = evaluate(*(el.right));
 
    switch (el.op.type)
    {
        case TokenType::MINUS:
            value = getDouble(el.op,left) - getDouble(el.op, right);
            break;
        case TokenType::STAR:
            value = getDouble(el.op,left) * getDouble(el.op, right);
            break;
        case TokenType::SLASH:
            if(getDouble(el.op, right)  != 0.0)
                value = getDouble(el.op, left)  / getDouble(el.op, right) ;
            else
                throw RunTimeError(el.op, "Division by zero");
            break;
        case TokenType::PLUS:
            if(std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
                value = std::get<double>(left) + std::get<double>(right);
            else if(std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right))
                value = std::get<std::string>(left) + std::get<std::string>(right);
            else
                throw RunTimeError(el.op, "Operands not compatable.");
            break;
        case TokenType::GREATER:
            value = getDouble(el.op, left)  > getDouble(el.op, right);
            break;
        case TokenType::GREATER_EQUAL:
            value = getDouble(el.op, left)  >= getDouble(el.op, right);
            break;
        case TokenType::LESS:
            value = getDouble(el.op, left)  < getDouble(el.op, right);
            break;
        case TokenType::LESS_EQUAL:
            value = getDouble(el.op, left)  <= getDouble(el.op, right);
            break;

        case TokenType::EQUAL_EQUAL:
            value = isEqual(left, right);
            break;
        case TokenType::BANG_EQUAL:
            value = !isEqual(left, right);
            break;
            
        default:
            throw RunTimeError(el.op, "Illegal binary operation");
            
    }
     
}

void TreeEval::visit(Grouping& el)
{
    TreeEval inner;
    el.expression->accept(inner);
    value = inner.value;
    
}

void TreeEval::visit(Literal& el)
{
    value =   el.value;
     
}

void TreeEval::visit(Unary& el)
{
 
    Value inner = evaluate(*(el.right));
    
    switch (el.op.type)
    {
        case TokenType::MINUS:
            value = -getDouble(el.op, inner);
            break;
        case TokenType::BANG:
            value =!isTruthy(inner);
            break;
        default:  //Note this is not possible, parse error
            throw RunTimeError(el.op, "Illegal unary operator.");
    }
}

void TreeEval::visit(ExprStmt& el)
{
    evaluate(*el.expression);
    // discards return value
}
    
void TreeEval::visit(Print& el)
{
    value = evaluate(*el.expression);
    std::cout << toString() << "\n";
}
    
 
