import argparse
import os 


visitString = """
   void accept(Visitor& v) override
    {
        return v.visit(*this);
    }
""" 
indent4 = "    "

def genVisitor(classDesc):
    res = "\n" + indent4 +  "class Visitor\n" + indent4 + "{\n" +  indent4+ "public:\n"
    for (className, fieldlist) in classDesc:
        res += indent4 + indent4 + f"virtual void visit({className}& el)=0;\n"
    res += indent4 + "};\n\n"
    return res

def forwardDeclare(classDesc):
    res = ""
    for (className, fieldlist) in classDesc:
        res += f"class {className};\n"
    return res

def processField(field):
    (type, name) = field.split()
    if type[-1] == "*":
        type = f"std::shared_ptr<{type[:-1]}>"
    if type[:4] == "List":
        type = f"std::vector<std::shared_ptr<{type[5:-1]}>>"
    initializer = f"{name}{{{name}}}"
    return (type, name, initializer)


def genConstructor(className, fields):
    assignments = ', '.join([field[2] for field in fields]) # grab initializers
    fieldlist = ', '.join([field[0]+" "+field[1] for field in fields]) # grab types
    return indent4 + f"{className}({fieldlist}):" + assignments + "{}\n"
    

def genFields(fields):
    res = ""
    for field in fields:
        res += indent4+ field[0] + " " + field[1]+ ";\n"
    return res

def defineType(baseName,className, fieldlist):
    fields = [processField(f.strip()) for f in fieldlist.split(',')]
    # is a list of (type, name, initializer)
    boilerplate = f"\nclass {className} : public {baseName}\n" + "{\npublic:\n" + genConstructor(className,fields) +  visitString 
    return boilerplate + "\n"+ genFields(fields) + "\n};\n"



def defineAst(outputDir, baseName, types):
    path = outputDir + '/' + baseName + ".h"
    # write out the boilerplate
    with open(path, 'w') as f:
        f.write("// Generated by GenerateAst.py\n\n")
        f.write(f"#ifndef {baseName}_h\n")
        f.write(f"#define {baseName}_h\n")
        f.write("#include<vector>\n");
        f.write('#include "Token.h"\n\n\n')
        classdescs = [(type.split(':')[0].strip(),type.split(':')[1].strip()) for type in types]
        f.write(forwardDeclare(classdescs))
        f.write(f"class {baseName}\n")
        f.write("{\npublic:\n")
        f.write(genVisitor(classdescs))
        f.write("    virtual void accept(Visitor& v) = 0;\n};\n") 
        for (className, fieldlist) in classdescs:
            f.write(defineType(baseName,className, fieldlist))
        f.write("\n#endif\n")
    


parser = argparse.ArgumentParser(description='Generate AST ')
parser.add_argument('--dest', help='output directory, defaults to .' , default='.')
 

args = parser.parse_args()

# 

defineAst(args.dest, "Expr", ["Assign   : Token name, Expr* value",
                              "Binary : Expr* left, Token op, Expr* right",
                              "Grouping : Expr* expression",
                              "Literal  : Value value",
                              "Logical  : Expr* left,Token op, Expr* right",
                               "Unary   : Token op, Expr* right",
                               "Variable : Token name"
                              ])


defineAst(args.dest, "Stmt", ["Block      : List<Stmt> statements",
                              "ExprStmt : Expr* expression", 
                              "If      : Expr* condition, Stmt* thenBranch, Stmt* elseBranch",
                              "Print      : Expr* expression", 
                              "While : Expr* condition, Stmt* body",
                              "Var      : Token name, Expr* expression"])