//
//  Callable.h
//  tlox
//
//  Created by Ronald Legere on 9/23/22.
//

#ifndef Callable_h
#define Callable_h

#include<vector>

#include "Value.h"

class TreeEval;

class Callable
{
public:
    virtual Value call(TreeEval evaluator, std::vector<Value> arguments) = 0;// to be fleshed out.
    
    virtual operator std::string() = 0;
    virtual int arity() = 0;
};

#endif /* Callable_h */
