//
//  Globals.cpp
//  tlox
//
//  Created by Ronald Legere on 10/31/22.
//

#include "Natives.h"
#include <chrono>


std::shared_ptr<Environment> make_global_enviroment()
{
    
    auto native{std::make_shared<Environment>()};
 
    // inner class
    
    struct Clock : Callable
    {
        int arity() override {return 0;}
        operator std::string()  override { return "<native fn>";}
        
        virtual Value call(std::vector<Value> arguments) override
        {
            // This seems overly complicated, typical c++
            using namespace std::chrono;
            auto ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
            return static_cast<double>(ms);
        }
        
    };
    
    native->define("Time", Clock{});
    
    
    auto global{native};
    return global;
    
}
