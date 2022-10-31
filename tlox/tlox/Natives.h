//
//  Globals.hpp
//  tlox
//
//  Created by Ronald Legere on 10/31/22.
//

// Defines global environment

#ifndef Natives_h
#define Natives_h

#include "Environment.h"

// Creates a global environment, including an enclosing 'super global' with natives.
std::shared_ptr<Environment> make_global_enviroment();




#endif /* Globals_hpp */
