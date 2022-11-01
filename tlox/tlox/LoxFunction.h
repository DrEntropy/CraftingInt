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
#include "ReturnExcept.h"

struct LoxFunction : Callable
{
    LoxFunction(std::string name, std::vector<Token> params,
                std::vector<std::shared_ptr<Stmt>> body, std::shared_ptr<Environment> env)
                : name{name}, params{params},body{body}, closure{env}{};
    
    Value call(std::vector<Value> arguments)  override
    {
        auto env{std::make_shared<Environment>(closure)};
        for (int i = 0; i < params.size(); i++)
        {
            env->define(params[i].lexeme, arguments[i]);
        }
        
        // this uses the exception mechanism to implement returns.
        try {
             auto [res, flag] = executeBlock(body, env);
             return res;
        } catch (ReturnExcept returnValue)
        {
            return returnValue.value;
        }
        
        
    }
    
    int arity() override {
        return params.size();
    }
    
    
    operator std::string() override
    {
        return "<fn " +  name + ">";
    }
    
                
    // keeps (shallow) copy of the declaration
    std::string name;
    std::vector< std::shared_ptr<Stmt> > body;
    std::vector<Token> params;
    
    std::shared_ptr<Environment> closure;
};
    
#endif /* LoxFunction_h */
