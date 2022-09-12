// Generated by GenerateAst.py

#ifndef Stmt_h
#define Stmt_h
#include<vector>
#include "Token.h"


class Block;
class ExprStmt;
class If;
class Print;
class While;
class Var;
class Stmt
{
public:

    class Visitor
    {
    public:
        virtual void visit(Block& el)=0;
        virtual void visit(ExprStmt& el)=0;
        virtual void visit(If& el)=0;
        virtual void visit(Print& el)=0;
        virtual void visit(While& el)=0;
        virtual void visit(Var& el)=0;
    };

    virtual void accept(Visitor& v) = 0;
};

class Block : public Stmt
{
public:
    Block(std::vector<std::shared_ptr<Stmt>> statements):statements{statements}{}

   void accept(Visitor& v) override
    {
        return v.visit(*this);
    }

    std::vector<std::shared_ptr<Stmt>> statements;

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

class If : public Stmt
{
public:
    If(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> thenBranch, std::shared_ptr<Stmt> elseBranch):condition{condition}, thenBranch{thenBranch}, elseBranch{elseBranch}{}

   void accept(Visitor& v) override
    {
        return v.visit(*this);
    }

    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> thenBranch;
    std::shared_ptr<Stmt> elseBranch;

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

class While : public Stmt
{
public:
    While(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body):condition{condition}, body{body}{}

   void accept(Visitor& v) override
    {
        return v.visit(*this);
    }

    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> body;

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
