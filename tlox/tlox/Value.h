//
//  Value.h
//  tlox
//
//  Created by Ronald Legere on 8/27/22.
//

#ifndef Value_h
#define Value_h

#include <variant>
#include <string>

// used for literal tokens and for Literal class to hold values.
using Value = std::variant<std::monostate, double, std::string, bool>;


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
};

inline std::string Stringify(Value v)
{
    return std::visit(Literal_to_string_vis(), v);
}
#endif /* Value_h */
