// Generated by GenerateAst.py

#ifndef Stmt_h
#define Stmt_h
#include "Token.h"


class ExprStmt;
class Print;
class Var;
class Stmt
{
public:

    class Visitor
    {
    public:
        virtual void visit(ExprStmt& el)=0;
        virtual void visit(Print& el)=0;
        virtual void visit(Var& el)=0;
    };

    virtual void accept(Visitor& v) = 0;
};

class ExprStmt : public Stmt
{
public:
    ExprStmt(std::shared_ptr<Expr> expression):expression{expression}{}

   void accept(Visitor& v) override
    {
        return v.visit(*this);
    }

    std::shared_ptr<Expr> expression;

};

class Print : public Stmt
{
public:
    Print(std::shared_ptr<Expr> expression):expression{expression}{}

   void accept(Visitor& v) override
    {
        return v.visit(*this);
    }

    std::shared_ptr<Expr> expression;

};

class Var : public Stmt
{
public:
    Var(Token name, std::shared_ptr<Expr> expression):name{name}, expression{expression}{}

   void accept(Visitor& v) override
    {
        return v.visit(*this);
    }

    Token name;
    std::shared_ptr<Expr> expression;

};

#endif
