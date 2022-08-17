//
//  AstPrint.cpp
//  tlox
//
//  Created by Ronald Legere on 8/17/22.
//

#include "AstPrint.h"

// THERE HAS to be a better way then creating all these sub visitors. 


void AstPrint::visit(Binary& el)
{
    AstPrint leftV, rightV;
    el.left->accept(leftV);
    el.right->accept(rightV);
    astString = "("+ el.op.lexeme + " " + leftV.toString() + " " + rightV.toString() + ")";
}

void AstPrint::visit(Grouping& el)
{
    AstPrint inner;
    el.expression->accept(inner);
    astString = "(group " + inner.toString() + ")";
    
}

void AstPrint::visit(Literal& el)
{
    astString = std::visit(Literal_to_string_vis(), el.value);
}

void AstPrint::visit(Unary& el)
{
    AstPrint inner;
    el.right->accept(inner);
    astString = "("+ el.op.lexeme + " " +inner.toString() + ")";
}
