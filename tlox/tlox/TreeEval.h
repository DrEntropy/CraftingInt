//
//  TreeEval.h
//  tlox
//
//  Created by Ronald Legere on 8/26/22.
//

#ifndef TreeEval_h
#define TreeEval_h

#include "Expr.h"
#include "Token.h"
#include <string>


class TreeEval : public Visitor
{
public:
    void visit(Binary& el);
    void visit(Grouping& el);
    void visit(Literal& el);
    void visit(Unary& el);
    
    std::string toString()
    {
        return std::visit(Literal_to_string_vis(), value);
    }
    
    Value value;
};



#endif /* TreeEval_hpp */