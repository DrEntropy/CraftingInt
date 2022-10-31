//
//  ReturnExcept.h
//  tlox
//
//  Created by Ronald Legere on 10/31/22.
//

#ifndef ReturnExcept_h
#define ReturnExcept_h
// used not as an exception but rather as control flow. Yuck.
#include "Value.h"

struct ReturnExcept
{
    ReturnExcept(Value val):value{val}{}
    
    Value value;
};


#endif /* ReturnExcept_h */
