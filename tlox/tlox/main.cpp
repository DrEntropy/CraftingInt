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
#include "Stmt.h"
//#include "AstPrint.h"
#include "Parser.h"
#include "TreeEval.h"
#include "Natives.h"

// All of this crap should be in an error reporting class

bool hadError = false;
bool hadRuntimeError = false;


void report(int line, std::string where, std::string message)
{
    std::cerr << "[Line: " + std::to_string(line) + "] Error" + where + ": " + message << std::endl;
}

void error(int line, std::string message)
{
    report(line, "", message);
    hadError = true;
}

void parse_error(Token token, std::string message) {
    if (token.type == TokenType::EOF_T) {
      report(token.line, " at end", message);
    } else {
      report(token.line, " at '" + token.lexeme + "'", message);
    }
  }

void runtime_error(RunTimeError err)
{
    std::cerr << err.message << "\n[line " << err.token.line << "]\n";
    hadRuntimeError = true;
}



Value run(std::string source, std::shared_ptr<Environment> env)
{
    Scanner scanner{source, error};
    
    std::vector<Token> tokens = scanner.scanTokens();
    Parser parser{tokens, parse_error};
    if(hadError) return Value();
    
    
    // For now, just print the tokens.
    // for (Token token : tokens) {
   //   std::cout << token.toString() << "\n";
   // }
    std::vector< std::unique_ptr<Stmt> > statements = parser.parse();
     
 
    return interpret(statements, runtime_error,env);
    
 
}



void runFile(std::string filename)
{
    std::cout << "running the file " + filename + "\n";
    std::ifstream t(filename);
    if(t.is_open())
    {
        auto env = make_global_enviroment();
        std::stringstream buffer;
        buffer << t.rdbuf();
        run(buffer.str(), env);
        if(hadError)
            exit(EXIT_FAILURE);
        if(hadRuntimeError)
            exit(70); // verify this code
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
    auto env = make_global_enviroment();
    while(true)
    {
        std::getline(std::cin,line);
        if(std::cin.eof())
            break;
       auto value =  run(line, env);
        //if not null, print it for read eval loop
        if(! std::holds_alternative<std::monostate>(value))
              std::cout << "Value:" << Stringify(value) << "\n";
        
        hadError = false; // keep going even if there was an error
    }
    
}

// NOTE AstPrint was removed
//void testPrint()
//{
//    // TEST CODE  DELETE
//    std::unique_ptr<Expr> expr(new Binary(std::make_shared<Unary>(Token(TokenType::MINUS,"-",Value(),1 ),
//                                       std::make_shared<Literal>(Value(123.0))),Token(TokenType::STAR, "*", Value(), 1),
//                                       std::make_shared<Grouping>(  std::make_shared<Literal>(Value(45.67)))));
//  //  std::unique_ptr<Expr> expr(new Grouping(std::make_unique<Literal>(Value(45.67))));
//    AstPrint printer;
//    expr->accept(printer);
//    std::cout << printer.toString() << "\n";
//}



int main(int argc, const char * argv[])
{
    // TEST CODE  DELETE
   // testPrint();
   // return EXIT_SUCCESS;
    
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

 
