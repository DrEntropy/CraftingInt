import argparse
import os 


visitString = """
   void accept(Visitor& v) override
    {
        return v.visit(*this);
    }
""" 
indent4 = "    "

def genConstructor(className, fields, fieldlist):
    justnames = [field.split()[1] for field in fields]
    assignments = ','.join([field+"{"+field+"}" for field in justnames])
    return indent4 + f"{className}({fieldlist}):" + assignments + "{};\n"
    

def genFields(fields):
    res = ""
    for field in fields:
        res += indent4+ field + ";\n"
    return res

def defineType(baseName, type):
    className = type.split(':')[0].strip()
    fieldlist = type.split(':')[1].strip()
    fields = [f.strip() for f in fieldlist.split(',')]
    boilerplate = f"\nclass {className} : public {baseName}\n" + "{\npublic:\n" + genConstructor(className,fields, fieldlist) +  visitString 
    return boilerplate + "\n"+ genFields(fields) + "\n};\n"



def defineAst(outputDir, baseName, types):
    path = outputDir + '/' + baseName + ".h"
    # write out the boilerplate
    with open(path, 'w') as f:
        f.write("""// Generated by GenerateAst.py
#ifndef Expression_h
#define Expression_h
#include "Token.h"
#include "Visitor.h"\n\n\n""")
        f.write(f"class {baseName}\n")
        f.write("{\npublic:\n")
        f.write("    virtual void accept(Visitor& v) = 0;\n};\n") 
        for type in types:
            f.write(defineType(baseName, type))
        f.write("\n#endif\n")
    


parser = argparse.ArgumentParser(description='Generate AST ')
parser.add_argument('--dest', help='output directory, defaults to .' , default='.')
 

args = parser.parse_args()

defineAst(args.dest, "Expr", ["Binary : Expr& left, Token& op, Expr& right",
                              "Grouping : Expr& expression",
                              "Literal  : Value value",
                               "Unary   : Token& op, Expr& right"
                              ])