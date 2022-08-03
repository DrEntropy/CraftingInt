//
//  main.cpp
//  tlox tree walking interpreter
//
//  Created by Ronald Legere on 7/16/22.
//

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "Scanner.h"

void run(std::string source)
{
    // temp
    Scanner scanner{source};
    //std::cout << source << "\n";
}



void runFile(std::string filename)
{
    std::cout << "running the file " + filename + "\n";
    std::ifstream t(filename);
    if(t.is_open())
    {
        std::stringstream buffer;
        buffer << t.rdbuf();
        run(buffer.str());
    }
    else
    {
        std::cout << "Failed to open \n";
    }
}


void runPrompt()
{
    std::cout << "Running Interpreter \n";
    std::string line;
    while(true)
    {
        std::getline(std::cin,line);
        if(std::cin.eof())
            break;
        run(line);
    }
    
}





int main(int argc, const char * argv[])
{
    if(argc > 2)
        std::cout << "usage:tlox [script]\n";
    else if (argc == 2)
        runFile(std::string(argv[1]));
    else
        runPrompt();
    return 0;
}

 
