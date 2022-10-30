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

class Callable;

// used for all fully evaluated 'values'.  monostate is the nulll state.

using Value = std::variant<std::monostate, double, std::string, bool, std::shared_ptr<Callable>>;

 
#endif /* Value_h */
