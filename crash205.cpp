 f { int i ; b2 , [ ] ( for ( i = 0 ; ; ) ) }
/*Checking crash205.cpp...

Program received signal SIGSEGV, Segmentation fault.
0x08292536 in Token::varId (this=0x0) at lib/token.h:442
442	        return _varId;
(gdb) backtrace
#0  0x08292536 in Token::varId (this=0x0) at lib/token.h:442
#1  0x08443344 in isVariableChanged (start=0x8547ee8, end=0x8547a08, varid=1)
    at lib/valueflow.cpp:260
#2  0x0844a3d0 in valueFlowForLoopSimplify (bodyStart=0x8547ee8, varid=1, 
    value=0, tokenlist=0xbfffe338, errorLogger=0xbfffeb3c, settings=0xbfffeb54)
    at lib/valueflow.cpp:1302
#3  0x0844b2e7 in valueFlowForLoop (tokenlist=0xbfffe338, 
    errorLogger=0xbfffeb3c, settings=0xbfffeb54) at lib/valueflow.cpp:1431
#4  0x0844c302 in ValueFlow::setValues (tokenlist=0xbfffe338, 
    errorLogger=0xbfffeb3c, settings=0xbfffeb54) at lib/valueflow.cpp:1580
#5  0x083fb055 in Tokenizer::tokenize (this=0xbfffe338, code=..., 
    FileName=0x8546f24 "crash205.cpp", configuration=..., noSymbolDB_AST=false)
    at lib/tokenize.cpp:1607
#6  0x0835d8b4 in CppCheck::checkFile (this=0xbfffeb3c, code=..., 
    FileName=0x8546f24 "crash205.cpp") at lib/cppcheck.cpp:355
#7  0x0835c298 in CppCheck::processFile (this=0xbfffeb3c, filename=..., 
    fileContent=...) at lib/cppcheck.cpp:235
#8  0x0835ace2 in CppCheck::check (this=0xbfffeb3c, path=...)
    at lib/cppcheck.cpp:66
#9  0x082701b7 in CppCheckExecutor::check_internal (this=0xbffff008, 
    cppcheck=..., argv=0xbffff114) at cli/cppcheckexecutor.cpp:743
#10 0x0826f391 in CppCheckExecutor::check (this=0xbffff008, argc=2, 
    argv=0xbffff114) at cli/cppcheckexecutor.cpp:189
*/
