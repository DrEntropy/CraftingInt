// Generated by GenerateAst.py

#ifndef Expr_h
#define Expr_h
#include "Token.h"


class Binary;
class Grouping;
class Literal;
class Unary;
class Variable;
class Expr
{
public:

    class Visitor
    {
    public:
        virtual void visit(Binary& el)=0;
        virtual void visit(Grouping& el)=0;
        virtual void visit(Literal& el)=0;
        virtual void visit(Unary& el)=0;
        virtual void visit(Variable& el)=0;
    };

    virtual void accept(Visitor& v) = 0;
};

class Binary : public Expr
{
public:
    Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right):left{left}, op{op}, right{right}{}

   void accept(Visitor& v) override
    {
        return v.visit(*this);
    }

    std::shared_ptr<Expr> left;
    Token op;
    std::shared_ptr<Expr> right;

};

class Grouping : public Expr
{
public:
    Grouping(std::shared_ptr<Expr> expression):expression{expression}{}

   void accept(Visitor& v) override
    {
        return v.visit(*this);
    }

    std::shared_ptr<Expr> expression;

};

class Literal : public Expr
{
public:
    Literal(Value value):value{value}{}

   void accept(Visitor& v) override
    {
        return v.visit(*this);
    }

    Value value;

};

class Unary : public Expr
{
public:
    Unary(Token op, std::shared_ptr<Expr> right):op{op}, right{right}{}

   void accept(Visitor& v) override
    {
        return v.visit(*this);
    }

    Token op;
    std::shared_ptr<Expr> right;

};

class Variable : public Expr
{
public:
    Variable(Token name):name{name}{}

   void accept(Visitor& v) override
    {
        return v.visit(*this);
    }

    Token name;

};

#endif
