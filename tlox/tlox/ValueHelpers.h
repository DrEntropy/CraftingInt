//
//  ValueHelpers.h
//  tlox
//
//  Created by Ronald Legere on 10/30/22.
//

#ifndef ValueHelpers_h
#define ValueHelpers_h

#include "Value.h"
#include "Callable.h"

// Visitor for literal variant. If too much of this refactor
struct Literal_to_string_vis
{
    std::string operator()(std::monostate)
    {
        return "NIL";
    }
    
    std::string operator()(std::string astring) {
        return astring;
    }
    
    std::string operator()(double value)
    {
        return std::to_string(value);
    }
    
    std::string operator()(bool value)
    {
        return value ? "TRUE" : "FALSE";
    }
    
    std::string operator()(std::shared_ptr<Callable> callable)
    {
        return *callable;
    }
};

inline std::string Stringify(Value v)
{
    return std::visit(Literal_to_string_vis(), v);
}

#endif /* ValueHelpers_h */
