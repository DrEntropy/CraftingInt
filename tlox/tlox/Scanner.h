//
//  Scanner.h
//  tlox
//
//  Created by Ronald Legere on 7/25/22.
//

#ifndef Scanner_h
#define Scanner_h

#include "Token.h"
#include <string>

class Scanner
{
public:
    
    Scanner(std::string& code):buffer{code} {
        
    }
private:
    std::string& buffer;
    //
};


#endif /* Scanner_h */
