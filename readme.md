# CraftingInterpreters

Workspace for working through "Crafting Interpreters" book by Robert Nystrom. 

The treewalk interpreter only goes through chapter 10, using C++

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

