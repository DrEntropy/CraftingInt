//
//  Visitor.h
//  tlox
//
//  Created by Ronald Legere on 8/16/22.
//

#ifndef Visitor_h
#define Visitor_h

//TODO: write a python script to autogenerate this file too!
class Binary;
class Grouping;
class Literal;
class Unary;

class Visitor
{
public:

   virtual void visit(Binary& el) =0;
    virtual void visit(Grouping& el) = 0;
    virtual void visit(Literal& el)= 0;
    virtual void visit(Unary& el) = 0;
};


#endif /* Visitor_h */
