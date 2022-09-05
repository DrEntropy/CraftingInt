//
//  RuntimeError.h
//  tlox
//
//  Created by Ronald Legere on 9/4/22.
//

#ifndef RuntimeError_h
#define RuntimeError_h
#include <string>
#include "Token.h"

struct RunTimeError
{
    RunTimeError(Token token, std::string message):token{token}, message{message}{}
    
    Token token;
    std::string message;
};

#endif /* RuntimeError_h */
