# CraftingInterpreters

Workspace for working through "Crafting Interpreters" book by Robert Nystrom. 

The treewalk interpreter only goes through chapter 10, using C++

I have no plan to revisit this and instead will probably try again from the top (see note at the bottom) or another language.

## Tree Walk Notes

- Design notes:
    - C++ does not allow  a virtual member template function.   So i had the visitor return void.   To work around this, the visitors can use member variables to build up whatever values they want to make.  In the end this decision made chapter 11 no fun (would require some significant rewrites) so I opted to end the treewalk and skip to the bytecode
    - I used std::unique_ptr<Expr> for subtrees.  This means each tree owns its leaves, but I am not sure this was the right way. It makes things complicated because I have to keep calling std::make_unique<> every time i want to create a new node, and then it is moved inside the constructor. Also i don’t like the algebra, it seems wonky. Like if i create a subtree, i have to std::move it into the larger tree right? This seems like ti will happen a lot in parsing!
    - I don’t attach environments to the visitor, but rather just thread them through everywhere.
    - I use shared_ptr for the parse tree. At first this was a convenience to avoid multiple std::move calls but i know am glad i did as there will be sharing for function bodies.  Note that unique_ptr’s convert to shared and i do use this in some places in the parser.
    - I used shared_ptr for environments , because i am quite sure closures will mean shared ownership.   However this created a memory leak see below
    
- Known issues that need to be resolved
    - Fix ASTPrint to deal with statements / etc. (currently removed)
    - Clean up exceptions to, derive from std::runtime_exception? Note we no longer catch parse exceptions
    - Use unique_ptrs in parsers for expression (to avoid unneeded reference counts)
    - Issue with shared_ptr for environments: An environment can contain a callable which has a pointer to the enclosing environment. It must be shared because the closure has to own the environment.   So when that environment goes out of scope, if it contains a function definition, it will not be deleted since the function definition also has a pointer to the environment. Neither can be weak pointers !  i think this requires a proper garbage collector
    - Bug in parsing , goes nuts with this error:  `if (true) x= 3 else x =44;`  Note missing semicolon after x=3

## If Redoing the Tree Walk

If starting over, consider using a `std::variant`-based AST instead of the OOP visitor pattern. Define each node type as a plain struct, combine them into a `std::variant`, and use `std::visit` with overloaded callables for each pass. This avoids the void-returning visitor workaround entirely — each pass (evaluator, resolver, printer) naturally returns its own type. It would also eliminate the `GenerateAst.py` code generator and make adding the Resolver in Chapter 11 straightforward.

```cpp
// Forward declare a wrapper to break the recursion
struct Expr;

struct Binary { std::shared_ptr<Expr> left; Token op; std::shared_ptr<Expr> right; };
struct Unary  { Token op; std::shared_ptr<Expr> right; };
struct Literal { Value value; };
// ... etc

struct Expr {
    std::variant<Binary, Unary, Literal, Grouping, /*...*/> node;
};

// Each "visitor" is just a callable with overloads and its own return type:

struct Evaluator {
    Environment& env;
    Value operator()(Literal& lit)  { return lit.value; }
    Value operator()(Binary& bin)   { /* ... */ return result; }
    // ...
};

struct Resolver {
    ScopeStack& scopes;
    void operator()(Variable& var) { /* resolve */ }
    // ...
};

// Usage — natural return types, no member-variable hacks
Value val = std::visit(Evaluator{env}, expr.node);
```

