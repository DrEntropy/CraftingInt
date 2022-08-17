// Generated by GenerateAst.py
#ifndef Expr_h
#define Expr_h
#include "Token.h"


class Binary;
class Grouping;
class Literal;
class Unary;

class Visitor
{
public:
    virtual void visit(Binary& el)=0;
    virtual void visit(Grouping& el)=0;
    virtual void visit(Literal& el)=0;
    virtual void visit(Unary& el)=0;
};

class Expr
{
public:
    virtual void accept(Visitor& v) = 0;
};

class Binary : public Expr
{
public:
    Binary(Expr& left, Token& op, Expr& right):left{left},op{op},right{right}{};

   void accept(Visitor& v) override
    {
        return v.visit(*this);
    }

    Expr& left;
    Token& op;
    Expr& right;

};

class Grouping : public Expr
{
public:
    Grouping(Expr& expression):expression{expression}{};

   void accept(Visitor& v) override
    {
        return v.visit(*this);
    }

    Expr& expression;

};

class Literal : public Expr
{
public:
    Literal(Value value):value{value}{};

   void accept(Visitor& v) override
    {
        return v.visit(*this);
    }

    Value value;

};

class Unary : public Expr
{
public:
    Unary(Token& op, Expr& right):op{op},right{right}{};

   void accept(Visitor& v) override
    {
        return v.visit(*this);
    }

    Token& op;
    Expr& right;

};

#endif