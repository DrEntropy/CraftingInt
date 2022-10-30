//
//  Environment.h
//  tlox
//
//  Created by Ronald Legere on 9/4/22.
//

#ifndef Environment_h
#define Environment_h

#include <memory>
#include <unordered_map>
#include "ValueHelpers.h"
#include "Token.h"
#include <string>
#include <stdexcept>
#include "RuntimeError.h"

struct Environment {
    std::weak_ptr<Environment> enclosing{};
    
    std::unordered_map<std::string, Value> values;
    
    void define(std::string name, Value value)
    {
        values[name] = value;
    }
    
    Value get(Token name)
    {
        try {
            return values.at(name.lexeme);
        } catch (const std::out_of_range& e) {
            if(std::shared_ptr<Environment> env = enclosing.lock())
                return env->get(name);
            
            throw RunTimeError(name, "Undefined Variable: " + name.lexeme + ".");
        }
    }
    
    void assign(Token name, Value value) {
        try {
            values.at(name.lexeme) = value;
        } catch (const std::out_of_range& e) {
        
        if(std::shared_ptr<Environment> env = enclosing.lock())
        {
            env->assign(name, value);
            return;
        }
        
        throw RunTimeError(name, "Undefined Variable: " + name.lexeme + ".");
        }
    }
};

#endif /* Environment_h */
