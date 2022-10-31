//
//  LoxFunction.h
//  tlox
//
//  Created by Ronald Legere on 10/31/22.
//

#ifndef LoxFunction_h
#define LoxFunction_h

#include "Callable.h"
#include "Parser.h"
#include "TreeEval.h"

struct LoxFunction : Callable
{
    LoxFunction(Function decl, std::shared_ptr<Environment> env) : declaration {decl}, closure{env}{};
    
    Value call(std::vector<Value> arguments)  override
    {
        auto env{std::make_shared<Environment>(closure)};
        for (int i = 0; i < declaration.params.size(); i++)
        {
            env->define(declaration.params[i].lexeme, arguments[i]);
        }
        
        auto [res, flag] = executeBlock(declaration.body, env);
        
        return res;
    }
    
    int arity() override {
        return declaration.params.size();
    }
    
    
    operator std::string() override
    {
        return "<fn " + declaration.name.lexeme + ">";
    }
    
                
    // keeps (shallow) copy of the declaration
    Function declaration;
    
    std::shared_ptr<Environment> closure;
};
    
#endif /* LoxFunction_h */
