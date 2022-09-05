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
#include "Value.h"
#include "Token.h"
#include <string>
#include <stdexcept>
#include "RuntimeError.h"

struct Environment {
    std::weak_ptr<Environment> parent{};
    
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
            throw RunTimeError(name, "Undefined Variable: " + name.lexeme + "");
        }
    }
    
};

#endif /* Environment_h */
