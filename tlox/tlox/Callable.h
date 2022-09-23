//
//  Callable.h
//  tlox
//
//  Created by Ronald Legere on 9/23/22.
//

#ifndef Callable_h
#define Callable_h

class Callable
{
public:
    virtual void call() = 0;// to be fleshed out.
    
    virtual operator std::string() = 0;
};
#endif /* Callable_h */
