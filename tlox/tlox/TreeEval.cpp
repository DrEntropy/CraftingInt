//
//  TreeEval.cpp
//  tlox
//
//  Created by Ronald Legere on 8/26/22.
//

#include "TreeEval.h"
#include<iostream>

#include "Callable.h"
#include "LoxFunction.h"
#include "ReturnExcept.h"

Value evaluate(Expr& expr, std::shared_ptr<Environment> env)
{
    TreeEval visitor(env);
    expr.accept(visitor);
    return visitor.value;
}


std::pair<Value,bool> execute(Stmt& stmt, std::shared_ptr<Environment> env)
{
    TreeEval visitor(env);
    stmt.accept(visitor);
    return {visitor.value, visitor.broke};
}




Value interpret(std::vector<std::unique_ptr<Stmt>>& statements, std::function<void(RunTimeError)> error_fun, std::shared_ptr<Environment> env)
{
    std::pair<Value, bool> evalue;
    try {
        evalue = executeBlock(statements, env);
        return evalue.first;
    } catch (const RunTimeError& err) {
        error_fun(err);
        return Value{};
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
    Value left = evaluate(*(el.left), environment);
    Value right = evaluate(*(el.right), environment);
 
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

void TreeEval::visit(Call& expr)
{
    Value callee = evaluate(*(expr.callee), environment);
    
    std::vector<Value> arguments;
    
    for(auto argument : expr.arguments)
    {
        arguments.push_back(evaluate(*argument, environment));
    }
    
    // Makes sure the value is a callable and then call it. 
    if(std::holds_alternative<std::shared_ptr<Callable> >(callee))
    {
        auto function = std::get<std::shared_ptr<Callable>>(callee);
        
        if(arguments.size() != function->arity())
            throw RunTimeError(expr.paren,"Expected " + std::to_string(function->arity()) +
                               " arguments but got " +
                               std::to_string(arguments.size()) + ".");
        
        value = function->call(arguments);
    }
    else
        throw RunTimeError(expr.paren,"Non callable in call expression");
    
}

void TreeEval::visit(Grouping& el)
{
    TreeEval inner(environment);
    el.expression->accept(inner);
    value = inner.value;
    
}

void TreeEval::visit(Literal& el)
{
    value =   el.value;
     
}

void TreeEval::visit(Unary& el)
{
 
    Value inner = evaluate(*(el.right),environment);
    
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
    value = evaluate(*el.expression, environment);
    // save return value for REPL
}

void TreeEval::visit(Function& functiondecl)
{
    Value fun = std::make_shared<LoxFunction>(functiondecl.name.lexeme, functiondecl.params, functiondecl.body,  environment);
    // value not used
    environment->define(functiondecl.name.lexeme, fun);
}


void TreeEval::visit(AnonFunction& functiondecl)
{
    value = std::make_shared<LoxFunction>("<ANON>", functiondecl.params,  functiondecl.body, environment);
}
    
void TreeEval::visit(Print& el)
{
    value = evaluate(*el.expression, environment);
    std::cout << toString() << std::endl;  // force a flush
    value = Value();  // to avoid reprint in REPL
}

 
void TreeEval::visit(Var& el)
{
       auto initializer =  Value(); // set value monostate
       if (el.expression) {
         initializer = evaluate(*(el.expression), environment);
       }

       environment->define(el.name.lexeme, initializer);
}

void TreeEval::visit(Variable& el)
{
    value = environment->get(el.name);
}

void TreeEval::visit(Assign& el)
{
    value = evaluate(*el.value, environment);
    environment->assign(el.name, value);
}



void TreeEval::visit(If& stmt)
{
    if (isTruthy(evaluate(*stmt.condition, environment))) {
        std::tie(value, broke) = execute(*stmt.thenBranch, environment);
      } else if (stmt.elseBranch) {
        std::tie(value, broke) = execute(*stmt.elseBranch,environment);
      }
}

void TreeEval::visit(Logical& expr)
{
    auto left = evaluate(*expr.left, environment);

    if (expr.op.type == TokenType::OR)
    {
      if (isTruthy(left))
      {
          value = left;
          return;
      }

    } else
    {
      if (!isTruthy(left))
      {
          value = left;
          return;
      }
    }

    value = evaluate(*expr.right, environment);
}

void TreeEval::visit(While& stmt)
{
    std::pair<Value,bool> evalue;
    
    while (isTruthy(evaluate(*stmt.condition, environment))) {
         evalue = execute(*stmt.body, environment);
         if(evalue.second)
             break;
       }
 
}


void TreeEval::visit(Block& el)
{
    // create a new environment
    std::shared_ptr<Environment> inner_env = std::make_shared<Environment>();
    inner_env->enclosing = environment;
 
    
    for (auto& statement : el.statements)
    {
        std::tie(value, broke) = execute(*statement, inner_env);
 
        if(broke)
            break;
    }
}

void TreeEval::visit(Break& stmt)
{
    broke = true;
}


void TreeEval::visit(Return& stmt)
{
    Value value;
    if(stmt.value != nullptr)
        value = evaluate(*stmt.value, environment);
    
    throw ReturnExcept(value);
}
