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
#include "Expr.h"
#include "AstPrint.h"


// All of this crap should be in an error reporting class

bool hadError = false;


void report(int line, std::string where, std::string message)
{
    std::cerr << "[Line: " + std::to_string(line) + "] Error" + where + ": " + message << std::endl;
}

void error(int line, std::string message)
{
    report(line, "", message);
    hadError = true;
}

// end poor error reporting ;)

void run(std::string source)
{
    Scanner scanner{source, error};
    std::vector<Token> tokens = scanner.scanTokens();

    // For now, just print the tokens.
    for (Token token : tokens) {
      std::cout << token.toString() << "\n";
    }
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
        if(hadError)
        {
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        std::cout << "Failed to open \n";
        exit(EXIT_FAILURE);
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
        hadError = false; // keep going even if there was an error
    }
    
}





int main(int argc, const char * argv[])
{
    // TEST CODE  DELETE
    std::unique_ptr<Expr> expr(new Binary(std::make_unique<Unary>(Token(TokenType::MINUS,"-",Value(),1 ),
                                       std::make_unique<Literal>(Value(123.0))),Token(TokenType::STAR, "*", Value(), 1),
                                       std::make_unique<Grouping>(  std::make_unique<Literal>(Value(45.67)))));
  //  std::unique_ptr<Expr> expr(new Grouping(std::make_unique<Literal>(Value(45.67))));
    AstPrint printer;
    expr->accept(printer);
    std::cout << printer.toString() << "\n";
    return EXIT_SUCCESS;
    
 // END TEST CODE
    
    // main
    if(argc > 2)
        std::cout << "usage:tlox [script]\n";
    else if (argc == 2)
        runFile(std::string(argv[1]));
    else
        runPrompt();
    return EXIT_SUCCESS;
}

 
