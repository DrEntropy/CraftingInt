//
//  TreeEval.cpp
//  tlox
//
//  Created by Ronald Legere on 8/26/22.
//

#include "TreeEval.h"

void TreeEval::visit(Binary& el)
{
    TreeEval leftV, rightV;
    el.left->accept(leftV);
    el.right->accept(rightV);
    //we will need to make sure the types are compatible.
    switch (el.op.type)
    {
        case TokenType::PLUS:
            break;
        default:
            break;
    }
     
}

void TreeEval::visit(Grouping& el)
{
    TreeEval inner;
    el.expression->accept(inner);
    // TODO
    
}

void TreeEval::visit(Literal& el)
{
    value =   el.value;
    //TODO
}

void TreeEval::visit(Unary& el)
{
    TreeEval inner;
    el.right->accept(inner);
    //TODO
}
