//
//  PrettyPrint.h
//  tlox
//
//  Created by Ronald Legere on 8/17/22.
//

#ifndef AstPrint_h
#define AstPrint_h
#include "Expr.h"
#include <string>


class AstPrint : public Visitor
{
public:
    void visit(Binary& el);
    void visit(Grouping& el);
    void visit(Literal& el);
    void visit(Unary& el);
    std::string toString()
    {
        return astString;
    }
    
    std::string astString{};
};

#endif /* PrettyPrint_h */
