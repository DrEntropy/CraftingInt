//
//  main.cpp
//  tlox tree walking interpreter
//
//  Created by Ronald Legere on 7/16/22.
//

#include <iostream>
#include <string>

int main(int argc, const char * argv[]) {
    if(argc > 2)
        std::cout << "usage:tlox [script]\n";
    else if (argc == 2)
        std::cout << "running the file " + std::string(argv[1]) + "\n";
    else
        std::cout << "Running Interpreter \n";
    return 0;
}
